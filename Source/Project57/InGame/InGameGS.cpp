// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGS.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

#include "../Base/BasePC.h"

AInGameGS::AInGameGS()
{

}

void AInGameGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameGS, RemainingPlayerCount);
}

void AInGameGS::OnRep_RemainingPlayerCount()
{
	// 클라이언트의 UI 숫자 업데이트 여기서
	ABasePC* PC = Cast<ABasePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->OnRemainingPlayerUpdated.Broadcast(RemainingPlayerCount);
	}
}
