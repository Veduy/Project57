// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"

#include "LobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyGS.h"

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
	}


}

void ALobbyPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}
