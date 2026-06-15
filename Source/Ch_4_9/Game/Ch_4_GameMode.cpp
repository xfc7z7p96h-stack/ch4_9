// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_4_GameMode.h"

#include "Ch_4_9/Game/Ch_4_GameState.h"
#include "Ch_4_9/Player/Ch_4_PlayerController.h"
#include "EngineUtils.h"
#include "Ch_4_9/Player/Ch_4_PlayerState.h"

void ACh_4_GameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ACh_4_PlayerController* PlayerController = Cast<ACh_4_PlayerController>(NewPlayer);
	if (IsValid(PlayerController) == true)
	{
		PlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(PlayerController);

		ACh_4_PlayerState* CXPS = PlayerController->GetPlayerState<ACh_4_PlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACh_4_GameState* NGameState =  GetGameState<ACh_4_GameState>();
		if (IsValid(NGameState) == true)
		{
			NGameState->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
		}
	}
}

FString ACh_4_GameMode::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}
	return Result;
}

bool ACh_4_GameMode::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			if (UniqueDigits.Contains(C))
			{
				return false;
			}
			
			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString ACh_4_GameMode::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	UE_LOG(LogTemp, Error,
	TEXT("Secret=%s Len=%d"),
	*InSecretNumberString,
	InSecretNumberString.Len());

	UE_LOG(LogTemp, Error,
		TEXT("Guess=%s Len=%d"),
		*InGuessNumberString,
		InGuessNumberString.Len());
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	
	if (StrikeCount ==3)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			ACh_4_PlayerController* PC = Cast<ACh_4_PlayerController>(It->Get());
			
			if (IsValid(PC))
			{
				ACh_4_PlayerState* PS = PC->GetPlayerState<ACh_4_PlayerState>();
				
				if (IsValid(PS))
				{
					FString PlayerName = PS->GetName();
				    PC->Client_Winner(PlayerName);
					
					GetWorld()->GetTimerManager().SetTimer(
						ResetTimerHandle,
						this,
						&ACh_4_GameMode::ResetAll,
						4.f,
						false
					);
				}
			}

			
		}
		return FString::Printf(TEXT("Correct all numbers!! Strike!!!"));
	}
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACh_4_GameMode::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString)
	
	UE_LOG(LogTemp, Error,
	   TEXT("Generated Secret = %s"),
	   *SecretNumberString);
}

void ACh_4_GameMode::PrintChatMessageString(ACh_4_PlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		
		IncreaseGuessCount(InChattingPlayerController);
		
		
		for (TActorIterator<ACh_4_PlayerController> It(GetWorld()); It; ++It)
		{
			ACh_4_PlayerController* PlayerController = *It;
			if (IsValid(PlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
				
			}
		}
	}
	else
	{
		for (TActorIterator<ACh_4_PlayerController> It(GetWorld()); It; ++It)
		{
			ACh_4_PlayerController* PlayerController = *It;
			if (IsValid(PlayerController) == true)
			{
				PlayerController->ClientRPCPrintChatMessageString(TEXT("Try again ues three numbers"));
			}
		
		}
	}
}

void ACh_4_GameMode::IncreaseGuessCount(ACh_4_PlayerController* InChattingPlayerController)
{
	ACh_4_PlayerState* PS = InChattingPlayerController->GetPlayerState<ACh_4_PlayerState>();
	if (IsValid(PS)== true)
	{
		PS->CurrentGuessCount++;
	}
}

void ACh_4_GameMode::CheckAllControllers()
{
	int32 GuessCount = 0;
	int32 PlayerCount = 0;

	ACh_4_GameState* GS = GetWorld()->GetGameState<ACh_4_GameState>();

	if (IsValid(GS))
	{
		PlayerCount = GS->PlayerArray.Num();
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ACh_4_PlayerController* PC = Cast<ACh_4_PlayerController>(It->Get());

		if (!IsValid(PC)) continue;

		ACh_4_PlayerState* PS = PC->GetPlayerState<ACh_4_PlayerState>();

		if (!IsValid(PS)) continue;

		if (PS->CurrentGuessCount >= 3)
		{
			GuessCount++;
		}
	}
	
	if (GuessCount == PlayerCount)
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			ACh_4_PlayerController* PC = Cast<ACh_4_PlayerController>(It->Get());

			if (IsValid(PC))
			{
				PC->Client_Tie();
				
				GetWorld()->GetTimerManager().SetTimer(
					ResetTimerHandle,
					this,
					&ACh_4_GameMode::ResetAll,
					4.0f,
					false
				);
			}
		}
	}
}

void ACh_4_GameMode::ResetAll()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetAll"));
	ACh_4_GameState* GS = GetGameState<ACh_4_GameState>();

	if (IsValid(GS))
	{
		for (APlayerState* PS : GS->PlayerArray)
		{
			ACh_4_PlayerState* MyPS = Cast<ACh_4_PlayerState>(PS);

			if (IsValid(MyPS))
			{
				MyPS->CurrentGuessCount = 0;
			}
		}
	}
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ACh_4_PlayerController* PC = Cast<ACh_4_PlayerController>(It->Get());
		
		if (IsValid(PC))
		{
			PC->Client_ResetUI();
		}
	}
	
	SecretNumberString = GenerateSecretNumber();
	
}
