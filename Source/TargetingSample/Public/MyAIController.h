// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class UMyPawnSensingComponent;
/**
 * 
 */
UCLASS()
class TARGETINGSAMPLE_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIController();

	void SetCurrentAllyTarget(AActor* InAllyTarget);
	void SetCurrentTarget(AActor* InAllyTarget);
	UMyPawnSensingComponent* GetSensingComponent() const { return SensingComponent; }

	bool IsValidTarget(AActor* InTarget, bool bEnemy = true) const;
	void InitializeBehaviorTree();

protected:
	virtual void OnPossess(APawn* InPawn) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	UBehaviorTree* TargetingBehaviorTree;
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UMyPawnSensingComponent* SensingComponent;

	UPROPERTY()
	AActor* CurrentTarget;
	UPROPERTY()
	AActor* CurrentAllyTarget;
};
