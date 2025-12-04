// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "LobbyRichTextBlockDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API ULobbyRichTextBlockDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()
	
};

/*
1) RichTextBlockDecorator를 상속한 커스텀 데코레이터 생성
특정 태그(예: <hover text = "설명">아이템< / hover>)를 감지해서
해당 부분을 SWidget으로 만들어주는 방식.
2) SWidget에서 OnMouseEnter / OnMouseLeave 오버라이드
마우스가 올라가면 원하는 툴팁 위젯을 띄우고,
벗어나면 숨기면 됨.
3) 툴팁은 일반 UUserWidget을 생성해서 AddToViewport()로 표시
또는 STooltip을 직접 띄우는 방식도 가능.

✅ 예시 코드 흐름(C++)
1) 데코레이터 클래스 만들기
class UHoverTextDecorator : public URichTextBlockDecorator
{
    GENERATED_BODY()

protected:
    virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
};


2) 실제 Slate 데코레이터 구현
class SHoverTextRun : public SWidget, public ITextDecorator
{
public:
    SLATE_BEGIN_ARGS(SHoverTextRun) {}
        SLATE_ARGUMENT(FString, TooltipText)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs)
    {
        TooltipText = InArgs._TooltipText;
    }

    virtual int32 OnPaint(...) override
    {
        // 텍스트 렌더링
    }

    virtual FReply OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
    {
        ShowTooltipWidget(TooltipText);
        return FReply::Handled();
    }

    virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
    {
        HideTooltipWidget();
    }

private:
    FString TooltipText;
};


3) RichTextBlock에서 태그 등록
RichText->DecoratorClasses.Add(UHoverTextDecorator::StaticClass());


4) 사용 예
<hover tip = "이 아이템은 희귀 아이템입니다.">희귀 아이템< / hover>
*/