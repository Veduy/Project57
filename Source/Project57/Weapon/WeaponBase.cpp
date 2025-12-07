// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "../Base/BaseCharacter.h"
#include "../Base/BasePC.h"
#include "../Weapon/DamageTypeBase.h"
#include "../Weapon/ProjectileBase.h"

#include "../Network/NetworkUtil.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	bNetLoadOnClient = true;
	SetReplicateMovement(true);

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

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, CurBulletCount);
	DOREPLIFETIME(AWeaponBase, MaxBulletCount);
	DOREPLIFETIME(AWeaponBase, FireRate);
	DOREPLIFETIME(AWeaponBase, bFullAuto);
	DOREPLIFETIME(AWeaponBase, TimeOfLastShot);
}

void AWeaponBase::Reload()
{
	ServerReload();
}

void AWeaponBase::ServerReload_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("Reload Completed"));
	CurBulletCount = MaxBulletCount;
}

void AWeaponBase::Fire()
{
	if (CurBulletCount < 1)
	{
		return;
	}

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	// HitLocation만 어차피 쓸거라 다른건 필요 없을듯.
	FVector AimOrigin;
	FVector AimDirection;
	FVector HitLocation;
	if (GetAimData(AimOrigin, AimDirection, HitLocation))
	{
		ServerStartFire(HitLocation);
	}

	if (bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeaponBase::Fire, FireRate, false);
		TempPlayFireSound();
	}
}

void AWeaponBase::TempPlayFireSound()
{
	if (FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
}

void AWeaponBase::ServerStartFire_Implementation(const FVector& HitLocation)
{
	if (CurBulletCount < 1)
	{
		return;
	}

	float CurrentTimeOfShoot = GetWorld()->TimeSeconds - TimeOfLastShot;

	if (CurrentTimeOfShoot < FireRate)
	{
		return;
	}

	FVector MuzzleLocation = Mesh->GetSocketLocation("Muzzle");
	FHitResult Hit;

	FVector ServerTraceEnd = HitLocation;

	// 서버에서 한번더 Trace check
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		MuzzleLocation,
		ServerTraceEnd,
		ObjectTypes,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red, 
		FLinearColor::Green,
		0.5f);

	FVector FinalTarget = HitResult.bBlockingHit ? HitResult.ImpactPoint : ServerTraceEnd;
	FVector Dir = (FinalTarget - MuzzleLocation).GetSafeNormal();
	
	FTransform SpawnTransform = FTransform(Dir.Rotation(),MuzzleLocation,FVector::OneVector);
	SpawnProjectile(SpawnTransform);

	CurBulletCount--;

	MulticastSpawnMuzzleFlash(MuzzleLocation, Dir.Rotation());
	MulticastPlayFireSound(MuzzleLocation);

	//// Recoil
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());

	ABasePC* PC = Cast<ABasePC>(Character->GetController());
	if (PC)
	{
		PC->AddPitchInput(FMath::FRandRange(-0.5f, 0.f));
		PC->AddYawInput(FMath::FRandRange(-0.5f, 0.5f));
		PC->FireAim();
	}

	TimeOfLastShot = GetWorld()->TimeSeconds;
}

void AWeaponBase::StopFire()
{
	if (FireTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	ServerStopFire();
}

void AWeaponBase::ServerStopFire_Implementation()
{
	if (FireTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void AWeaponBase::SpawnProjectile(const FTransform& SpawnTrasnform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTrasnform, SpawnParams);

	Projectile->SetOwner(this);
}

void AWeaponBase::MulticastSpawnMuzzleFlash_Implementation(const FVector& SpawnLocation, const FRotator& AimRotation)
{
	if(MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash,
			SpawnLocation, AimRotation, FVector(3.f,3.f,3.f));
	}
}

void AWeaponBase::MulticastPlayFireSound_Implementation(const FVector& SpawnLocation)
{
	if (HasAuthority())
	{
		return;
	}

	if (FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, SpawnLocation);
	}
}

bool AWeaponBase::GetAimData(FVector& OutAimLocation, FVector& OutAimDirection, FVector& OutHitLocation)
{
	ABasePC* PC = Cast<ABasePC>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PC)
	{
		return false;
	}

	int32 SizeX;
	int32 SizeY;
	PC->GetViewportSize(SizeX, SizeY);
	int32 CenterX = SizeX * 0.5f;
	int32 CenterY = SizeY * 0.5f;

	FVector WorldLocation;
	FVector WorldDirection;
	PC->DeprojectScreenPositionToWorld(CenterX, CenterY, WorldLocation, WorldDirection);

	OutAimLocation = WorldLocation;
	OutAimDirection = WorldDirection;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + OutAimDirection * 1000.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		ObjectTypes,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		0.5f);

	OutHitLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

	return true;
}