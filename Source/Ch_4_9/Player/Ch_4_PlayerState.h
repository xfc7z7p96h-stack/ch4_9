// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Ch_4_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CH_4_9_API ACh_4_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	
	ACh_4_PlayerState();
	
	UPROPERTY(Replicated)
	FString PlayerNameString;
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();
	
};
