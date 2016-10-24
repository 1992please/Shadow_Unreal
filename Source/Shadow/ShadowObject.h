// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ShadowObject.generated.h"

/**
 * 
 */
UINTERFACE()
class SHADOW_API UShadowObject : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SHADOW_API IShadowObject
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION()
	virtual void ActivateShadow(bool bMode);
};
