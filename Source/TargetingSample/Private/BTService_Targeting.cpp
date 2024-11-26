// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Targeting.h"

#include "MyAIController.h"
#include "MyCharacterBase.h"
#include "MyPawnSensingComponent.h"

UBTService_Targeting::UBTService_Targeting(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Targeting");
	Interval = 0.3f;
	RandomDeviation = 0.02f;
	bAllyTargeting = false;
	bDebug = true;
}

void UBTService_Targeting::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (TargetingPriority.Num() == 0)
	{
		FAITargetingCondition DefaultTargetingCond;
		DefaultTargetingCond.TargetSelectType = EAISelectType::Distance;
		DefaultTargetingCond.TargetSelectCondition = EAISelectCondition::Lowest;
		TargetingPriority.Add(DefaultTargetingCond);
	}
}

void UBTService_Targeting::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AMyAIController* AIController = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController))
	{
		return;
	}

	AActor* TargetActor = SelectTarget(AIController, NodeMemory);

	if (bAllyTargeting)
	{
		AIController->SetCurrentAllyTarget(TargetActor);
	}
	else
	{
		AIController->SetCurrentTarget(TargetActor);
	}

	if (bDebug)
	{
		AActor* MyActor = AIController->GetPawn();
		if (IsValid(MyActor))
		{
			if (IsValid(TargetActor))
			{
				FVector StartLoc = MyActor->GetActorLocation();
				FVector EndLoc = StartLoc + (TargetActor->GetActorLocation() - StartLoc) / 3.0f;
				DrawDebugDirectionalArrow(GetWorld(), StartLoc, EndLoc, 50.0f, bAllyTargeting ? FColor::Blue : FColor::Red, false, DeltaSeconds);
			}
			else
			{
				DrawDebugSphere(GetWorld(), MyActor->GetActorLocation(), 50.0f, 20, bAllyTargeting ? FColor::Cyan : FColor::Magenta, false, DeltaSeconds);
			}
		}

		UMyPawnSensingComponent* PawnSensingComp = AIController->GetSensingComponent();
		if (IsValid(PawnSensingComp))
		{
			DrawDebugSphere(GetWorld(), MyActor->GetActorLocation(), PawnSensingComp->SightRadius, 20, FColor::Green, false, DeltaSeconds);
		}
	}
}

uint16 UBTService_Targeting::GetInstanceMemorySize() const
{
	return sizeof(FBTServiceTargetingMemory);
}

AActor* UBTService_Targeting::SelectTarget(AMyAIController* InAIController, uint8* NodeMemory)
{
	if (!IsValid(InAIController))
	{
		return nullptr;
	}

	FBTServiceTargetingMemory* MyMemory = CastInstanceNodeMemory<FBTServiceTargetingMemory>(NodeMemory);
	if (!MyMemory)
	{
		return nullptr;
	}

	AActor* SelectedTarget = nullptr;
	for (int32 Priority = 0; Priority < TargetingPriority.Num(); Priority++)
	{
		TArray<TWeakObjectPtr<AActor>> RecognizedTargetList;
		GetTargetList(InAIController, RecognizedTargetList);
		TArray<AActor*> ValidTargetList;
		for (int32 Target = 0; Target < RecognizedTargetList.Num(); Target++)
		{
			// 유효성 검사
			AActor* TargetActor = RecognizedTargetList[Target].Get();
			if (!InAIController->IsValidTarget(TargetActor, !bAllyTargeting))
			{
				continue;
			}

			// 조건 검사
			if (!CheckCondition(TargetingPriority[Priority].CheckConditions, InAIController, TargetActor))
			{
				continue;
			}

			ValidTargetList.Add(TargetActor);
		}

		SelectedTarget = CalcBestTarget(TargetingPriority[Priority], ValidTargetList, InAIController);

		// 적정 타겟 서치
		if (IsValid(SelectedTarget))
		{
			MyMemory->CurrentTarget = SelectedTarget;
			break;
		}
	}

	return SelectedTarget;
}

