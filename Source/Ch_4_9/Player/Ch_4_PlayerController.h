// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ch_4_PlayerController.generated.h"

class UCh_4_UserWidget;
class UUserWidget;
class UCh_4_JudgmentWidget;
/**
 * 
 */
UCLASS()
class CH_4_9_API ACh_4_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	ACh_4_PlayerController();
	
	virtual void BeginPlay() override;
	
	void SetChatMessageString(const FString& ChatMessageString);
	
	void PrintChatMessageString(const FString& ChatMessageString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& ChatMessageString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& ChatMessageString);
	
	UFUNCTION(Client, Reliable)
	void Client_Winner(const FString& Winner);
	
	UFUNCTION(Client, Reliable)
	void Client_Tie();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Client, Reliable)
	void Client_ResetUI();
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCh_4_UserWidget> ChatCh_4_UserWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCh_4_UserWidget> ChatCh_4_UserWidget;
	
	FString ChatMessage;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCh_4_JudgmentWidget> JudgmentWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCh_4_JudgmentWidget> JudgmentWidget;
	
public:
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
