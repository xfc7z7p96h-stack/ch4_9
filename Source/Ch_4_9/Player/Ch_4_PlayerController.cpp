// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_4_PlayerController.h"

#include "Ch_4_9/UI/Ch_4_UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Ch_4_9/Player/Ch_4_PlayerState.h"
#include "Ch_4_9/Game/Ch_4_GameMode.h"
#include "Ch_4_9/UI/Ch_4_JudgmentWidget.h"

ACh_4_PlayerController::ACh_4_PlayerController()
{
	bReplicates = true;
}

void ACh_4_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning,
	TEXT("Controller=%s Local=%d Authority=%d"),
	*GetName(),
	IsLocalController(),
	HasAuthority());
	
	UE_LOG(LogTemp, Warning, TEXT("Player Controller BeginPlay"));
	
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't create UI Widget for player controller"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FAILED : NotificationTextWidgetClass nullptr"));
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	
	if (IsValid(ChatCh_4_UserWidgetClass))
	{
		ChatCh_4_UserWidget = CreateWidget<UCh_4_UserWidget>(this, ChatCh_4_UserWidgetClass);
		if (IsValid(ChatCh_4_UserWidget))
		{
			ChatCh_4_UserWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("ChatInput Widget Created"));
		}
		
		UE_LOG(LogTemp, Warning,
	TEXT("Chat Widget = %s"),
	*GetNameSafe(ChatCh_4_UserWidget));
	}
	UE_LOG(LogTemp, Warning,
	TEXT("NotificationTextWidgetClass = %s"),
	*GetNameSafe(NotificationTextWidgetClass));

	if (IsValid(NotificationTextWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Class Valid"));

		NotificationTextWidget =
			CreateWidget<UUserWidget>(
				this,
				NotificationTextWidgetClass);

		UE_LOG(LogTemp, Warning,
			TEXT("Instance = %s"),
			*GetNameSafe(NotificationTextWidget));

		if (IsValid(NotificationTextWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("Instance Valid"));

			NotificationTextWidget->AddToViewport();

			UE_LOG(LogTemp, Warning, TEXT("Viewport Added"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Class Invalid"));
	}
}

void ACh_4_PlayerController::SetChatMessageString(const FString& ChatMessageString)
{
	ChatMessage = ChatMessageString;
	
	if (IsLocalController() == true)
	{
		ACh_4_PlayerState* Ch_4_PlayerState = GetPlayerState<ACh_4_PlayerState>();
		if (IsValid(Ch_4_PlayerState) == true)
		{
			FString CombinedMessageString = Ch_4_PlayerState->GetPlayerInfoString() + TEXT(": ") + ChatMessage;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ACh_4_PlayerController::PrintChatMessageString(const FString& ChatMessageString)
{
	UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.f);

}

void ACh_4_PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}

void ACh_4_PlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& ChatMessageString)
{
	PrintChatMessageString(ChatMessageString);
}

void ACh_4_PlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& ChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ACh_4_GameMode* CXGM = Cast<ACh_4_GameMode>(GM);
		if (IsValid(CXGM) == true)
		{
			CXGM->PrintChatMessageString(this, ChatMessageString);
			CXGM->CheckAllControllers();
		}
	}
	
}

void ACh_4_PlayerController::Client_Winner_Implementation(const FString& Winner)
{
	JudgmentWidget = CreateWidget<UCh_4_JudgmentWidget>(this, JudgmentWidgetClass);

	if (IsValid(JudgmentWidget))
	{
		JudgmentWidget->HaveWinner(Winner);
		JudgmentWidget->AddToViewport();
	}
}

void ACh_4_PlayerController::Client_Tie_Implementation()
{
	JudgmentWidget = CreateWidget<UCh_4_JudgmentWidget>(this, JudgmentWidgetClass);
	
	if (IsValid(JudgmentWidget))
	{
		JudgmentWidget->Tie();
		JudgmentWidget->AddToViewport();
	}
}

void ACh_4_PlayerController::Client_ResetUI_Implementation()
{
	if (IsValid(JudgmentWidget))
	{
		JudgmentWidget->RemoveFromParent();
		JudgmentWidget = nullptr;
	}
}
