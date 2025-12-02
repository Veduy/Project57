// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePC.generated.h"

class UTitleWidget;
/**
 * 
 */
UCLASS()
class PROJECT57_API ATitlePC : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UTitleWidget> TitleWidget;
};
