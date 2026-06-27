// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "BigSword.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEV_API ABigSword : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator) override;
	
	
};
