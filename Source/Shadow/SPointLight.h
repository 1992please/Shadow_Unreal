// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/PointLight.h"
#include "SPointLight.generated.h"

/**
 * 
 */
UCLASS()
class SHADOW_API ASPointLight : public APointLight
{
	GENERATED_BODY()
private:
	ASPointLight();

	class AShadowCharacter* DamagedPlayer;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;

	// override Actor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	//End of override
public:
	UPROPERTY(EditAnywhere, Category = Curve)
	UCurveFloat* Curve;

private:
//overriden to avoid Erros
#if WITH_EDITOR
		//~ Begin AActor Interface.
		virtual void EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	//~ End AActor Interface.
#endif

	//~ Begin UObject Interface.
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void LoadedFromAnotherClass(const FName& OldClassName) override;
#endif
	//~ End UObject Interface.
	
};
