// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "../ItemBase.h"
#include "WeaponBase.generated.h"


class AProjectileBase;
class USkeletalMeshComponent;



UCLASS()
class PROJECT57_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

public:	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerReload();
	void ServerReload_Implementation();

	// BaseCharacter에서 발사 입력이 들어왔을때, 호출될 함수.
	UFUNCTION()
	void Fire();

	UFUNCTION(Server, Reliable)
	void ServerFire();
	void ServerFire_Implementation();

	UFUNCTION()
	void TempPlayFireSound();

	UFUNCTION()
	void StopFire();

	UFUNCTION(Server, Reliable)
	void ServerStartFire(const FVector& HitLocation);
	void ServerStartFire_Implementation(const FVector& HitLocation);

	UFUNCTION(Server, Reliable)
	void ServerStopFire();
	void ServerStopFire_Implementation();

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FTransform& SpawnTrasnform);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnMuzzleFlash(const FVector& SpawnLocation, const FRotator& AimRotation);
	void MulticastSpawnMuzzleFlash_Implementation(const FVector& SpawnLocation, const FRotator& AimRotation);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayFireSound(const FVector& SpawnLocation);
	void MulticastPlayFireSound_Implementation(const FVector& SpawnLocation);

	UFUNCTION()
	bool GetAimData(FVector& OutAimLocation, FVector& OutAimDirection, FVector& OutHitLocation);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FName Name;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FName SocketName = TEXT("");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UParticleSystem> MuzzleFlash;   

public:
	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MaxBulletCount = 100;

	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 CurBulletCount = 100;

	// 초당 발사율 >> (UI표시할때 분당 발사율로 변환)
	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float FireRate = 1.f;

	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "Data", meta = (ClampMin = 0.1f, ClampMax = 2.f, Unit = "s"))
	uint8 bFullAuto : 1 = false;

	UPROPERTY(Replicated)
	float TimeOfLastShot;
	
	FTimerHandle FireTimer;
};
