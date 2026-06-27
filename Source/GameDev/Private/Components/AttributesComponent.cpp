// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/AttributesComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAttributesComponent::RecieveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}

float UAttributesComponent::GetStamina()
{
	return Stamina / MaxStamina;
}

float UAttributesComponent::ChangeProgressBar()
{
	return Health / MaxHealth;
}

void UAttributesComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

bool UAttributesComponent::IsAlive()
{
	return Health > 0;
}

void UAttributesComponent::RegeStamina(float DeltaTime)
{
	Stamina  = FMath::Clamp(Stamina + RegenStamina * DeltaTime, 0.f, MaxStamina);
}

void UAttributesComponent::AddSouls(int32 SoulsNumber)
{
   SoulsCount += SoulsNumber;
}

void UAttributesComponent::AddGold(int32 GoldNumber)
{
	GoldCount += GoldNumber;
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

