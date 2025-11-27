// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Zombie.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AZombie* Character = Cast<AZombie>(TryGetPawnOwner()))
	{
		Speed = Character->GetCharacterMovement()->Velocity.Length();

		State = Character->State;
	}
}
