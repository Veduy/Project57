// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"



class UButton;
class UEditableTextBox;
class UTextBlock;
class UScrollBox;
class URichTextBlockDecorator;

/**
 * 
 */
UCLASS()
class PROJECT57_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void Start();

	UFUNCTION()
	void ProcessOnCommit(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void ProcessOnChanged(const FText& Text);

	UFUNCTION()
	void UpdateLeftTime(int32 InLeftTime);

	UFUNCTION()
	void UpdateConnectionCount(int32 InCount);

	UFUNCTION()
	void AddMessage(const FText& InMessage);

	UFUNCTION()
	void ShowStartButton();

	UFUNCTION()
	void ShowChatInputBox();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (WidgetBind))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (WidgetBind))
	TObjectPtr<UEditableTextBox> ChatInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (WidgetBind))
	TObjectPtr<UScrollBox> ChatScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (WidgetBind))
	TObjectPtr<UTextBlock> ConnectionCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (WidgetBind))
	TObjectPtr<UTextBlock> LeftTime;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataTable> TextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<URichTextBlockDecorator> ItemDecoratorClass;
};
