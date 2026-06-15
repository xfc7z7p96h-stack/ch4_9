// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ch_4_JudgmentWidget.generated.h"


/**
 * 
 */
UCLASS()
class CH_4_9_API UCh_4_JudgmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ResultTextBox;
	
	void HaveWinner(const FString& Winner);
	void Tie();
	
	
};
