// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_4_JudgmentWidget.h"
#include "Components/TextBlock.h"

void UCh_4_JudgmentWidget::HaveWinner(const FString& Winner)
{
	if (IsValid(ResultTextBox))
	{
		ResultTextBox->SetText(FText::FromString(FString::Printf(TEXT("Player %s Win!!!!"), *Winner)));
	}
}

void UCh_4_JudgmentWidget::Tie()
{
	if (IsValid(ResultTextBox))
	{
		ResultTextBox->SetText(FText::FromString("No Winner..."));
	}
}

