// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributesComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthBarComponent.h"
#include "AIController.h"
#include "Soul.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapons/Weapon.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Healthbar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Healthbar"));
	Healthbar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("pawnsensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, InnerRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, InnerRadius);
}

bool AEnemy::IsAttacking()
{
	return EnemyStates == EEnemyStates::EES_Attacking;
}

bool AEnemy::IsChasing()
{
	return EnemyStates == EEnemyStates::EES_Chasing;
}

void AEnemy::StartChasing()
{
	EnemyStates = EEnemyStates::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MovetoTarget(CombatTarget);
}

void AEnemy::HideHealthBar()
{
	if (Healthbar) Healthbar->SetVisibility(false);
}

void AEnemy::ShowHealthBar()
{
	if (Healthbar) Healthbar->SetVisibility(true);
}

void AEnemy::LoseIntrest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyStates = EEnemyStates::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MovetoTarget(PatrolTarget);
}

void AEnemy::StartAttackTimer()
{
	EnemyStates = EEnemyStates::EES_Attacking;

	const float AttackTime = FMath::RandRange(MinAttackTime, MaxAttackTime);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AEnemy::IsDead()
{
	return EnemyStates == EEnemyStates::EES_Dead;
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsEngaged()
{
	return EnemyStates == EEnemyStates::EES_Engaged;
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("Sword"), this, this );
		EquippedWeapon = DefaultWeapon;
		
	}
}

void AEnemy::IntializeEnemy()
{
	HideHealthBar();
	EnemyController = Cast<AAIController>(GetController());
	MovetoTarget(PatrolTarget);
	SpawnDefaultWeapon();
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    IntializeEnemy();

	Tags.Add(FName("Enemy"));
	
	if (PawnSensing)PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if(World && SoulClass && Attributes)
	{
		ASoul* SpawnedSoul =  World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(Attributes->GetSoulsCount());
		}
	}
}

void AEnemy::Die_Implementation()
{
	
	Super::Die_Implementation();
	EnemyStates = EEnemyStates::EES_Dead;
	
	ClearAttackTimer();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	HideHealthBar();
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SpawnSoul();
	
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyStates = EEnemyStates::EES_Engaged;

	PlayAttackMontage(); 
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistancetoTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistancetoTarget <= Radius;
}

void AEnemy::TimerFinished()
{
	MovetoTarget(PatrolTarget);
}

void AEnemy::MovetoTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(150.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChooseTarget()
{
			TArray<AActor*> ValidTargets;
			for (AActor* TargetActor : PatrolTargets)
			{
				if (TargetActor != PatrolTarget)
				{
					ValidTargets.AddUnique(TargetActor);
				}
			}
			const int32 NumPatrolTargets = ValidTargets.Num();
			if (NumPatrolTargets > 0)
			{
				const int32 Selection = FMath::RandRange(0, NumPatrolTargets - 1);
				return ValidTargets[Selection];
			}

	return nullptr;
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
			!IsAttacking() &&
				!IsEngaged() &&
	         !IsDead();
	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Healthbar)
	{
		ShowHealthBar();
		Healthbar->SetHealthPercent(Attributes->ChangeProgressBar());
	}
}

void AEnemy::AttackEnd()
{
EnemyStates = EEnemyStates::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (SeenPawn->ActorHasTag(FName("Dead"))) return;
	
		const bool bShouldChaseTarget =
				EnemyStates != EEnemyStates::EES_Dead &&
				EnemyStates != EEnemyStates::EES_Chasing &&
			   EnemyStates < EEnemyStates::EES_Attacking &&
			   SeenPawn->ActorHasTag(FName("Character"));
	
		UE_LOG(LogTemp, Warning, TEXT("sEEN from enemy"));
	
		if (bShouldChaseTarget)
		{
			CombatTarget = SeenPawn;
			ClearPatrolTimer();
			StartChasing();
		}
	
	
}
void AEnemy::Destroyed()
{
   if (EquippedWeapon)
   {
	   EquippedWeapon->Destroy();
   }
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser) 
{
	HandleDamage(DamageAmount);
	if (IsDead()) return DamageAmount;
	CombatTarget = EventInstigator->GetPawn();
	if (IsInsideAttackRadius())
	{
		EnemyStates = EEnemyStates::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
	StartChasing();
	}
	return DamageAmount;
}

void AEnemy::SetPatrolTarget()
{
	if (InTargetRange(PatrolTarget, TargetRadius))
	{
		PatrolTarget = ChooseTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::TimerFinished,WaitTime);
	}
}
void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseIntrest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideCombatRadius() && !IsChasing())
	{
	    if (!IsAttacking()) ClearAttackTimer();
		if (!IsEngaged()) StartChasing();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ActorHasTag(FName("Dead"))) return;
	if (IsDead()) return;
	if (EnemyStates > EEnemyStates::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		SetPatrolTarget();
	}
}
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	
    SetWeaponCollision(ECollisionEnabled::NoCollision);
	StopAttackMontage();

	if (IsInsideAttackRadius())
	{
		if (!IsDead()) StartAttackTimer();
	}

	AttackEnd();
	
}




