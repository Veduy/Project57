// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"

#include "../Network/NetworkUtil.h"
#include "UObject/UObjectGlobals.h"
#include "LobbyGS.h"
#include "LobbyPC.h"

void ALobbyGM::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* ALobbyGM::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return PC;
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	NET_LOG(TEXT("PostLogin"));

	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		TArray<APlayerController*> PlayerControllers;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PC = Iterator->Get();
			if (PC)
			{
				PlayerControllers.Add(PC);
			}
		}

		GS->ConnectionCount = PlayerControllers.Num();
		GS->OnRep_ConnectionCount();
	}
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LeftTimerHandle,
		FTimerDelegate::CreateLambda([this]() {
			ALobbyGS* GS = GetGameState<ALobbyGS>();
			if (GS)
			{
				if (GS->LeftTime > 0)
				{
					GS->LeftTime--;
					GS->OnRep_LeftTime();
				}
				else
				{
					GetWorld()->GetTimerManager().ClearTimer(LeftTimerHandle);
					GetWorld()->ServerTravel(TEXT("InGameMap"));
				}
			}}),
			1.f,
			true,
			0.f);
}

void ALobbyGM::StartPlay()
{
	Super::StartPlay();
}

void ALobbyGM::StartGame()
{
	GetWorld()->GetTimerManager().ClearTimer(LeftTimerHandle);

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*Iter);
		if (PC)
		{
			PC->ClientShowLoadingScreen();
		}
	}

	GetWorld()->ServerTravel(TEXT("InGameMap"));
}
