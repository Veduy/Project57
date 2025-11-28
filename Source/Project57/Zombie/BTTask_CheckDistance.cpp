// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckDistance::UBTTask_CheckDistance()
{
	NodeName = TEXT("CheckDistance");
}

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombie* Pawn = Cast<AZombie>(OwnerComp.GetAIOwner()->GetPawn());
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor")));

	if (Pawn && Target)
	{
		float Distance = (float)FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());

		switch (TargetCondition)
		{
		case ECondition::LessThan :
			if (Distance < TargetDistance)
			{
				Pawn->SetState(TargetState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetState);
			}
			break;
		case ECondition::GreaterThan:
			if (Distance > TargetDistance)
			{
				Pawn->SetState(TargetState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetState);
			}
			break;
		}
	}

	return EBTNodeResult::Succeeded;
}
