// Fill out your copyright notice in the Description page of Project Settings.

#include "CUSkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

void UCUSkeletalMeshComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCUSkeletalMeshComponent, CurrentMesh);
}

void UCUSkeletalMeshComponent::SetRoleMesh(const EGameRole& GameRole)
{
	check(MeshSets.Contains(GameRole));

	if (CurrentMesh != MeshSets[GameRole].Mesh)
	{
		CurrentMesh = MeshSets[GameRole].Mesh;
		
		SetSkeletalMesh(CurrentMesh);
		SetAnimInstanceClass(MeshSets[GameRole].AnimationClass);	
	}
}

USkeletalMesh* UCUSkeletalMeshComponent::GetCurrentMesh() const
{
	return CurrentMesh;
}
