// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Ch_4_GameState.generated.h"

/**
 * 
 */
UCLASS()
class CH_4_9_API ACh_4_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXXXX")));
	
};
