// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"

#include "InGameGS.h"
#include "../Network/NetworkUtil.h"

void AInGameGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NET_LOG("");

	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		int32 PlayerCount = 0;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (IsValid(Iterator->Get()))
			{
				PlayerCount++;
			}
		}

		GS->RemainingPlayerCount = PlayerCount;
		GS->OnRep_RemainingPlayerCount();
	}
}
