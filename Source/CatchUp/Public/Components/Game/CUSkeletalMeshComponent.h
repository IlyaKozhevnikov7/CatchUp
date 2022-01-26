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

	UPROPERTY(Replicated)
	USkeletalMesh* CurrentMesh;

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	
	void SetRoleMesh(const EGameRole& GameRole);

	USkeletalMesh* GetCurrentMesh() const;
	
	//USkeletalMesh* GetRoleMesh(const EGameRole& GameRole) const;
	
};
