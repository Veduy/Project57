// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasePCM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API ABasePCM : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ABasePCM();

	virtual void UpdateCamera(float DeltaTime);

private:
	float DefaultFOV = 90.f;
	float IronSightFOV = 60.f;
	float ZoomSpeed = 10.f;
};
