// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FaceToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree\BTService.h"

UBTService_FaceToTarget::UBTService_FaceToTarget()
{

}

void UBTService_FaceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Pawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor")));

	if (Target && Pawn)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), Target->GetActorLocation());

		FRotator TargetRotation = FMath::RInterpTo(Pawn->GetActorRotation(), Rotation, DeltaSeconds, 15);
		TargetRotation.Pitch = 0;
		TargetRotation.Roll = 0;

		Pawn->SetActorRotation(TargetRotation);
	}
}
