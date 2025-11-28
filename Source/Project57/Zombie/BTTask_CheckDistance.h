// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Zombie.h"
#include "BTTask_CheckDistance.generated.h"

UENUM(BlueprintType)
enum class ECondition : uint8
{
	LessThan = 0 UMETA(DisplayName = "<"),
	GreaterThan = 1 UMETA(DisplayName = ">")
};

UCLASS()
class PROJECT57_API UBTTask_CheckDistance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_CheckDistance();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECondition TargetCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAIState TargetState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetDistance;
};
