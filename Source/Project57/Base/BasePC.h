// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRecoilSignature, float, Vertical, float, Horizontal);

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

public:
	UFUNCTION()
	void FireAim();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")	
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	float VerticalRecoil;
	float HorizontalRecoil;
};
