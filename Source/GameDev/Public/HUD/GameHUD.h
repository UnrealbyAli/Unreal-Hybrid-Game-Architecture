// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEV_API AGameHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();

	private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameOverlay> GameoverOverlayClass;
	class UGameOverlay* GameOverlay;

public:
	FORCEINLINE UGameOverlay* GetGameOverlay() const { return GameOverlay; }
};
