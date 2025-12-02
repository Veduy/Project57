// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NetworkPawnBase.generated.h"

UCLASS()
class PROJECT57_API ANetworkPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANetworkPawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostNetReceiveRole() override;

	virtual void PostNetInit() override;

	virtual void PossessedBy(AController* NewController);

	virtual void OnRep_Owner() override;
};
