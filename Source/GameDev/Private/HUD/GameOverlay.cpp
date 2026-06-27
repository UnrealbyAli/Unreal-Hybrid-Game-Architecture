// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGameOverlay::SetHealthPercent(float Percent)
{
	if (Healthprogressbar)
	{
		Healthprogressbar->SetPercent(Percent);
	}
}

void UGameOverlay::SetStaminaPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UGameOverlay::SetGoldText(int32 Gold)
{
	if (GoldText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
		FText Text = FText::FromString(String);
		GoldText->SetText(Text);
	}
}

void UGameOverlay::SetSoulsText(int32 Souls)
{
	if (SoulsText)
	{
		const FString String = FString::Printf(TEXT("%d"), Souls);
		FText Text = FText::FromString(String);
		SoulsText->SetText(Text);
	}
}
