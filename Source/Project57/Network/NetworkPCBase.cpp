// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPCBase.h"

#include "NetworkUtil.h"

void ANetworkPCBase::BeginPlay()
{
	NET_LOG(TEXT("Begin"));

	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		NET_LOG(TEXT("ENetRole::ROLE_Authority"));
	}
	else if(GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		NET_LOG(TEXT("ENetRole::ROLE_AutonomousProxy"));
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		NET_LOG(TEXT("ENetRole::ROLE_SimulatedProxy"));
	}


	if (GetRemoteRole() == ENetRole::ROLE_Authority)
	{
		NET_LOG(TEXT("GetRemoteRole::ROLE_Authority"));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
	{
		NET_LOG(TEXT("GetRemoteRole::ROLE_AutonomousProxy"));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
	{
		NET_LOG(TEXT("GetRemoteRole::ROLE_SimulatedProxy"));
	}

	NET_LOG(TEXT("End"));
}
