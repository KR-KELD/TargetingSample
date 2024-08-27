// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacterBase.generated.h"

UCLASS()
class TARGETINGSAMPLE_API AMyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacterBase();

	float GetHp() const { return CurrentHP; }
	float GetMaxHp() const { return MaxHP; }
	int32 GetTeamNumber() const { return TeamNumber; }
public:
	UPROPERTY(EditAnywhere)
	int32 TeamNumber;
private:

	UPROPERTY()
	float CurrentHP;
	UPROPERTY()
	float MaxHP;
};
