// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UTitleWidget::NativeConstruct()
{
	StartServerButton->OnClicked.AddDynamic(this, &UTitleWidget::StartServer);
	ConnectButton->OnClicked.AddDynamic(this, &UTitleWidget::Connect);
}

void UTitleWidget::StartServer()
{
}

void UTitleWidget::Connect()
{
}
