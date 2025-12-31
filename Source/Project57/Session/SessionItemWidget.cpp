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
    //Set(SETTING_MAPNAME, FString(TEXT("")), EOnlineDataAdvertisementType::ViaOnlineService);
    //Set(SETTING_NUMBOTS, 0, EOnlineDataAdvertisementType::ViaOnlineService);
    //Set(SETTING_GAMEMODE, FString(TEXT("")), EOnlineDataAdvertisementType::ViaOnlineService);

    Result = InSessionResult;
    Result.OnlineResult.Session.SessionSettings.Set(FName("SessionName"), FString(TEXT("1 vs 1 gosuman"), EOnlineDataAdvertisementType::ViaOnlineService));
    Result.OnlineResult.Session.SessionSettings.Set(FName("MapName"), FString(TEXT("firstMap"), EOnlineDataAdvertisementType::ViaOnlineService));

    //Result.OnlineResult.Session.SessionSettings

    RefreshUI();
}

void USessionItemWidget::RefreshUI()
{
    FString SessionName;
    Result.OnlineResult.Session.SessionSettings.Get(FName("SessionName"), OUT SessionName);
    Txt_SessionName->SetText(FText::FromString(SessionName));
 
    const int32 CurrentPlayers = Result.OnlineResult.Session.SessionSettings.NumPublicConnections - Result.OnlineResult.Session.NumOpenPublicConnections;
    const int32 MaxPlayers = Result.OnlineResult.Session.SessionSettings.NumPublicConnections;
    FString PlayerCount = FString::Printf(TEXT("(%d / %d)"), MaxPlayers, CurrentPlayers);
    Txt_PlayerCount->SetText(FText::FromString(PlayerCount));

    FString MapName;
    Result.OnlineResult.Session.SessionSettings.Get(FName("MapName"), OUT MapName);
    Txt_MapName->SetText(FText::FromString(MapName));
}
