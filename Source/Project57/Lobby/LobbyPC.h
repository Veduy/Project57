// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

class ULobbyWidget;
/**
 * 
 */
UCLASS()
class PROJECT57_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPC();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<ULobbyWidget> LobbyWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;
};
