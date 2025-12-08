// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePC.h"
#include "BaseHUD.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"

ABasePC::ABasePC()
{
}

void ABasePC::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMapping)
			{
				InputSystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	if (IsLocalPlayerController() && MainHUDWidgetClass)
	{
		MainHUDWidget = CreateWidget(this, MainHUDWidgetClass, TEXT("MainHUD"));
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		}
	}
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
