// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "LobbyGS.h"
#include "LobbyPC.h"
#include "../Title/DataGameInstanceSubsystem.h"
#include "../Network/NetworkUtil.h"
//#include "Textb"

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::Start);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidget::ProcessOnCommit);
		ChatInput->OnTextChanged.AddDynamic(this, &ULobbyWidget::ProcessOnChanged);
	}

	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->LeftTimeChanged.AddDynamic(this, &ULobbyWidget::UpdateLeftTime);
		GS->ConnectionChanged.AddDynamic(this, &ULobbyWidget::UpdateConnectionCount);

		GS->ConnectionChanged.Broadcast(GS->ConnectionCount);
	}
}

void ULobbyWidget::Start()
{
	GetWorld()->ServerTravel(TEXT("InGame"));
}

void ULobbyWidget::ProcessOnCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
		NET_LOG("Pressed Enter");
		if (ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer()))
		{
			UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
			if (GI)
			{
				UDataGameInstanceSubsystem* MySubsystem = GI->GetSubsystem<UDataGameInstanceSubsystem>();

				FString Temp = FString::Printf(TEXT("%s:%s"), *MySubsystem->UserId, *Text.ToString());

				PC->ServerSendMessage(FText::FromString(Temp));

				ChatInput->SetText(FText());
			}
		}


		break;
	case ETextCommit::OnCleared:
		if (ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer()))
		{
			ChatInput->SetUserFocus(PC);
		}
		break;
	default:
		break;
	}
}

void ULobbyWidget::ProcessOnChanged(const FText& Text)
{
}

void ULobbyWidget::UpdateLeftTime(int32 InLeftTime)
{
	if (LeftTime)
	{
		FString Message = FString::Printf(TEXT("%d 초 남음"), InLeftTime);
		LeftTime->SetText(FText::FromString(Message));
	}
}

void ULobbyWidget::UpdateConnectionCount(int32 InCount)
{
	if (ConnectionCount)
	{
		FString Message = FString::Printf(TEXT("%d 플레이어 대기중"), InCount);
		ConnectionCount->SetText(FText::FromString(Message));
	}
}

void ULobbyWidget::AddMessage(const FText& InMessage)
{
	if (ChatScrollBox)
	{
		//UTextBlock* NewMessage = NewObject<UTextBlock>(ChatScrollBox);
		URichTextBlock* NewMessage = NewObject<URichTextBlock>(ChatScrollBox);

		if (NewMessage)
		{
			NewMessage->SetText(InMessage);

			NewMessage->SetTextStyleSet(TextStyle);
			NewMessage->SetAutoWrapText(true);
			NewMessage->SetWrapTextAt(ChatScrollBox->GetCachedWidget().Get()->GetDesiredSize().X);
			NewMessage->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);

			ChatScrollBox->AddChild(NewMessage);

			// 밑으로 스크롤 해줌.
			ChatScrollBox->ScrollToEnd();
		}
	}
}
