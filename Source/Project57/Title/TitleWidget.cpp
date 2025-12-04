// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "DataGameInstanceSubsystem.h"
#include "../Network/NetworkUtil.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//meta=(WidgetBind)로 대체 가능
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));

	StartServerButton->OnClicked.AddDynamic(this, &UTitleWidget::StartServer);
	ConnectButton->OnClicked.AddDynamic(this, &UTitleWidget::Connect);
}

void UTitleWidget::StartServer()
{
	SaveData();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"), true, TEXT("listen"));
}

void UTitleWidget::Connect()
{	
	SaveData();
	UGameplayStatics::OpenLevel(GetWorld(), FName(ServerIP->GetText().ToString()), true, TEXT("Option"));
}

void UTitleWidget::SaveData()
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (GI)
	{
		UDataGameInstanceSubsystem* MySubsystem = GI->GetSubsystem<UDataGameInstanceSubsystem>();
		MySubsystem->UserId = UserID->GetText().ToString();
		UE_LOG(LogTemp, Warning, TEXT("UserID: %s"), *UserID->GetText().ToString());

		MySubsystem->Password = Password->GetText().ToString();
		UE_LOG(LogTemp, Warning, TEXT("UserID: %s"), *Password->GetText().ToString());
	}
}
