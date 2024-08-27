// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawnSensingComponent.h"

#include "MyCharacterBase.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"

UMyPawnSensingComponent::UMyPawnSensingComponent()
{
	SensingInterval = 0.5f;
	bHearNoises = false;
	SightRadius = 1500.0f;
}

bool UMyPawnSensingComponent::CanSenseAnything() const
{
	return SightRadius > 0.0f;
}

void UMyPawnSensingComponent::UpdateAISensing()
{
	const AController* const OwnerController = Cast<AController>(GetOwner());
	if (!IsValid(OwnerController) || !IsValid(OwnerController->GetWorld()))
	{
		return;
	}

	AMyCharacterBase* OwnerCharacter = Cast<AMyCharacterBase>(OwnerController->GetPawn());
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	KnownTargets.Empty();
	AllyTargets.Empty();

	for (TActorIterator<APawn> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		AMyCharacterBase* const KnownCharacter = Cast<AMyCharacterBase>(*Iterator);
		if (!IsSensorActor(KnownCharacter) && CouldSeePawn(KnownCharacter, true))
		{
			if (OwnerCharacter->GetTeamNumber() == KnownCharacter->GetTeamNumber())
			{
				AllyTargets.AddUnique(KnownCharacter);
			}
			else
			{
				KnownTargets.AddUnique(KnownCharacter);
			}
		}
	}
}

bool UMyPawnSensingComponent::CouldSeePawn(const APawn* Other, bool bMaySkipChecks) const
{
	const AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return false;
	}

	if (!IsValid(Other))
	{
		return false;
	}

	float OwnerHalfHeight = 0.f;
	float OwnerRadius = 0.f;
	const AController* const OwnerController = Cast<AController>(Owner);
	if (IsValid(OwnerController))
	{
		const AMyCharacterBase* const MyCharacter = Cast<AMyCharacterBase>(OwnerController->GetPawn());
		if (IsValid(MyCharacter))
		{
			OwnerHalfHeight = MyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			OwnerRadius = MyCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
		}
	}

	float OtherRadius = 0.f;
	const AMyCharacterBase* const TargetCharacter = Cast<AMyCharacterBase>(Other);
	if (IsValid(TargetCharacter))
	{
		OtherRadius = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	FVector const OtherLoc = Other->GetActorLocation();
	FVector const SensorLoc = GetSensorLocation();
	FVector const SelfToOther = OtherLoc - SensorLoc;

	float const ZValue = OtherLoc.Z - SensorLoc.Z;
	if (ZValue > OwnerHalfHeight * 2.f)
	{
		return false;
	}

	float const SelfToOtherDistSquared = SelfToOther.Size2D() - (OwnerRadius + OtherRadius);
	if (SelfToOtherDistSquared > SightRadius)
	{
		return false;
	}

	if (bMaySkipChecks && (FMath::Square(FMath::FRand()) * SelfToOtherDistSquared > FMath::Square(0.4f * SightRadius)))
	{
		return false;
	}

	FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
	FVector const MyFacingDir = GetSensorRotation().Vector();

	return ((SelfToOtherDir | MyFacingDir) >= PeripheralVisionCosine);
}
