// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "BaseCharacter.generated.h"


class UInputAction;
class AInteractActor;
class UAIPerceptionStimuliSourceComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unarmed = 0 UMETA(DisplayName = "Unarmed"),
	Pistol = 10 UMETA(DisplayName = "Pistol"),
	Rifle = 20 UMETA(DisplayName = "Rifle"),
	Launcher = 30 UMETA(DisplayName = "Launcher")
};

UCLASS()
class PROJECT57_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	virtual void SpawnHitEffect(const FHitResult& Hit);

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	UFUNCTION()
	FRotator GetAimOffset() const;

public:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UChildActorComponent> Weapon;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> PerceptionSource;

public:
	UFUNCTION(BlueprintCallable)
	void Move(float Forward, float Right);

	UFUNCTION(BlueprintCallable)
	void Look(float Pitch, float Yaw);

	UFUNCTION(BlueprintCallable)
	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();
		
	UFUNCTION(BlueprintCallable)
	void DoFire();

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();

	UFUNCTION(Server, Reliable)
	void ServerStartFire();
	void ServerStartFire_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();
	void ServerStopFire_Implementation();

	UFUNCTION(BlueprintCallable)
	void StartIronSight(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void StopIronSight(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DoDeath();

	UFUNCTION(BlueprintCallable)
	void DoDeathEnd();

	UFUNCTION()
	void DoHitReact();

	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void StopSprint();

	UFUNCTION()
	void LeftLean(const FInputActionValue& Value);

	UFUNCTION()
	void EndLeftLean();

	UFUNCTION()
	void RightLean(const FInputActionValue& Value);

	UFUNCTION()
	void EndRightLean();

	void EquipItem(AInteractActor* PickedupItem);
	void UseItem(AInteractActor* PickedupItem);
	void EatItem(AInteractActor* PickedupItem);

	UFUNCTION(Server, Reliable)
	void ServerStopSprint();
	void ServerStopSprint_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerStartSprint();
	void ServerStartSprint_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerStartIronSight();
	void ServerStartIronSight_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerStopIronSight();
	void ServerStopIronSight_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerLeftLean();
	void ServerLeftLean_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerEndLeftLean();
	void ServerEndLeftLean_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerRightLean();
	void ServerRightLean_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerEndRightLean();
	void ServerEndRightLean_Implementation();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> IA_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> IA_Fire;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_IronSight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_LeftLean;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_RightLean;

public:

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	float CurrentHP = 100;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	float MaxHP = 100;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bSprint : 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bLeftLean : 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bRightLean : 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bAiming : 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsFire : 1 = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsIronSight : 1 = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	EWeaponState WeaponState = EWeaponState::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UParticleSystem> BloodEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
	FGenericTeamId TeamID;

private:
	FName HitMonatageSection[8] =
	{
		"Back_Med_01",
		"Front_Hvy_01",
		"Front_Lgt_01",
		"Front_Lgt_02",
		"Front_Lgt_03",
		"Front_Lgt_04",
		"Front_Med_01",
		"Front_Med_02"
	};

	FName DeathMonatageSection[6] =
	{
		"Back_01",
		"Front_01",	
		"Front_02",
		"Front_03",
		"Left_01"
		"Right_01"
	};
};
