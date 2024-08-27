// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"

#include "MyCharacterBase.h"
#include "MyPawnSensingComponent.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIController::AMyAIController()
{
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	SensingComponent = CreateDefaultSubobject<UMyPawnSensingComponent>(TEXT("SensingComp"));
	SensingComponent->SetPeripheralVisionAngle(180.f);
}

void AMyAIController::SetCurrentAllyTarget(AActor* InAllyTarget)
{
	CurrentTarget = InAllyTarget;
}

void AMyAIController::SetCurrentTarget(AActor* InAllyTarget)
{
	CurrentAllyTarget = InAllyTarget;
}

bool AMyAIController::IsValidTarget(AActor* InTarget, bool bEnemy) const
{
	AMyCharacterBase* MyCharacter = GetPawn<AMyCharacterBase>();
	if (!IsValid(MyCharacter))
	{
		return false;
	}

	AMyCharacterBase* TargetCharacter = Cast<AMyCharacterBase>(InTarget);
	if (!IsValid(TargetCharacter))
	{
		return false;
	}

	bool IsEnemyTeam = MyCharacter->GetTeamNumber() != TargetCharacter->GetTeamNumber();
	if (bEnemy != IsEnemyTeam)
	{
		return false;
	}

	return true;
}

void AMyAIController::InitializeBehaviorTree()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	if (IsValid(BTComp) && IsValid(TargetingBehaviorTree))
	{
		UBlackboardComponent* BBComp = Blackboard;
		UseBlackboard(TargetingBehaviorTree->BlackboardAsset, BBComp);
		BBComp->InitializeBlackboard(*TargetingBehaviorTree->BlackboardAsset);
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InitializeBehaviorTree();

	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	if (IsValid(BTComp))
	{
		BTComp->StartTree(*TargetingBehaviorTree);
	}
}
