// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindSessionsCallbackProxy.h"
#include "SessionItemWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECT57_API USessionItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetInfo(FBlueprintSessionResult InSessionResult);

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_SessionName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_PlayerCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_MapName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Join;
	
public:
	UPROPERTY(BlueprintReadOnly)
	FBlueprintSessionResult Result;
	
};
