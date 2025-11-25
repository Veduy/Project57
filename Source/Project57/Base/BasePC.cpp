// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePC.h"
#include "BaseHUD.h"

ABasePC::ABasePC()
{
}

void ABasePC::FireAim()
{
	Cast<ABaseHUD>(GetHUD())->OnAimScaleAdded.Broadcast(15);
}
