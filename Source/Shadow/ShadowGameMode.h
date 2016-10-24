// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "ShadowGameMode.generated.h"

/**
 * 
 */

UCLASS()
class SHADOW_API AShadowGameMode : public AGameMode
{
	GENERATED_BODY()
		AShadowGameMode();
public:
	void SwitchShadow(bool bShadowMode);

	UFUNCTION(BlueprintPure, Category = ShadowGamePlay)
	bool IsShadowWorldActive() const;

private:
	bool bShadowWorld;
};
