// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UObject/ObjectMacros.h"
#include "LobbyGS.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeftTimeChanged, int32, Value);

/**
 * 
 */
UCLASS()
class PROJECT57_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY() 

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_LeftTime();

	UFUNCTION()
	void OnRep_ConnectionCount();

	FOnLeftTimeChanged LeftTimeChanged;
	FOnLeftTimeChanged ConnectionChanged;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing=OnRep_LeftTime)
	int32 LeftTime = 60;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing=OnRep_ConnectionCount)
	int32 ConnectionCount = 0;

};
