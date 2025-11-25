// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	OnAimScaleAdded.AddDynamic(this, &ABaseHUD::ApplyScaleToAim);
}

void ABaseHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Scale > 0)
	{
		Scale = UKismetMathLibrary::FInterpTo(Scale, 0, UGameplayStatics::GetWorldDeltaSeconds(this), 5);
		ScaleToAdd = UKismetMathLibrary::FInterpTo(ScaleToAdd, 0, UGameplayStatics::GetWorldDeltaSeconds(this), 5);
	}
}

void ABaseHUD::DrawHUD()
{
	Super::DrawHUD();

	float CurrentSpeed = 0.f;
	float MaxSpeed = 0.f;
	float GapRatio = 0.f;
	int32 GapX = UnitX * 2;
	int32 GapY = UnitY;


	UnitX = Canvas->SizeX * 0.5f / 100;
	UnitY = Canvas->SizeY * 0.5f / 100;
	CenterX = Canvas->SizeX / 2;
	CenterY = Canvas->SizeY / 2;
	DrawSize = 2;


	ACharacter* Pawn = Cast<ACharacter>(GetOwningPawn());
	if (Pawn)
	{
		MaxSpeed = Pawn->GetCharacterMovement()->GetMaxSpeed();
		CurrentSpeed = Pawn->GetCharacterMovement()->Velocity.Size();
		GapRatio = CurrentSpeed / MaxSpeed;
	}
	GapX = (int32)((float)GapX * GapRatio);
	GapY = (int32)((float)GapY * GapRatio);

	// ╩С
	Draw2DLine(CenterX, CenterY - (UnitX * DrawSize) - Scale - GapX,
		CenterX, CenterY - Scale - GapX, FColor::White);

	// го
	Draw2DLine(CenterX, CenterY + (UnitX * DrawSize) + Scale + GapX,
		CenterX, CenterY + Scale + GapX, FColor::White);

	// аб
	Draw2DLine(CenterX - (UnitX * DrawSize) - Scale - GapX, CenterY,
		CenterX - Scale - GapX, CenterY, FColor::White);

	// ©Л
	Draw2DLine(CenterX + Scale + GapX, CenterY,
		CenterX + (UnitX * DrawSize) + Scale + GapX, CenterY, FColor::White);
}

void ABaseHUD::ApplyScaleToAim(float NewValue)
{
	ScaleToAdd += NewValue;
	float InterpValue = UKismetMathLibrary::FInterpTo(Scale, Scale+ScaleToAdd, UGameplayStatics::GetWorldDeltaSeconds(this),
		50);

	Scale = UKismetMathLibrary::Clamp(InterpValue, 0, 300);
}

