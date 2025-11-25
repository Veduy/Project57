// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "../Weapon/DamageTypeBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Reload()
{
	UE_LOG(LogTemp, Display, TEXT("Reload Completed"));
	CurBulletCount = MaxBulletCount;
}

void AWeaponBase::Fire()
{
	float CurrentTimeOfShoot = GetWorld()->TimeSeconds - TimeOfLastShot;

	if (CurrentTimeOfShoot < FireRate)
	{
		return;
	}

	if (bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeaponBase::Fire, FireRate, false);
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
	{
		int32 SizeX;
		int32 SizeY;
		PC->GetViewportSize(SizeX, SizeY);
		int32 CenterX = SizeX * 0.5f;
		int32 CenterY = SizeY * 0.5f;

		FVector WorldLocation;
		FVector WorldDirection;

		PC->DeprojectScreenPositionToWorld(CenterX, CenterY, WorldLocation, WorldDirection);

		FVector CameraLocation;
		FRotator CameraRotation;
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector Start = CameraLocation;
		FVector End = CameraLocation + WorldDirection * 100000;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			ObjectTypes,
			true,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			0.5f);

		if (bResult)
		{
			UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 10, -HitResult.ImpactNormal, HitResult, PC, this, UDamageTypeBase::StaticClass());
		}

	}

	if(CurBulletCount > 0)
	{
		CurBulletCount--;

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}

	TimeOfLastShot = GetWorld()->TimeSeconds;
}

void AWeaponBase::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

void AWeaponBase::FireProjectile()
{

}

