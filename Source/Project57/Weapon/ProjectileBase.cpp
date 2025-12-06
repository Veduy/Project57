// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DamageTypeBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	SetReplicates(true);
	SetReplicateMovement(true);
	bNetUseOwnerRelevancy = true;
	bNetLoadOnClient = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(Collision);
	Collision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->MaxSpeed = 5000.f;
	Movement->InitialSpeed = 5000.f;
	Movement->ProjectileGravityScale = 0.1f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.f);

	Collision->OnComponentHit.AddDynamic(this, &AProjectileBase::ComponentHit);
}

void AProjectileBase::ComponentHit(UPrimitiveComponent* HitCompoennt, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, const FHitResult& Hit)
{
	SpawnHitEffect(Hit);

	if (GetOwner())
	{
		//서버가 아니면 총알의 주인이 없다.
		return;
	}

	if (APawn* Pawn = Cast<APawn>(GetOwner()->GetOwner()))
	{
		APlayerController* PC = Cast<APlayerController>(Pawn->GetController());

		UGameplayStatics::ApplyPointDamage(OtherActor, 10, -Hit.ImpactNormal, Hit, PC, this, UDamageTypeBase::StaticClass());

		//범위 공격, 폭탄
		//UGameplayStatics::wda ApplyRadialDamage(HitResult.GetActor(), 10, HitResult.ImpactPoint, 300.0f, UDamageTypeBase::StaticClass(),
		//	ActorsToIgnore,
		//	this,
		//	PC,
		//	true);
	}

	//Destroy();
}

void AProjectileBase::SpawnHitEffect(const FHitResult& Hit)
{
	if (Decal)
	{
		UDecalComponent* MadeDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
			Decal,
			FVector(5, 5, 5),
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			5.f);

		MadeDecal->SetFadeScreenSize(0.f);
	}
}