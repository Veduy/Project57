// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API ABasePC : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePC();

public:
	UFUNCTION()
	void FireAim();
};
