// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadow.h"
#include "SPointLight.h"
#include "ShadowCharacter.h"

ASPointLight::ASPointLight()
{
	PrimaryActorTick.bCanEverTick = true;
	// initialized Sphere Collider
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionProfileName("OverlapAll");
	Sphere->SetupAttachment(PointLightComponent);
}

void ASPointLight::NotifyActorBeginOverlap(AActor * OtherActor)
{
	AShadowCharacter* SC = Cast<AShadowCharacter>(OtherActor);
	
	if (SC)
	{
		DamagedPlayer = SC;
	}
}

void ASPointLight::NotifyActorEndOverlap(AActor * OtherActor)
{
	AShadowCharacter* SC = Cast<AShadowCharacter>(OtherActor);
	if (SC == DamagedPlayer)
	{
		DamagedPlayer = NULL;
	//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "Exited Light");
	}
}

void ASPointLight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// in case designer decided to use default radius
	Sphere->SetSphereRadius(PointLightComponent->AttenuationRadius);
}

void ASPointLight::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (DamagedPlayer && Curve)
	{
		const float DistanceToPlayer = FVector::Dist(DamagedPlayer->GetActorLocation(), GetActorLocation());
		const float Factor = 1 - DistanceToPlayer/ PointLightComponent->AttenuationRadius;
		GEngine->AddOnScreenDebugMessage(0, 10, FColor::White, FString::SanitizeFloat(Curve->GetFloatValue(Factor)));
	}
}

/// Overriden to avoid Errors
#if WITH_EDITOR
void ASPointLight::EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	const FVector ModifiedScale = DeltaScale * (AActor::bUsePercentageBasedScaling ? 10000.0f : 100.0f);

	FMath::ApplyScaleToFloat(PointLightComponent->AttenuationRadius, ModifiedScale, 1.0f);
	Sphere->SetSphereRadius(PointLightComponent->AttenuationRadius);

	PostEditChange();
}
#endif

void ASPointLight::PostLoad()
{
	AActor::PostLoad();

	if (GetLightComponent()->Mobility == EComponentMobility::Static)
	{
		GetLightComponent()->LightFunctionMaterial = NULL;
	}
}

void ASPointLight::LoadedFromAnotherClass(const FName & OldClassName)
{
	AActor::LoadedFromAnotherClass(OldClassName);

	if (GetLinkerUE4Version() < VER_UE4_REMOVE_LIGHT_MOBILITY_CLASSES)
	{
		static FName PointLightStatic_NAME(TEXT("PointLightStatic"));
		static FName PointLightMovable_NAME(TEXT("PointLightMovable"));
		static FName PointLightStationary_NAME(TEXT("PointLightStationary"));

		check(GetLightComponent() != NULL);

		if (OldClassName == PointLightStatic_NAME)
		{
			GetLightComponent()->Mobility = EComponentMobility::Static;
		}
		else if (OldClassName == PointLightMovable_NAME)
		{
			GetLightComponent()->Mobility = EComponentMobility::Movable;
		}
		else if (OldClassName == PointLightStationary_NAME)
		{
			GetLightComponent()->Mobility = EComponentMobility::Stationary;
		}
	}
}
 