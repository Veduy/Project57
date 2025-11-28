// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AIC.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie.h"
#include "../Base/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AZombie_AIC::AZombie_AIC()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	Sight->SightRadius = 300.0f;
	Sight->LoseSightRadius = 400.0f;
	Sight->PeripheralVisionAngleDegrees = 45.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void AZombie_AIC::Tick(float DeltaTime)
{
	//if (APawn* Target = Cast<APawn>(GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"))))
	//{
	//	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), Target->GetActorLocation());

	//	FRotator TargetRotation = FMath::RInterpTo(GetPawn()->GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), 15);
	//	TargetRotation.Pitch = 0;
	//	TargetRotation.Roll = 0;

	//	UE_LOG(LogTemp, Warning, TEXT("Rotation: %f"), TargetRotation.Pitch);
	//	GetPawn()->SetActorRotation(TargetRotation);
	//}
}

void AZombie_AIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	Perception->OnPerceptionUpdated.AddDynamic(this, &AZombie_AIC::PerceptionUpdated);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &AZombie_AIC::TargetPerceptionUpdated);
	Perception->OnTargetPerceptionForgotten.AddDynamic(this, &AZombie_AIC::TargetPerceptionForgotten);
	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AZombie_AIC::TargetPerceptionInfoUpdated);

	SetGenericTeamId(3);
}

void AZombie_AIC::OnUnPossess()
{
	Super::OnUnPossess();

}

void AZombie_AIC::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
}

void AZombie_AIC::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (AZombie* Zombie = Cast<AZombie>(GetOwner()))
		{
			return;
		}

		if (Stimulus.WasSuccessfullySensed())
		{
			if (AZombie* Zombie = Cast<AZombie>(GetPawn()))
			{
				Blackboard->SetValueAsObject(FName("TargetActor"), Actor);
				Blackboard->SetValueAsEnum(FName("State"), (uint8)(EAIState::Chase));
				Zombie->SetState(EAIState::Chase);
				Zombie->ChangeSpeed(300.f);
			}		
		}
		else
		{
			if (AZombie* Zombie = Cast<AZombie>(GetPawn()))
			{
				Blackboard->SetValueAsObject(FName("TargetActor"), nullptr);
				Blackboard->SetValueAsEnum(FName("State"), (uint8)(EAIState::Normal));
				Zombie->SetState(EAIState::Normal);
				Zombie->ChangeSpeed(50.f);
			}
		}
	}
}

void AZombie_AIC::TargetPerceptionForgotten(AActor* Actor)
{


	if (Actor == Blackboard->GetValueAsObject(FName("TargetActor")))
	{
		if (AZombie* Zombie = Cast<AZombie>(GetOwner()))
		{
			return;
		}

		if (AZombie* Zombie = Cast<AZombie>(GetPawn()))
		{
			Blackboard->SetValueAsObject(FName("TargetActor"), nullptr);
			Blackboard->SetValueAsEnum(FName("State"), (uint8)(EAIState::Normal));
			Zombie->SetState(EAIState::Normal);
			Zombie->ChangeSpeed(50.f);
		}		
	}
}

void AZombie_AIC::TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
}

void AZombie_AIC::SetState(EAIState NewState)
{
	Blackboard->SetValueAsEnum(FName("State"), (uint8)(NewState));
}
