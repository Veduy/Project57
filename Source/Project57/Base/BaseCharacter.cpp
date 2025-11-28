// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameframeWork/SpringArmComponent.h"
#include "GameFramework/DamageType.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
			FVector(3.f,3.f,3.f));
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
	const FRotator CameraRotation =  GetController()->GetControlRotation();
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
	if(AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor()))
	{
		ChildWeapon->Reload();
	}
}

void ABaseCharacter::DoFire()
{
	if (AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor()))
	{
		ChildWeapon->Fire();
	}
}

void ABaseCharacter::StartFire()
{
	bIsFire = true;
	DoFire();
}

void ABaseCharacter::StopFire()
{
	bIsFire = false;
	if (AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor()))
	{
		ChildWeapon->StopFire();
	}
}

void ABaseCharacter::StartIronSight(const FInputActionValue& Value)
{
	bIsIronSight = true;
}

void ABaseCharacter::StopIronSight(const FInputActionValue& Value)
{
	bIsIronSight = false;
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
