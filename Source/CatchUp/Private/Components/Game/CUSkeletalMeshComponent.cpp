// Fill out your copyright notice in the Description page of Project Settings.

#include "CUSkeletalMeshComponent.h"

void UCUSkeletalMeshComponent::SetRoleMesh(const EGameRole& GameRole)
{
	check(MeshSets.Contains(GameRole));
	
	SetSkeletalMesh(MeshSets[GameRole].Mesh);
	SetAnimInstanceClass(MeshSets[GameRole].AnimationClass);
}
