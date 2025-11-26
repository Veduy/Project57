// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(Collision);
	Collision->SetBoxExtent(FVector(10.f, 10.f, 10.f));

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

	OnActorBeginOverlap.AddDynamic(this, &AProjectileBase::ActorBeginOverlap);
	
}

void AProjectileBase::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UGameplayStatics::ApplyPointDamage(OtherActor, 10, -HitResult.ImpactNormal, HitResult, PC, this, UDamageTypeBase::StaticClass());

	////¹üÀ§ °ø°Ý, ÆøÅº
	//UGameplayStatics::ApplyRadialDamage(HitResult.GetActor(), 10, HitResult.ImpactPoint, 300.0f, UDamageTypeBase::StaticClass(),
	//	ActorsToIgnore,
	//	this,
	//	PC,
	//	true
	//);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


