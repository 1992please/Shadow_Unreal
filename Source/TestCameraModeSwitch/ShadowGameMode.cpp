// Fill out your copyright notice in the Description page of Project Settings.

#include "TestCameraModeSwitch.h"
#include "ShadowGameMode.h"
#include "ShadowObject.h"

AShadowGameMode::AShadowGameMode()
{
	bShadowWorld = false;
}

void AShadowGameMode::SwitchShadow(bool bShadowMode)
{
	if(bShadowMode == bShadowWorld)
		return;

	bShadowWorld = !bShadowWorld;
	UWorld* World = GetWorld();
	// We do nothing if not class provided, rather than giving ALL actors!
	if (World)
	{
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			IShadowObject* ShadowObject = Cast<IShadowObject>(Actor);
			if (ShadowObject && !Actor->IsPendingKill())
			{
				ShadowObject->ActivateShadow(bShadowWorld);
			}
		}
	}
}

bool AShadowGameMode::IsShadowWorldActive() const
{
	return bShadowWorld;
}