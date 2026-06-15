// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Ch_4_GameMode.generated.h"

class ACh_4_PlayerController;
/**
 * 
 */
UCLASS()
class CH_4_9_API ACh_4_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	FString GenerateSecretNumber();
	
	bool IsGuessNumberString(const FString& InNumberString);
	
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
	virtual void BeginPlay() override;
	
	void PrintChatMessageString(ACh_4_PlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ACh_4_PlayerController* InChattingPlayerController);
	
	void CheckAllControllers();
	
	void ResetAll();
protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACh_4_PlayerController>> AllPlayerControllers;
	
	FTimerHandle ResetTimerHandle;
};
