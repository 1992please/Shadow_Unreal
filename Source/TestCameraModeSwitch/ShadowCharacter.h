// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ShadowObject.h"
#include "GameFramework/Character.h"
#include "ShadowCharacter.generated.h"

UCLASS()
class TESTCAMERAMODESWITCH_API AShadowCharacter : public ACharacter, public IShadowObject
{
	GENERATED_BODY()

		AShadowCharacter();
	// override Actor
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
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected:
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

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fear")
	float FearImmunity;

	float FearTimer;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaSeconds) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UMyCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = Fear)
	float GetFearPercentage() const;

protected:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Shadow, EditAnywhere, BlueprintReadOnly)
	UMaterialInstance* ShadowMaterial;	

	UPROPERTY(Category = Shadow, EditAnywhere, BlueprintReadOnly)
	UMaterialInstance* NormalMaterial;
};
