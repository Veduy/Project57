// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePC.h"
#include "BaseHUD.h"

ABasePC::ABasePC()
{
}

void ABasePC::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void ABasePC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePC::FireAim()
{
	ABaseHUD* HUD = Cast<ABaseHUD>(GetHUD());
	if (HUD)
	{
		HUD->OnAimScaleAdded.Broadcast(15);
	}
}
