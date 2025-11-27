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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void Reload();

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void FireProjectile(FTransform SpawnTrasnform, FHitResult InHitResult);

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

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MaxBulletCount = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 CurBulletCount = 100;

	// 초당 발사율 >> (UI표시할때 분당 발사율로 변환)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	float FireRate = 1.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data", meta = (ClampMin = 0.1f, ClampMax = 2.f, Unit = "s"))
	uint8 bFullAuto : 1 = false;

	float TimeOfLastShot;
	
	FTimerHandle FireTimer;
};
