// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

class UButton;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class PROJECT57_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


public:
	UFUNCTION()
	void StartServer();

	UFUNCTION()
	void Connect();

	UFUNCTION()
	void SaveData();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta=(WidgetBind))
	TObjectPtr<UButton> StartServerButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta=(WidgetBind))
	TObjectPtr<UButton> ConnectButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta=(WidgetBind))
	TObjectPtr<UEditableTextBox> UserID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (WidgetBind))
	TObjectPtr<UEditableTextBox> Password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<UEditableTextBox> ServerIP;


};
