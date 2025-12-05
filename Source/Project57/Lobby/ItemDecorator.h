// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "ItemDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API UItemDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UUserWidget> ItemTooltipClass;
};