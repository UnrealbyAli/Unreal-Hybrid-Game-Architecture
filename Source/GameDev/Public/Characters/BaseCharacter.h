// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterType.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
UCLASS()
class GAMEDEV_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
	
protected:
	
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	virtual void Attack();
	virtual bool CanAttack();

	UFUNCTION(BlueprintNativeEvent)
	void Die();
	
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathStates> DeathState;

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
    void StopAttackMontage();
	void DisableCapsule();
	void PlayHitReactMontage(const FName& SectionName);
	void HitDirectionReact(const FVector& Impactpoint);
	bool IsAlive();
	void PlaySound(const FVector& Impactpoint);
	void PlayHitParticles(const FVector& Impactpoint);
	virtual void  HandleDamage(float DamageAmount);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontage(UAnimMontage* Montage, const TArray<FName> SectionNames);


	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationTarget();
	
	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> PlayMontageSectionIndex;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathSectionNames;
	

	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	//components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAttributesComponent* Attributes;

private:
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Particles)
	UParticleSystem* HitParticles;

public:
	FORCEINLINE TEnumAsByte<EDeathStates> GetDeathState() const { return DeathState; }
};
