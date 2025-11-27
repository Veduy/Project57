// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
	TargetPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Direction = TargetPawn->GetActorLocation() - GetActorLocation();
	AddMovementInput(Direction, 1);

	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPawn->GetActorLocation());
	SetActorRotation(Rotation);
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
	};	
}

void AZombie::SpawnHitEffect(const FHitResult& Hit)
{
	if (BloodEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			FVector(3.f, 3.f, 3.f));
	}
}

