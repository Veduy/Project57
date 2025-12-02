// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGMBase.h"

#include "../Project57.h"
#include "../Network/NetworkUtil.h"

void ANetworkGMBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* ANetworkGMBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return PC;
}

void ANetworkGMBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ANetworkGMBase::BeginPlay()
{
	Super::BeginPlay();
}

void ANetworkGMBase::StartPlay()
{
	Super::StartPlay();
}

void ANetworkGMBase::PostNetInit()
{
}
