// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTextDecorator.h"

#include "Components/RichTextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"

FItemTextDecorator::FItemTextDecorator(URichTextBlock* InOwner)
	:FRichTextDecorator(InOwner)
{
}

bool FItemTextDecorator::Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const
{
    return RunParseResult.Name == TEXT("Item");
}

TSharedPtr<SWidget> FItemTextDecorator::CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const
{
    return SNew(STextBlock)
        .Text(RunInfo.Content)
        .Font(DefaultTextStyle.Font)
        .ColorAndOpacity(FSlateColor(FLinearColor(0.7f, 0.0f, 0.7f, 1.0f))) //º¸¶ó»ö
        .ToolTip(SNew(SToolTip)
            [
                SNew(STextBlock)
                    .Text(FText::Format(FText::FromString("Item Info: \n\n\n\n\n ..."), RunInfo.Content))
                    .Font(DefaultTextStyle.Font)
                    .ColorAndOpacity(FSlateColor(FLinearColor::White))
            ]
        );
}
