// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ShadowObject.h"
#include "GameFramework/Character.h"
#include "ShadowCharacter.generated.h"

UENUM(BlueprintType)
enum class EBarkourState
{
	GroundedNormal,
	GroundedActive,
	WallRunning,
	Hanging,
	ClimbingEdge,
	Rolling,
	InAir
};

UCLASS()
class SHADOW_API AShadowCharacter : public ACharacter, public IShadowObject
{
	GENERATED_BODY()

	AShadowCharacter();
	// override Actor
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	//End of override Actor
	void ActivateShadow(bool bMode) override;
	// override shadowObject

	// end of override 
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UMyCameraComponent* FollowCamera;

	///** Tracing Objects */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//USphereComponent* SphereTracer;
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

protected:
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> MeshMats;

	UPROPERTY(VisibleAnywhere, Category = Tags)
	FName Volume2DTag;

	UPROPERTY(EditDefaultsOnly, Category = "2DMode")
	float CameraSwitchTime;

	bool bTransitionCameraMode;

	bool b2DMode;

	bool bShadowMode;

	float InterpFactor;

	FRotator InterpRotation;

	void SwitchTo2DMode(bool bMode);

	void EnterShadowWorld();

	void ExitShadowWorld();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	virtual void Jump() override;

	virtual void StopJumping() override;

	void FocusedModeOn();

	void FocusedModeOff();
//protected:
//	UFUNCTION()
//	void OnSphereTracerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
//	UFUNCTION()
//	void OnSphereTracerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	UPROPERTY(EditDefaultsOnly, Category = Barkour)
	float HangingOffset;

	UPROPERTY(EditDefaultsOnly, Category = Barkour)
	UAnimMontage* ClimbWallAnim;

	// to be called from the anim blueprint
	UFUNCTION(BlueprintCallable, Category = Barkour)
	void SetBarkourState(EBarkourState NewState);

	UFUNCTION(BlueprintPure, Category = Barkour)
	EBarkourState GetBarkourState() const;

private:
	float WallRunningOldAltitude;
	float WallRunningNewAltitude;
	float WallRunningFactor;


	bool bFocusedMode;

	EBarkourState BarkourState;

	FVector WallTraceImpactPoint;

	FVector WallNormal;

	FVector LedgeImpactPoint;

	bool CheckCanGrab();
	
	bool CheckCanWallRun();

	void UpdateTracers(float DeltaTime = .1f);

	bool TraceLowerForward(FHitResult& HitOut);

	bool TraceMiddleForward(FHitResult& HitOut);

	bool TraceUpperDownward(FHitResult& HitOut);

	bool TraceUpperForward(FHitResult& HitOut);

	bool TraceWall(FVector Start, FVector End, FHitResult& HitOut);

	FVector2D MovementInput;

	FVector GetAxisDirection();

	bool bCanWallRun;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fear")
	float FearImmunity;

	float FearTimer;

	float BaseTimeForRegenration;

	float HealthRegenrationPerSecond;

	float HealthTimer;


	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaSeconds) override;
private:
	bool bDamaged;

	void UpdateCameraPosition(float DeltaSeconds);

	void UpdateFearTimer(float DeltaSeconds);

	void UpdateHealth(float DeltaSeconds);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Character)
	float MaxHealth;

	float Health;


	void UpdateBurnParameter();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UMyCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = Fear)
	float GetFearPercentage() const;

	UFUNCTION(BlueprintPure, Category = Fear)
	float GetHealthPercentage() const;
protected:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Shadow, EditAnywhere, BlueprintReadOnly)
	UMaterialInstance* ShadowMaterial;	

	UPROPERTY(Category = Shadow, EditAnywhere, BlueprintReadOnly)
	UMaterialInstance* NormalMaterial;
};
