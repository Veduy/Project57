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
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendMessage(const FText& Message);
	void ServerSendMessage_Implementation(const FText& Message);
	bool ServerSendMessage_Validate(const FText& Message);

	UFUNCTION(Client, Reliable)
	void ClientSendMessage(const FText& Message); // 언리얼 헤더툴이 자동 생성해줌.
	void ClientSendMessage_Implementation(const FText& Message);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<ULobbyWidget> LobbyWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;
};
