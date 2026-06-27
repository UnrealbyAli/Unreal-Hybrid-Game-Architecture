// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverlay.generated.h"

UCLASS()
class GAMEDEV_API UGameOverlay : public UUserWidget
{
	GENERATED_BODY()

	public:

	void SetHealthPercent(float Percent);
	void SetStaminaPercent(float Percent);

	void SetGoldText(int32 Gold);
	void SetSoulsText(int32 Souls);
	
	private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Healthprogressbar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsText;
};
