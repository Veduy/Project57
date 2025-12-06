// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

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
	SetReplicates(true);
	SetReplicateMovement(true);
	bNetUseOwnerRelevancy = true;
	bNetLoadOnClient = true;

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

void AWeaponBase::StartFire(const FVector& HitLocation)
{
	PendingHitLocation = HitLocation;
	Fire();
}

void AWeaponBase::Fire()
{
	// 서버에서 실행될 로직
	NET_LOG("");

	if (CurBulletCount < 1)
	{
		return;
	}

	float CurrentTimeOfShoot = GetWorld()->TimeSeconds - TimeOfLastShot;

	if (CurrentTimeOfShoot < FireRate)
	{
		return;
	}

	if (bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeaponBase::Fire, FireRate, false);
	}

	FVector MuzzleLocation = Mesh->GetSocketLocation("Muzzle");
	FHitResult Hit;

	FVector ServerTraceEnd = PendingHitLocation;

	// 서버에서 보정
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
	
	FTransform SpawnTransform = FTransform(Dir.Rotation(), MuzzleLocation, FVector::OneVector);
	SpawnProjectile(SpawnTransform);

	//FVector SpawnLocation;
	//FVector TargetLocation;
	//FVector BulletDirection;
	//FRotator AimRotation;
	//FHitResult HitResult;

	//CalculateShootData(SpawnLocation, TargetLocation, BulletDirection, AimRotation);

	//SpawnProjectile(FTransform(AimRotation, SpawnLocation, FVector::OneVector));

	//CurBulletCount--;
	//MulticastSpawnMuzzleFlash(SpawnLocation, AimRotation);
	//MulticastPlayFireSound(SpawnLocation);

	//// Recoil
	//Character->AddControllerPitchInput(FMath::FRandRange(-0.5, 0));
	//Character->AddControllerYawInput(FMath::FRandRange(-0.5, 0.5));

	//ABasePC* PC = Cast<ABasePC>(Character->GetController());
	//{
	//	PC->FireAim();
	//}

	TimeOfLastShot = GetWorld()->TimeSeconds;
}

void AWeaponBase::StopFire()
{
	if (FireTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
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

bool AWeaponBase::CalculateShootData(FVector& OutSpawnLocation, FVector& OutTargetLocation, FVector& OutBulletDirection, FRotator& OutAimRotation)
{
	NET_LOG("");

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return false;
	}

	if (ABasePC* PC = Cast<ABasePC>(Character->GetController()))
	{
		FString PCNum = PC->GetName();
		NET_LOG(*PCNum);

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
		FVector End = CameraLocation + WorldDirection * 1000;

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

		OutSpawnLocation = Mesh->GetSocketLocation(FName("Muzzle"));
		OutTargetLocation = bResult ? HitResult.ImpactPoint : End;
		OutBulletDirection = OutTargetLocation - OutSpawnLocation;
		OutAimRotation = UKismetMathLibrary::FindLookAtRotation(OutSpawnLocation, OutTargetLocation + (UKismetMathLibrary::RandomUnitVector() * 0.3f));

		return true;
	}

	return false;
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