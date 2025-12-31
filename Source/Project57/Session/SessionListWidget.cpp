// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "SessionItemWidget.h"
#include "../Network/NetworkUtil.h"

bool USessionListWidget::Initialize()
{
	Super::Initialize();

	SetInfo();

	return true;
}

void USessionListWidget::SetInfo()
{
	Sessions.Empty();

	for (int i = 0; i < 10; ++i)
	{
		if (!SessionItemClass)
			continue;

		USessionItemWidget* ChildWidget = CreateWidget<USessionItemWidget>(GetWorld(), SessionItemClass, FName());
		if (!ChildWidget)
			continue;

		SessionList->AddChildToWrapBox(ChildWidget);

		Sessions.Add(ChildWidget);		
	}

	RefreshUI();
}

void USessionListWidget::RefreshUI()
{
	// 실제 접속자 몇명인지 확인
	const int32 SessionLength = SessionInfos.Num();
	for (int32 i = 0; i < 10; i++)
	{
		const int32 Index = i;

		if (Index < SessionLength)
		{
			Sessions[Index]->SetVisibility(ESlateVisibility::Visible);
			FBlueprintSessionResult Result = SessionInfos[Index];
			Sessions[Index]->SetInfo(Result);
		}
		else
		{
			Sessions[Index]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
