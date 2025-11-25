// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimScaleAddedSignature, float, ValueToAdd);

UCLASS()
class PROJECT57_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void DrawHUD() override;

public:
	FAimScaleAddedSignature OnAimScaleAdded;

	UFUNCTION()
	void ApplyScaleToAim(float NewValue);

	int32 UnitX;
	int32 UnitY;
	int32 CenterX;
	int32 CenterY;
	int32 DrawSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	float ScaleToAdd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	float Scale;
};
