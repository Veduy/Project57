// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "LobbyWidget.h"

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, ConnectionCount);
}

void ALobbyGS::OnRep_LeftTime()
{
	//ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//
	//if (PC && PC->LobbyWidgetObject)
	//{
	//	PC->LobbyWidgetObject->UpdateLeftTime(LeftTime);
	//}
	LeftTimeChanged.Broadcast(LeftTime);
}