float UBTService_Targeting::GetSelectTypeValue(EAISelectType InSelectType, AAIController* InAIController, AActor* InTargetActor)
{
	if (!IsValid(InAIController))
	{
		return FAISystem::InvalidRange;
	}

	if (!IsValid(InTargetActor))
	{
		return FAISystem::InvalidRange;
	}

	AActor* MyActor = InAIController->GetPawn();
	if (!IsValid(MyActor))
	{
		return FAISystem::InvalidRange;
	}

	switch (InSelectType)
	{
	case EAISelectType::Distance:
	{
		return FVector::Dist2D(MyActor->GetActorLocation(), InTargetActor->GetActorLocation());
	}
	break;
	case EAISelectType::HPRate:
	{
		AMyCharacterBase* TargetCharacter = Cast<AMyCharacterBase>(InTargetActor);
		if (IsValid(TargetCharacter))
		{
			return TargetCharacter->GetHp() / TargetCharacter->GetMaxHp() * 100.0f;
		}
	}
	break;
	default:
		break;
	}
	return FAISystem::InvalidRange;
}

bool UBTService_Targeting::CheckCondition(const TArray<FAICheckCondition>& InCheckConditions, AAIController* InAIController, AActor* InTargetActor)
{
	if (!IsValid(InAIController))
	{
		return false;
	}

	if (!IsValid(InTargetActor))
	{
		return false;
	}

	AActor* MyActor = InAIController->GetPawn();
	if (!IsValid(MyActor))
	{
		return false;
	}

	if (InCheckConditions.Num() == 0)
	{
		return true;
	}

	bool bResult = false;
	for (auto CheckConditionData : InCheckConditions)
	{
		bResult = false;
		switch (CheckConditionData.CheckValueType)
		{
		case EAICheckType::Distance:
		{
			float Dist = FVector::Dist2D(MyActor->GetActorLocation(), InTargetActor->GetActorLocation());
			if (CheckConditionData.CompareValue(Dist))
			{
				bResult = true;
			}
		}
		break;
		case EAICheckType::HPRate:
		{
			AMyCharacterBase* TargetCharacter = Cast<AMyCharacterBase>(InTargetActor);
			if (IsValid(TargetCharacter))
			{
				float HpRate = TargetCharacter->GetHp() / TargetCharacter->GetMaxHp() * 100.0f;
				if (CheckConditionData.CompareValue(HpRate))
				{
					bResult = true;
				}
			}
		}
		break;
		default:
			break;
		}

		// 한 조건이라도 통과하지 못하면 실패
		if (!bResult)
		{
			break;
		}
	}

	return bResult;
}

AActor* UBTService_Targeting::CalcBestTarget(const FAITargetingCondition& InTargetingCondition, const TArray<AActor*>& InValidTargetList, AAIController* InAIController)
{
	AActor* BestTarget = nullptr;
	switch (InTargetingCondition.TargetSelectCondition)
	{
	case EAISelectCondition::Lowest:
	case EAISelectCondition::Highest:
	{
		float BestValue = InTargetingCondition.GetInitValue();

		for (int32 ValidTarget = 0; ValidTarget < InValidTargetList.Num(); ValidTarget++)
		{
			// 유효성 검사
			AActor* TargetActor = InValidTargetList[ValidTarget];

			// 선택 조건검사
			float NewValue = GetSelectTypeValue(InTargetingCondition.TargetSelectType, InAIController, TargetActor);
			if (NewValue == FAISystem::InvalidRange)
			{
				continue;
			}

			if (InTargetingCondition.CompareSelectValue(BestValue, NewValue))
			{
				BestTarget = TargetActor;
				BestValue = NewValue;
			}
		}
	}
		break;
	case EAISelectCondition::Random:
	{
		int32 RandomIndex = InValidTargetList.Num() > 0 ? FMath::RandRange(0, InValidTargetList.Num() - 1) : INDEX_NONE;
		if (RandomIndex != INDEX_NONE)
		{
			BestTarget = InValidTargetList[RandomIndex];
		}
	}
		break;
	default:
		break;
	}
	return BestTarget;
}

void UBTService_Targeting::GetTargetList(AMyAIController* InAIController, TArray<TWeakObjectPtr<AActor>>& OutTargetList)
{
	if (!IsValid(InAIController))
	{
		return;
	}

	UMyPawnSensingComponent* PawnSensingComp = InAIController->GetSensingComponent();
	if (!IsValid(PawnSensingComp))
	{
		return;
	}

	if (bAllyTargeting)
	{
		OutTargetList = PawnSensingComp->GetAllyTargets();
	}
	else
	{
		OutTargetList = PawnSensingComp->GetKnownTargets();
	}

}
