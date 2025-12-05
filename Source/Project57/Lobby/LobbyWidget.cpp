// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "LobbyGM.h"
#include "LobbyGS.h"
#include "LobbyPC.h"
#include "../Title/DataGameInstanceSubsystem.h"
#include "../Network/NetworkUtil.h"

#include "ItemDecorator.h"

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetIsFocusable(true);

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::Start);
		StartButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidget::ProcessOnCommit);
		ChatInput->OnTextChanged.AddDynamic(this, &ULobbyWidget::ProcessOnChanged);
		ChatInput->SetVisibility(ESlateVisibility::Hidden);
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
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		StartButton->SetVisibility(ESlateVisibility::Hidden);
		GM->StartGame();
	}
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

				FString UserID = "<RichText.UserID>" + MySubsystem->UserId + "</>";
				FString Temp = FString::Printf(TEXT("%s: %s"), *UserID, *Text.ToString());

				if (Text.IsEmpty())
				{
					break;
				}
				PC->ServerSendMessage(FText::FromString(Temp));
				ChatInput->SetText(FText());
			}
		}
		break;
	case ETextCommit::OnCleared:
		if (ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer()))
		{
			// 치고나서 사라지게
			ChatInput->SetVisibility(ESlateVisibility::Hidden);		

			SetFocus();
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
		URichTextBlock* NewMessage = NewObject<URichTextBlock>(ChatScrollBox);

		if (NewMessage)
		{
			NewMessage->SetTextStyleSet(TextStyle);
			NewMessage->SetAutoWrapText(true);
			NewMessage->SetWrapTextAt(ChatScrollBox->GetCachedGeometry().GetLocalSize().X);
			NewMessage->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);

			TArray<TSubclassOf<URichTextBlockDecorator>> Decorators;
			Decorators.Add(ItemDecoratorClass);
			NewMessage->SetDecorators(Decorators);

			//NewMessage->SetText(InMessage);
			NewMessage->SetText(FText::FromString("<Item><New Item></> Item Info")); //Decorator Test

			ChatScrollBox->AddChild(NewMessage);
			ChatScrollBox->ScrollToEnd();
		}
	}
}

void ULobbyWidget::ShowStartButton()
{
	if (StartButton)
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget::ShowChatInputBox()
{
	if (ChatInput)
	{
		if (ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer()))
		{
			ChatInput->SetVisibility(ESlateVisibility::Visible);
			ChatInput->SetUserFocus(PC);
		}
	}
}
