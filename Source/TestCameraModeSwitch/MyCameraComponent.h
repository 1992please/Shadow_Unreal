// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "MyCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class TESTCAMERAMODESWITCH_API UMyCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
		
	UMyCameraComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

public:
	void SwitchToOrthogonal(bool bMode);

	void UpdateSwitching(float Factor);

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float SwitchTime;

private:
	bool bSwitching;

	bool bSwitchMode;

	float Timer;

};
