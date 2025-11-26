// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePCM.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"


ABasePCM::ABasePCM()
{

}

void ABasePCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	if (ABaseCharacter* Pawn = Cast<ABaseCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		float TargetFOV = Pawn->bIsIronSight ? IronSightFOV : DefaultFOV;
		float CurrentFOV = FMath::FInterpTo(GetFOVAngle(), TargetFOV, GetWorld()->GetDeltaSeconds(), ZoomSpeed);
		SetFOV(CurrentFOV);
	}
}
