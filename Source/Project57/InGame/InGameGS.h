// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameGS.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API AInGameGS : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AInGameGS();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	void OnRep_RemainingPlayerCount();

public:
	UPROPERTY(ReplicatedUsing = OnRep_RemainingPlayerCount, VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 RemainingPlayerCount;
};
