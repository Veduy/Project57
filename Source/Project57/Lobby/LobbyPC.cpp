// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"

#include "LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGS.h"

#include "../Network/NetworkUtil.h"

ALobbyPC::ALobbyPC()
{

}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass && IsLocalController())
	{
		LobbyWidgetObject = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		LobbyWidgetObject->AddToViewport();

		if (HasAuthority())
		{
			LobbyWidgetObject->ShowStartButton();
		}

		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());

		InputComponent->BindAction(FName("PressEnter"), IE_Pressed, this, &ALobbyPC::OnShowChatInputBox);
		UE_LOG(LogTemp, Warning, TEXT("Input Binding"));
	}
}

void ALobbyPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ALobbyPC::ServerSendMessage_Implementation(const FText& Message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		NET_LOG("");
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			PC->ClientSendMessage(Message);
		}
	}
}

bool ALobbyPC::ServerSendMessage_Validate(const FText& Message)
{
	return true;
}

void ALobbyPC::ClientSendMessage_Implementation(const FText& Message)
{
	if (LobbyWidgetObject)
	{
		LobbyWidgetObject->AddMessage(Message);
	}
}

void ALobbyPC::ClientShowLoadingScreen_Implementation()
{
	if (LoadingWidgetClass)
	{
		if (IsLocalController())
		{
			UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
			if (LoadingWidget)
			{
				NET_LOG("Add to Viewport");
				LoadingWidget->AddToViewport(1);
			}
		}
	}
	else
	{
		NET_LOG("LoadingWidgetClass is Empty");
	}
}

void ALobbyPC::OnShowChatInputBox()
{
	if (LobbyWidgetObject)
	{
		LobbyWidgetObject->ShowChatInputBox();
	}
}
