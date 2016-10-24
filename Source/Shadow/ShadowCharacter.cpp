// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadow.h"
#include "ShadowCharacter.h"
#include "MyCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShadowGameMode.h"
//////////////////////////////////////////////////////////////////////////
// AShadowCharacter
AShadowCharacter::AShadowCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UMyCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

												   // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
												   // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	b2DMode = false;
	bTransitionCameraMode = false;
	CameraSwitchTime = .2f;
	Volume2DTag = "CameraOrthSwitch";
	bShadowMode = false;
	FearImmunity = 5;
}

void AShadowCharacter::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(Volume2DTag))
	{
		SwitchTo2DMode(true);
		if (bShadowMode)
		{
			FollowCamera->SwitchToOrthogonal(true);
		}
	}
}

void AShadowCharacter::NotifyActorEndOverlap(AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(Volume2DTag))
	{
		SwitchTo2DMode(false);
		if (bShadowMode)
		{
			FollowCamera->SwitchToOrthogonal(false);
		}
	}
}

void AShadowCharacter::ActivateShadow(bool bMode)
{
	//GetMesh()->SetMaterial(0, bMode?ShadowMaterial:NormalMaterial);
	bShadowMode = bMode;
	if (b2DMode)
	{
		FollowCamera->SwitchToOrthogonal(bMode);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShadowCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("SwitchCameraMode", IE_Pressed, this, &AShadowCharacter::EnterShadowWorld);
	InputComponent->BindAction("SwitchCameraMode", IE_Released, this, &AShadowCharacter::ExitShadowWorld);


	InputComponent->BindAxis("MoveForward", this, &AShadowCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AShadowCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AShadowCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AShadowCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AShadowCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AShadowCharacter::TouchStopped);
}

void AShadowCharacter::Tick(float DeltaSeconds)
{
	if (bTransitionCameraMode)
	{
		InterpFactor += DeltaSeconds;
		const float Factor = InterpFactor / CameraSwitchTime;
		if (Factor <= 1)
		{
			if (!b2DMode)
			{

				CameraBoom->SetWorldRotation(UKismetMathLibrary::RLerp(InterpRotation, GetViewRotation(), Factor, true));
				CameraBoom->TargetArmLength = FMath::Lerp(1000.f, 300.f, Factor);

			}
			else
			{
				CameraBoom->SetWorldRotation(UKismetMathLibrary::RLerp(InterpRotation, FRotator::ZeroRotator, Factor, true));
				CameraBoom->TargetArmLength = FMath::Lerp(300.f, 1000.f, Factor);
			}
		}
		else
		{
			bTransitionCameraMode = false;
			if (!b2DMode)
			{
				CameraBoom->bUsePawnControlRotation = true;
			}
			else
			{
				//FollowCamera->ProjectionMode = ECameraProjectionMode::Orthographic;

			}
		}
	}
	else if (b2DMode)
	{
		CameraBoom->SetWorldRotation(FRotator::ZeroRotator);
	}

	if (bShadowMode)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, "LOL");

		FearTimer += DeltaSeconds;
		if (FearTimer >= FearImmunity)
		{
			ExitShadowWorld();
		}
	}
	else if(FearTimer > 0)
	{
		FearTimer -= DeltaSeconds;
	}
}

float AShadowCharacter::GetFearPercentage() const
{
	return FearTimer / FearImmunity;
}

void AShadowCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}

}

void AShadowCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AShadowCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShadowCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShadowCharacter::SwitchTo2DMode(bool bMode)
{
	b2DMode = bMode;
	bTransitionCameraMode = true;
	InterpFactor = 0;

	if (!b2DMode)
	{
		InterpRotation = CameraBoom->GetComponentRotation();
	}
	else
	{
		CameraBoom->bUsePawnControlRotation = false;
		InterpRotation = CameraBoom->GetComponentRotation();
	}
}

void AShadowCharacter::EnterShadowWorld()
{
	AShadowGameMode* GameMode = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->SwitchShadow(true);
	}
}

void AShadowCharacter::ExitShadowWorld()
{
	AShadowGameMode* GameMode = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->SwitchShadow(false);
	}
}

void AShadowCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (!b2DMode)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(Direction, Value);
		}
	}
}

void AShadowCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (!b2DMode)
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
		else
		{
			const FVector Direction(0, 1, 0);
			AddMovementInput(Direction, Value);

		}
	}
}
