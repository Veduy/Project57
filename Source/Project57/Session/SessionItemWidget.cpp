// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool USessionItemWidget::Initialize()
{
    Super::Initialize();

    return false;
}

void USessionItemWidget::SetInfo(FBlueprintSessionResult InSessionResult)
{
    Result = InSessionResult;
    RefreshUI();
}

void USessionItemWidget::RefreshUI()
{
    Txt_SessionName->SetText(FText());
    Txt_PlayerCount->SetText(FText());
    Txt_MapName->SetText(FText());



}
