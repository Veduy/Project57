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
    //RunInfo.Content;
    //RunInfo.MetaData;

	return SNew(STextBlock)
        .Text(RunInfo.Content)
        .Font(DefaultTextStyle.Font)
        .ColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 0.2f))) // 노란 텍스트
        .ToolTip(SNew(SToolTip)
            [
                SNew(STextBlock)
                    .Text(FText::Format(FText::FromString("Item Info: \n ..."), RunInfo.Content))
                    .Font(DefaultTextStyle.Font)
                    .ColorAndOpacity(FSlateColor(FLinearColor::White))
            ]
        );
}
