// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"

#include "InGameGS.h"
#include "../Network/NetworkUtil.h"
#include "../Base/BasePC.h"

void AInGameGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// TODO: Controller가 조종중인 Pawn을 가져와서 CurHP > 0 만을 조건으로 변경하기.

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

void AInGameGM::Logout(AController* Exiting)
{
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

	Super::Logout(Exiting);
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
			
			FTimerHandle LodingTimer;
			GetWorld()->GetTimerManager().SetTimer(LodingTimer, FTimerDelegate::CreateLambda([this]()
				{
					GetWorld()->ServerTravel(TEXT("Lobby")); 
				}),
				5.f,
				false,
				5.f);
		}
	}
}
