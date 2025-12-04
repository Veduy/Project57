// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDecorator.h"
#include "ItemTextDecorator.h"

TSharedPtr<ITextDecorator> UItemDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FItemTextDecorator(InOwner));
}
