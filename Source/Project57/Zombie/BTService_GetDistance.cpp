// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetDistance.h"
#include "BTService_GetDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTService_GetDistance::UBTService_GetDistance()
{

}

void UBTService_GetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (Target)
	{
		FVector ZombieLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
		FVector PlayerLocation = Target->GetActorLocation();
		float Distance = (float)FVector::Dist2D(ZombieLocation, PlayerLocation);
	}
}
