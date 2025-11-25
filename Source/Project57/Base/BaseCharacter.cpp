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
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"

#include "../Weapon/WeaponBase.h"
#include "BasePC.h"
#include "../Weapon/DamageTypeBase.h"



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
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::TakeAnyDamage);

	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ChildWeapon->SocketName);
	}
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
		Input->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &ABaseCharacter::DoFire);
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HitMontage)
	{
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

		PlayAnimMontage(HitMontage, 1.f, HitMonatageSection[FMath::RandRange(0, 7)]);
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
			//UE_LOG(LogTemp, Warning, TEXT("Point Damage %f %s"), DamageAmount, *(Event->HitInfo.BoneName.ToString()));
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
			//UE_LOG(LogTemp, Warning, TEXT("Radial Damage %f %s"), DamageAmount, *Event->DamageTypeClass->GetName());
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
		//UE_LOG(LogTemp, Warning, TEXT("Damage %f"), DamageAmount);
	}


	if (CurrentHP <= 0)
	{
		if (DeathMontage)
		{
			FName DeathMonatageSection[6] =
			{
				"Back_01",
				"Front_01",
				"Front_02",
				"Front_03",
				"Left_01"
				"Right_01"
			};

			PlayAnimMontage(DeathMontage, 1.f, DeathMonatageSection[FMath::RandRange(0, 5)]);

			// 애니메이션 시간 구해서 넣기.
			FTimerHandle Timer;
			GetWorld()->GetTimerManager().SetTimer(Timer, 
				[this]()
				{
					GetMesh()->SetSimulatePhysics(true);
				}, 
				1.f, false, 1.f);
		};
	}


	return DamageAmount;
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
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
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
			UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 10,-HitResult.ImpactNormal, HitResult, PC, this, UDamageTypeBase::StaticClass());
		}
	
	}

	if (AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor()))
	{
		ChildWeapon->Fire();
		if (ABasePC* PC = Cast<ABasePC>(GetController()))
		{
			PC->FireAim();
		}
	}
}
