// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception\AIPerceptionComponent.h"
#include "Zombie.h"
#include "Zombie_AIC.generated.h"

//FPerceptionUpdatedDelegate, const TArray<AActor*>&, UpdatedActors);
//FActorPerceptionUpdatedDelegate, AActor*, Actor, FAIStimulus, Stimulus);
//FActorPerceptionForgetUpdatedDelegate, AActor*, Actor);
// 
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorPerceptionInfoUpdatedDelegate, const FActorPerceptionUpdateInfo&, UpdateInfo);
//FActorPerceptionInfoUpdatedDelegate OnTargetPerceptionInfoUpdated;

UCLASS()
class PROJECT57_API AZombie_AIC : public AAIController
{
	GENERATED_BODY()

public:
	AZombie_AIC();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void TargetPerceptionForgotten(AActor* Actor);

	UFUNCTION()
	void TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION()
	void SetState(EAIState NewState);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> Perception;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};