// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchUpTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "CUSkeletalMeshComponent.generated.h"

UCLASS()
class CATCHUP_API UCUSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<EGameRole, FMeshSet> MeshSets;

public:

	void SetRoleMesh(const EGameRole& GameRole);
	
};
