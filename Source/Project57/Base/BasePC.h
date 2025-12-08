// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRecoilSignature, float, Vertical, float, Horizontal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthUpdateSignature, float, HealthValue);

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PROJECT57_API ABasePC : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePC();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	FRecoilSignature OnRecoil;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FHealthUpdateSignature OnHealthUpdated;

public:
	UFUNCTION()
	void FireAim();


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")	
	TObjectPtr<UInputMappingContext> InputMapping;

	float VerticalRecoil;
	float HorizontalRecoil;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainHUDWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> MainHUDWidget;
};
