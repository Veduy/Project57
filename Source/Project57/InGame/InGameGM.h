// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameGM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API AInGameGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UFUNCTION()
	void CheckLastPlayerLeft();
};
