// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "LobbyWidget.h"

#include "../Network/NetworkUtil.h"

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, ConnectionCount);
}

void ALobbyGS::OnRep_LeftTime()
{
	LeftTimeChanged.Broadcast(LeftTime);
}

void ALobbyGS::OnRep_ConnectionCount()
{
	NET_LOG(TEXT("OnRep_ConnectionCount()"));
	ConnectionChanged.Broadcast(ConnectionCount);
}
