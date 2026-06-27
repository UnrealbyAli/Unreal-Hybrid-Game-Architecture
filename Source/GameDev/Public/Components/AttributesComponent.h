// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEDEV_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = Attributes)
	float Health;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = Attributes)
	int32 SoulsCount;

	UPROPERTY(VisibleAnywhere, Category = Attributes)
	int32 GoldCount;
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	float Stamina;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float RegenStamina = 2.f;
public:
	void RecieveDamage(float Damage);
	float GetStamina();
	float ChangeProgressBar();
	void UseStamina(float StaminaCost);
	bool IsAlive();
	void RegeStamina(float DeltaTime);
	void AddSouls(int32 SoulsNumber);
	void AddGold(int32 GoldNumber);

	FORCEINLINE int32 GetSoulsCount() { return SoulsCount; }
	FORCEINLINE int32 GetGoldCount() { return GoldCount; }
	FORCEINLINE int32 StaminaGet() {return Stamina;}
	FORCEINLINE float GetDodgeCost() {return DodgeCost; }
};
