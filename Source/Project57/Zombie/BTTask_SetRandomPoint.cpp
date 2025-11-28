// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Navigationsystem.h"


UBTTask_SetRandomPoint::UBTTask_SetRandomPoint()
{

}

EBTNodeResult::Type UBTTask_SetRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		FVector Location = AIC->GetPawn()->GetActorLocation();
		FVector Destination;

		//K2_GetRandomLocationInNavigableRadius(UObject * WorldContextObject, const FVector & Origin, FVector & RandomLocation, 
		//	float Radius, ANavigationData * NavData = NULL, TSubclassOf<UNavigationQueryFilter> FilterClass = {});

		if (UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(GetWorld(),
			AIC->GetPawn()->GetActorLocation(),
			Destination,
			5000.f))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
		}
	}
	
	return EBTNodeResult::Succeeded;
}
