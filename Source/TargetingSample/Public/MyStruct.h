// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TARGETINGSAMPLE_API MyStruct
{
public:
	MyStruct();
	~MyStruct();
};

UENUM()
enum class EAICheckType : uint8
{
	None		UMETA(DisplayName = "None"),
	Distance	UMETA(DisplayName = "Distance"),
	HPRate		UMETA(DisplayName = "HP Rate(%)"),
};

UENUM()
enum class EAISelectType : uint8
{
	Distance		UMETA(DisplayName = "Distance"),
	HPRate			UMETA(DisplayName = "HP Rate(%)"),
};

UENUM()
enum class EAISelectCondition : uint8
{
	Lowest		UMETA(DisplayName = "Lowest"),
	Highest		UMETA(DisplayName = "Highest"),
	Random		UMETA(DisplayName = "Random"),
};
