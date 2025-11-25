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

#include "EnhancedInputComponent.h"
#include "Engine/DamageEvents.h"

#include "TimerManager.h"

#include "../Weapon/WeaponBase.h"

#include "BasePC.h"



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
		ChildWeapon->SetOwner(this);
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
		Input->BindAction(IA_Fire, ETriggerEvent::Started, this, &ABaseCharacter::StartFire);
		Input->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ABaseCharacter::StopFire);
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FVector_NetQuantizeNormal HitDirection;

	if (CurrentHP <= 0)
	{
		return DamageAmount;
	}

	if (HitMontage)
	{
		PlayAnimMontage(HitMontage, 1.f, HitMonatageSection[FMath::RandRange(0, 7)]);
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
			HitDirection = Event->ShotDirection;
			//UE_LOG(LogTemp, Warning, TEXT("Point Damage %f %s"), DamageAmount, *(Event->HitInfo.BoneName.ToString()));
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;

			// 오리진에서 직접 계산 필요
			//HitDirection = Event->Origin;
			
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
			// HitDriection 하고 Forward, Left, Right, Back, 내적해서 값이 가장 작은 방향으로 선정.	
			PlayAnimMontage(DeathMontage, 1.f, DeathMonatageSection[FMath::RandRange(0, 5)]);

			//FTimerHandle Timer;
			//GetWorld()->GetTimerManager().SetTimer(Timer, 
			//	[this]()
			//	{
			//		GetMesh()->SetSimulatePhysics(true);
			//	}, 
			//	1.f, false, 1.f);
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
	if (AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor()))
	{
		ChildWeapon->Fire();

		if (ABasePC* PC = Cast<ABasePC>(GetController()))
		{
			PC->FireAim();
		}
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

void ABaseCharacter::DoHitReact()
{

}
