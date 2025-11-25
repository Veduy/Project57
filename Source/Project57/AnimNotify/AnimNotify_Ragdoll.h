// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Ragdoll.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API UAnimNotify_Ragdoll : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
