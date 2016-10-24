// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ShadowObject.h"
#include "GameFramework/Actor.h"
#include "ShadowStaticMesh.generated.h"

UCLASS()
class TESTCAMERAMODESWITCH_API AShadowStaticMesh : public AActor, public IShadowObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShadowStaticMesh();

	void ActivateShadow(bool bMode) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* NormalMesh;
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* ShadowMesh;
};
