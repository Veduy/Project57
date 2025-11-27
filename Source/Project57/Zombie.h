// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Normal		= 0 UMETA(DisplayName = "Normal"),
	Chase		= 1 UMETA(DisplayName = "Chase"),
	Battle		= 2 UMETA(DisplayName = "Battle"),
	Death		= 3 UMETA(DisplayName = "Death")
};

class UParticleSystem;

UCLASS()
class PROJECT57_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AZombie();

protected:
	virtual void BeginPlay() override;

public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION()
	void DoDeath();

	UFUNCTION()
	void SpawnHitEffect(const FHitResult& Hit);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAIState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UParticleSystem> BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP = 100;
};
