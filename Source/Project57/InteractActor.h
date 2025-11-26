// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base/BaseCharacter.h"
#include "InteractActor.generated.h"

class AItemBase;
class USphereComponent;
class UStaticMeshComponent;
class UStaticMesh;
class AWeaponBase;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None	=	0 UMETA(DisplayName = "None"),
	Use		=	10 UMETA(DisplayName = "Use"),
	Eat		=	20 UMETA(DisplayName = "Eat"),
	Equip	=	30 UMETA(DisplayName = "Equip"),
};

USTRUCT(BlueprintType)
struct FInteractItemInfo
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<AItemBase> ItemTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EWeaponState WeaponType;
};


UCLASS()
class PROJECT57_API AInteractActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	FInteractItemInfo Info;
};
