// Fill out your copyright notice in the Description page of Project Settings.

#include "TestCameraModeSwitch.h"
#include "MyCameraComponent.h"

UMyCameraComponent::UMyCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bSwitching = false;
	bSwitchMode = false;
	Timer = 0;
	SwitchTime = .4f;
}

void UMyCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bSwitching)
	{
		Timer += DeltaTime;
		UpdateSwitching(Timer / SwitchTime);
	}
}

void UMyCameraComponent::SwitchToOrthogonal(bool bMode)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::SanitizeFloat(bSwitching));

	bSwitching = true;
	bSwitchMode = bMode;
	Timer = 0;
	if (!bMode)
	{
		ProjectionMode = ECameraProjectionMode::Perspective;
	}
}

void UMyCameraComponent::UpdateSwitching(float Factor)
{
	if (Factor <= .5)
	{
		Factor *= 2;
		FieldOfView = FMath::Lerp(90, 120, Factor);
	}
	else if (Factor <= 1)
	{
		Factor = (Factor - .5f) * 2;
		FieldOfView = FMath::Lerp(120, 90, Factor);
	}
	else
	{
		if (bSwitchMode)
		{
			ProjectionMode = ECameraProjectionMode::Orthographic;
			OrthoNearClipPlane = 610.09021f;
			OrthoWidth = 2908.819336;
		}
		//reset variables
		bSwitching = false;
	}
}
