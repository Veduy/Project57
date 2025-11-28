// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Zombie_AIC.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 50.f;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHP <= 0)
	{
		return DamageAmount;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
			SpawnHitEffect(Event->HitInfo);
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage %f"), DamageAmount);
	}

	if (CurrentHP <= 0)
	{
		DoDeath();
	}


	return DamageAmount;
}

void AZombie::DoDeath()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage, 1.f);
		SetState(EAIState::Death);

		if(AZombie_AIC* AIC = Cast<AZombie_AIC>(GetController()))
		{
			AIC->SetState(EAIState::Death);
		}
	};	
}

void AZombie::SpawnHitEffect(const FHitResult& Hit)
{
	if (BloodEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Point: %f, %f, %f"), Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z);

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			FVector(3.f, 3.f, 3.f));
	}
}

void AZombie::SetState(EAIState NewState)
{
	State = NewState;
}

void AZombie::DoAttack()
{
	
}

void AZombie::ChangeSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}
