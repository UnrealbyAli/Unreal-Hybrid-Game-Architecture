// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Characters/CharacterType.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEV_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMainCharacter* MainCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = CharacterStates)
	ECharacterStates CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = CharacterStates)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = CharacterStates)
	TEnumAsByte<EDeathStates> DeathState;
	
	
};
