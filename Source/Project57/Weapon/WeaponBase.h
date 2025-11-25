// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AProjectileBase;
class USkeletalMeshComponent;

UCLASS()
class PROJECT57_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	FName SocketName = TEXT("");

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 MaxBulletCount = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
	int32 CurBulletCount = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UAnimMontage> ReloadMontage;

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void Fire();
};
