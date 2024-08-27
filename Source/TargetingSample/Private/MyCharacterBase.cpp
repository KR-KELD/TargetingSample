// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterBase.h"


AMyCharacterBase::AMyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MaxHP = 100.0f;
	CurrentHP = FMath::RandRange(20.0f, 100.0f);
}
