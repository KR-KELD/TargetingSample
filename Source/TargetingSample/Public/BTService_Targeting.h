// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "MyStruct.h"

#include "BTService_Targeting.generated.h"

class AMyAIController;


struct FBTServiceTargetingMemory
{
	TWeakObjectPtr<AActor> CurrentTarget;
};

USTRUCT(DisplayName = "Check Condition")
struct FAICheckCondition
{
	GENERATED_BODY()
public:
	bool CompareValue(float InValue)
	{
		bool bResult = false;
		switch (ValueCompareType.GetValue())
		{
		case EArithmeticKeyOperation::Equal:
		{
			if (InValue == Value)
			{
				bResult = true;
			}
		}
		break;
		case EArithmeticKeyOperation::NotEqual:
		{
			if (InValue != Value)
			{
				bResult = true;
			}
		}
		break;
		case EArithmeticKeyOperation::Less:
		{
			if (InValue < Value)
			{
				bResult = true;
			}
		}
		break;
		case EArithmeticKeyOperation::LessOrEqual:
		{
			if (InValue <= Value)
			{
				bResult = true;
			}
		}
		break;
		case EArithmeticKeyOperation::Greater:
		{
			if (InValue > Value)
			{
				bResult = true;
			}
		}
		break;
		case EArithmeticKeyOperation::GreaterOrEqual:
		{
			if (InValue >= Value)
			{
				bResult = true;
			}
		}
		break;
		default:
			break;
		}

		return bResult;
	}
public:

	UPROPERTY(Category = "Check Condition", EditAnywhere, DisplayName = "Check Value Type")
	EAICheckType CheckValueType = EAICheckType::None;

	UPROPERTY(EditAnywhere, Category = "Check Condition", meta = (EditCondition = "CheckValueType == EAICheckType::HPRate || CheckValueType == EAICheckType::Distance", EditConditionHides))
	TEnumAsByte<EArithmeticKeyOperation::Type> ValueCompareType;
	UPROPERTY(EditAnywhere, Category = "Check Condition", meta = (EditCondition = "CheckValueType == EAICheckType::HPRate || CheckValueType == EAICheckType::Distance", EditConditionHides))
	float Value = 0.0f;
};

USTRUCT(DisplayName = "Targeting Condition")
struct FAITargetingCondition
{
	GENERATED_BODY()
public:
	float GetInitValue() const
	{
		if (TargetSelectCondition == EAISelectCondition::Highest)
		{
			return FLT_MIN;
		}
		else
		{
			return FLT_MAX;
		}
	}
	bool CompareSelectValue(float InCurrentValue, float InNewValue) const
	{
		if (TargetSelectCondition == EAISelectCondition::Highest)
		{
			if (InCurrentValue < InNewValue)
			{
				return true;
			}
		}
		else
		{
			if (InCurrentValue > InNewValue)
			{
				return true;
			}
		}
		return false;
	}
public:
	UPROPERTY(Category = "Targeting Condition", EditAnywhere, DisplayName = "Target Select Type")
	EAISelectType TargetSelectType = EAISelectType::Distance;
	UPROPERTY(Category = "Targeting Condition", EditAnywhere, DisplayName = "Target Select Condition")
	EAISelectCondition TargetSelectCondition = EAISelectCondition::Lowest;
	UPROPERTY(Category = "Targeting Condition", EditAnywhere, DisplayName = "Check Conditions")
	TArray<FAICheckCondition> CheckConditions;
};

UCLASS()
class TARGETINGSAMPLE_API UBTService_Targeting : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Targeting(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

	virtual AActor* SelectTarget(AMyAIController* InAIController, uint8* NodeMemory);

	virtual float GetSelectTypeValue(EAISelectType InSelectType, AAIController* InAIController, AActor* InTargetActor);

	virtual bool CheckCondition(const TArray<FAICheckCondition>& InCheckConditions, AAIController* InAIController, AActor* InTargetActor);

	virtual AActor* CalcBestTarget(const FAITargetingCondition& InTargetingCondition, const TArray<AActor*>& InValidTargetList, AAIController* InAIController);

private:
	void GetTargetList(AMyAIController* InAIController, TArray<TWeakObjectPtr<AActor>>& OutTargetList);
	
protected:

	UPROPERTY(EditAnywhere, Category = "Targeting", DisplayName = "Debug")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Targeting", DisplayName = "Use Ally Targeting")
	bool bAllyTargeting;

	UPROPERTY(EditAnywhere, Category = "Targeting", DisplayName = "Targeting Priority")
	TArray<FAITargetingCondition> TargetingPriority;
	
};
