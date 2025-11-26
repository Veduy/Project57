// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Ragdoll.h"
#include "../Base/BaseCharacter.h"

FString UAnimNotify_Ragdoll::GetNotifyName_Implementation() const
{
	return FString("Ragdoll");
}

void UAnimNotify_Ragdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Character->DoDeathEnd();
	}
}
