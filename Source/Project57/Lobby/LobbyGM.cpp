// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"

#include "../Network/NetworkUtil.h"
#include "LobbyGS.h"
#include "UObject/UObjectGlobals.h"

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
				GS->LeftTime--;
				GS->OnRep_LeftTime();
			}}),
			1.f,
			true,
			0.f);
}

void ALobbyGM::StartPlay()
{
	Super::StartPlay();
}
