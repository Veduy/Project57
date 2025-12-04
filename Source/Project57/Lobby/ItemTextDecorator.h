// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"

/**
 * 
 */
class PROJECT57_API FItemTextDecorator : public FRichTextDecorator
{
public:
	FItemTextDecorator(URichTextBlock* InOwner);

	/** Override this function to specify which types of tags are handled by this decorator */
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override;

	/** Override this function if you want to create a unique widget like an image */
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const override;
};
