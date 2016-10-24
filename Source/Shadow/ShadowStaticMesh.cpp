// Fill out your copyright notice in the Description page of Project Settings.

#include "Shadow.h"
#include "ShadowStaticMesh.h"


// Sets default values
AShadowStaticMesh::AShadowStaticMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	NormalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Normal Mesh"));
	NormalMesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	NormalMesh->Mobility = EComponentMobility::Static;
	NormalMesh->bGenerateOverlapEvents = false;
	NormalMesh->bUseDefaultCollision = true;

	RootComponent = NormalMesh;

	ShadowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shadow Mesh"));
	ShadowMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	ShadowMesh->Mobility = EComponentMobility::Static;
	ShadowMesh->bGenerateOverlapEvents = false;
	ShadowMesh->SetVisibility(false);
	ShadowMesh->bUseDefaultCollision = true;
	ShadowMesh->SetupAttachment(RootComponent);
}




void AShadowStaticMesh::ActivateShadow(bool bMode)
{
	NormalMesh->SetVisibility(!bMode);
	ShadowMesh->SetVisibility(bMode);
}
