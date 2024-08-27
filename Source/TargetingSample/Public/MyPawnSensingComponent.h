// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "MyPawnSensingComponent.generated.h"

class AMyCharacterBase;
/**
 * 
 */
UCLASS()
class TARGETINGSAMPLE_API UMyPawnSensingComponent : public UPawnSensingComponent
{
	GENERATED_BODY()
	
public:
	UMyPawnSensingComponent();

	// Begin PawnSensingComponent interface
	virtual bool CanSenseAnything() const override;
	virtual void UpdateAISensing() override;
	virtual bool CouldSeePawn(const APawn* Other, bool bMaySkipChecks = false) const override;
	// End PawnSensingComponent interface

	const TArray<TWeakObjectPtr<AActor>>& GetKnownTargets() { return KnownTargets; }
	const TArray<TWeakObjectPtr<AActor>>& GetAllyTargets() { return AllyTargets; }
public:
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> KnownTargets;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> AllyTargets;
};
