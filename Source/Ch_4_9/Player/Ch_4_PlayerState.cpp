// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_4_PlayerState.h"

#include "Net/UnrealNetwork.h"

ACh_4_PlayerState::ACh_4_PlayerState()
	: PlayerNameString(TEXT("None"))
, CurrentGuessCount(0)
, MaxGuessCount(3)
{
	bReplicates = true;
}

void ACh_4_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString ACh_4_PlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
 
	if (CurrentGuessCount >= MaxGuessCount)
	{
		return TEXT("No More Chance");
	}
	return PlayerInfoString;
}
