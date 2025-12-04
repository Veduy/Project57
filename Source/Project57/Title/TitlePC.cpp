// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"

#include "TitleWidget.h"

void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	FSoftClassPath TitleWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyProject/Blueprints/Title/WBP_Title.WBP_Title_C'"));
	UClass* WidgetClass = TitleWidgetClass.TryLoadClass<UTitleWidget>();

	if (WidgetClass && IsLocalPlayerController())
	{
		TitleWidget = CreateWidget<UTitleWidget>(this, WidgetClass);
		if (TitleWidget)
		{
			TitleWidget->AddToViewport();
		}

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}
