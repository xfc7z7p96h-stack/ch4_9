// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_4_GameState.h"

#include "Kismet/GameplayStatics.h"
#include "Ch_4_9/Player/Ch_4_PlayerController.h"

void ACh_4_GameState::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACh_4_PlayerController* CPC = Cast<ACh_4_PlayerController>(PC);
			if (IsValid(CPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
