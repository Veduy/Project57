// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacter.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float GroundSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bLeftLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bRightLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float CurrentLeanAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EWeaponState WeaponState = EWeaponState::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsCrouched : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsFalling : 1;
};