// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterType.h"
#include "Enemy.generated.h"

class USoundBase;
UCLASS()
class GAMEDEV_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser) override;
	void SetPatrolTarget();
	void CheckCombatTarget();

	

	
private:
	
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* Healthbar;


	double CombatRadius = 1000.f;
	double InnerRadius = 300.f;
	

	UPROPERTY(EditAnywhere, Category = NavigationAI)
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = NavigationAI)
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = NavigatigationAI)
	double TargetRadius = 200.f;

	FTimerHandle PatrolTimer;
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;

	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsAttacking();
	bool IsChasing();
	void StartChasing();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseIntrest();
	void StartPatrolling();
    void StartAttackTimer();
	void ClearPatrolTimer();
	bool IsDead();
	void ClearAttackTimer();
	bool IsEngaged();
	void SpawnDefaultWeapon();
	void IntializeEnemy();

	void StopMontage();
	float ChasingSpeed = 300.f;
	float PatrollingSpeed = 125.f;
	FTimerHandle AttackTimer;

	float MinAttackTime = 1.5f;
	float MaxAttackTime = 3.f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnSoul();
	virtual void Die_Implementation() override;


	virtual void Attack() override;

	bool InTargetRange(AActor* Target, double Radius);
	void TimerFinished();
	void MovetoTarget(AActor*  Target);
	AActor* ChooseTarget();
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;

	UPROPERTY(EditAnywhere, Category = NavigationAI)
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = NavigationAI)
	float WaitMax = 10.f;

	UPROPERTY(EditAnywhere)
	float DeathLifeSpan = 7.f;

	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyStates EnemyStates = EEnemyStates::EES_Patrolling;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

    UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;
	
     
public:	


};
