// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/MainCharacter.h"	
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

	if (MainCharacter)
	{
		MovementComponent = MainCharacter->GetCharacterMovement();
	}
	
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		IsFalling = MovementComponent->IsFalling();
		CharacterState = MainCharacter->GetCharacterState();
		ActionState = MainCharacter->GetActionState();
		DeathState = MainCharacter->GetDeathState();
	}
}
