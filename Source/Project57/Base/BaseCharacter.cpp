// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameframeWork/SpringArmComponent.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


#include "EnhancedInputComponent.h"
#include "Engine/DamageEvents.h"

#include "TimerManager.h"

#include "../Weapon/WeaponBase.h"

#include "BasePC.h"
#include "../InteractActor.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponActor"));
	Weapon->SetupAttachment(GetMesh());

	PerceptionSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionSource"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ABaseCharacter::ActorBeginOverlap);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CurrentHP);
	DOREPLIFETIME(ABaseCharacter, MaxHP);
	DOREPLIFETIME(ABaseCharacter, bSprint);
	DOREPLIFETIME(ABaseCharacter, bLeftLean);
	DOREPLIFETIME(ABaseCharacter, bRightLean);
	DOREPLIFETIME(ABaseCharacter, bIsFire);
	DOREPLIFETIME(ABaseCharacter, bIsIronSight);
	DOREPLIFETIME(ABaseCharacter, WeaponState);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_Reload, ETriggerEvent::Triggered, this, &ABaseCharacter::Reload);
		Input->BindAction(IA_Fire, ETriggerEvent::Started, this, &ABaseCharacter::StartFire);
		Input->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ABaseCharacter::StopFire);

		Input->BindAction(IA_IronSight, ETriggerEvent::Started, this, &ABaseCharacter::StartIronSight);
		Input->BindAction(IA_IronSight, ETriggerEvent::Completed, this, &ABaseCharacter::StopIronSight);

		Input->BindAction(IA_Sprint, ETriggerEvent::Started, this, &ABaseCharacter::StartSprint);
		Input->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);

		Input->BindAction(IA_LeftLean, ETriggerEvent::Started, this, &ABaseCharacter::LeftLean);
		Input->BindAction(IA_LeftLean, ETriggerEvent::Completed, this, &ABaseCharacter::EndLeftLean);

		Input->BindAction(IA_RightLean, ETriggerEvent::Started, this, &ABaseCharacter::RightLean);
		Input->BindAction(IA_RightLean, ETriggerEvent::Completed, this, &ABaseCharacter::EndRightLean);
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHP <= 0)
	{
		return DamageAmount;
	}

	DoHitReact();

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

void ABaseCharacter::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ABasePC* PC = Cast<ABasePC>(GetController());
	if (!PC || !PC->HasAuthority())
	{
		return;
	}

	if (AInteractActor* Item = Cast<AInteractActor>(OtherActor))
	{
		switch (Item->Info.ItemType)
		{
		case EItemType::None:
		case EItemType::Use:
			UseItem(Item);
			break;
		case EItemType::Eat:
			EatItem(Item);
			break;
		case EItemType::Equip:
			EquipItem(Item);
			break;
		}

		Item->SetOwner(PC);

		Item->Destroy();
	}
}

void ABaseCharacter::SpawnHitEffect(const FHitResult& Hit)
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

void ABaseCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;
}

FGenericTeamId ABaseCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void ABaseCharacter::Move(float Forward, float Right)
{
	const FRotator CameraRotation = GetController()->GetControlRotation();
	const FRotator YawRotation = FRotator(0, CameraRotation.Yaw, 0);
	const FRotator YawRollRotation = FRotator(0, CameraRotation.Yaw, CameraRotation.Roll);

	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(YawRotation);
	AddMovementInput(ForwardVector, Forward);

	const FVector RightVector = UKismetMathLibrary::GetRightVector(YawRollRotation);
	AddMovementInput(RightVector, Right);
}

void ABaseCharacter::Look(float Pitch, float Yaw)
{
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
}

void ABaseCharacter::Reload()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		PlayAnimMontage(ChildWeapon->ReloadMontage);
	}
}

void ABaseCharacter::ReloadWeapon()
{
	if (AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor()))
	{
		ChildWeapon->Reload();
	}
}

void ABaseCharacter::DoFire(const FVector& ClientHitLocation)
{
	// 서버에서 실행
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->StartFire(ClientHitLocation);
	}
}

void ABaseCharacter::StartFire()
{
	bIsFire = true;

	FVector AimOrigin;
	FVector AimDirection;
	FVector HitLocation;

	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		if (ChildWeapon->GetAimData(AimOrigin, AimDirection, HitLocation))
		{
			ServerStartFire(HitLocation);
		}
	}
}

void ABaseCharacter::StopFire()
{
	bIsFire = false;
	ServerStopFire();
}

void ABaseCharacter::ServerStartFire_Implementation(const FVector& ClientHitLocation)
{
	bIsFire = true;
	DoFire(ClientHitLocation);
}

void ABaseCharacter::ServerStopFire_Implementation()
{
	bIsFire = false;
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->StopFire();
	}
}

void ABaseCharacter::StartIronSight(const FInputActionValue& Value)
{
	bIsIronSight = true;
	ServerStartIronSight();
}

void ABaseCharacter::StopIronSight(const FInputActionValue& Value)
{
	bIsIronSight = false;
	ServerStopIronSight();
}

void ABaseCharacter::DoDeath()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage, 1.f, DeathMonatageSection[FMath::RandRange(0, 5)]);
	};
}

void ABaseCharacter::DoDeathEnd()
{
	GetController()->SetActorEnableCollision(false);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"), true);
}

void ABaseCharacter::DoHitReact()
{
	if (HitMontage)
	{
		PlayAnimMontage(HitMontage, 1.f, HitMonatageSection[FMath::RandRange(0, 7)]);
	}
}

FRotator ABaseCharacter::GetAimOffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();
	
	return AimRotLS;
}

void ABaseCharacter::EquipItem(AInteractActor* PickedupItem)
{
	Weapon->SetChildActorClass(PickedupItem->Info.ItemTemplate);
	WeaponState = PickedupItem->Info.WeaponType;

	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ChildWeapon->SocketName);
		ChildWeapon->SetOwner(this);
	}
}

void ABaseCharacter::UseItem(AInteractActor* PickedupItem)
{
}

void ABaseCharacter::EatItem(AInteractActor* PickedupItem)
{
}

void ABaseCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	ServerStartSprint();
}

void ABaseCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	ServerStopSprint();
}

void ABaseCharacter::LeftLean(const FInputActionValue& Value)
{
	bLeftLean = true;
	ServerLeftLean();
}

void ABaseCharacter::EndLeftLean()
{
	bLeftLean = false;
	ServerEndLeftLean();
}

void ABaseCharacter::RightLean(const FInputActionValue& Value)
{
	bRightLean = true;
	ServerRightLean();
}

void ABaseCharacter::EndRightLean()
{
	bRightLean = false;
	ServerEndRightLean();
}

void ABaseCharacter::ServerStopSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ABaseCharacter::ServerStartSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ABaseCharacter::ServerStartIronSight_Implementation()
{
	bIsIronSight = true;
}

void ABaseCharacter::ServerStopIronSight_Implementation()
{
	bIsIronSight = false;
}

void ABaseCharacter::ServerLeftLean_Implementation()
{
	bLeftLean = true;
}

void ABaseCharacter::ServerEndLeftLean_Implementation()
{
	bLeftLean = false;
}

void ABaseCharacter::ServerRightLean_Implementation()
{
	bRightLean = true;
}

void ABaseCharacter::ServerEndRightLean_Implementation()
{
	bRightLean = false;
}
