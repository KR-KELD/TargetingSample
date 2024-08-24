// Copyright Epic Games, Inc. All Rights Reserved.

#include "TargetingSampleGameMode.h"
#include "TargetingSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATargetingSampleGameMode::ATargetingSampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
