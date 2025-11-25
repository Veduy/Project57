// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Ragdoll.h"

FString UAnimNotify_Ragdoll::GetNotifyName_Implementation() const
{
	return FString("Ragdoll");
}

void UAnimNotify_Ragdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(FName("Ragdoll"), true);
}
