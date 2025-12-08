// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"

#include "InGameGS.h"
#include "../Network/NetworkUtil.h"
#include "../Base/BasePC.h"

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

void AInGameGM::CheckLastPlayerLeft()
{
	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		if (GS->RemainingPlayerCount <= 1)
		{
			for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
			{
				ABasePC* PC = Cast<ABasePC>(*Iter);
				if (PC)
				{
					//PC->ClientShowLoadingScreen();
				}
			}

			GetWorld()->ServerTravel(TEXT("Lobby"));
		}
	}
}
