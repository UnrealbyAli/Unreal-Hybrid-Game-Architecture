// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameHUD.h"
#include "HUD/GameOverlay.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && GameoverOverlayClass)
		{
			GameOverlay = CreateWidget<UGameOverlay>(PlayerController, GameoverOverlayClass);
			GameOverlay->AddToViewport();
		}
	}
}
