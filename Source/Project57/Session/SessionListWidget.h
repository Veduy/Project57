// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindSessionsCallbackProxy.h"
#include "SessionListWidget.generated.h"


class UWrapBox;
class UButton;
class USessionItemWidget;

/**
 * 
 */

UCLASS()
class PROJECT57_API USessionListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void SetInfo();

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

public:
	UPROPERTY(BlueprintReadWrite, meta= (BindWidget))
	TObjectPtr<UWrapBox> SessionList;

	UPROPERTY(BlueprintReadWrite, meta= (BindWidget))
	TObjectPtr<UButton> Btn_Refresh;
	
	UPROPERTY(BlueprintReadWrite, meta= (BindWidget))
	TObjectPtr<UButton> Btn_CreateSession;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Close;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<USessionItemWidget> SessionItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<USessionItemWidget>> Sessions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FBlueprintSessionResult> SessionInfos;
	
};
