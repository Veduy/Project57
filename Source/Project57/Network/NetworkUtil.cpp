// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkUtil.h"

#include "../Project57.h"

NetworkUtil::NetworkUtil()
{
}

NetworkUtil::~NetworkUtil()
{
}

void NetworkUtil::Log(UWorld* World, const FString& CallInfo, const FString& InString)
{
	if (!World)
	{
		return;
	}

	FString Prefix;

	switch (World->GetNetMode())
	{
	case NM_Client:
		// GPlayInEditorID 0 is always the server, so 1 will be first client.
		// You want to keep this logic in sync with GeneratePIEViewportWindowTitle and UpdatePlayInEditorWorldDebugString
		Prefix = FString::Printf(TEXT("Client %d: "), UE::GetPlayInEditorID());
		break;
	case NM_DedicatedServer:
	case NM_ListenServer:
		Prefix = FString::Printf(TEXT("Server: "));
		break;
	case NM_Standalone:
		break;
	}

	UE_LOG(LogNetworkUtil, Warning, TEXT("%s %s - %s"), *(Prefix), *CallInfo, *InString)
}
