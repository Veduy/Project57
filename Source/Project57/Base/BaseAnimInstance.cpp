// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "BaseCharacter.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABaseCharacter* Character = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (Character)
	{
		GroundSpeed = Character->GetCharacterMovement()->Velocity.Size2D();
		bSprint = Character->bSprint;
		Direction = UKismetAnimationLibrary::CalculateDirection(Character->GetCharacterMovement()->Velocity, Character->GetActorRotation());
		AimYaw = Character->GetBaseAimRotation().Yaw;
		AimPitch = Character->GetBaseAimRotation().Pitch;
		bAiming = Character->bAiming;
		bLeftLean = Character->bLeftLean;
		bRightLean = Character->bRightLean;

		float TargetLeanAngle = 0;
		if (bLeftLean)
		{
			TargetLeanAngle = -30.f;
		}
		else if (bRightLean)
		{
			TargetLeanAngle = 30.f;
		}
		else
		{
			TargetLeanAngle = 0;
		}

		CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, TargetLeanAngle, DeltaSeconds, 8.0f);

		WeaponState = Character->WeaponState;

		bIsCrouched = Character->bIsCrouched;

		bIsFalling = Character->GetMovementComponent()->IsFalling();
	}
}