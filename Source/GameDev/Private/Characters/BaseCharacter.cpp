// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "VectorVMExperimental.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/Weapon.h"
#include "Components/AttributesComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CharacterType.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
	if (CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::Die_Implementation()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

void ABaseCharacter::AttackEnd()
{
}


void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		HitDirectionReact(Hitter->GetActorLocation());
	}
	else Die();
	PlaySound(ImpactPoint);
	PlayHitParticles(ImpactPoint);
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontage(AnimMontage, PlayMontageSectionIndex);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontage(DeathMontage, DeathSectionNames);
	TEnumAsByte<EDeathStates> Pose(Selection);
	if (Pose < EDeathStates::EDP_MAX)
	{
		DeathState = Pose;
	}
	return Selection;
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AnimMontage);
	}
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::HitDirectionReact(const FVector& Impactpoint)
{

	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(Impactpoint.X, Impactpoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double costheta = FVector::DotProduct(Forward, ToHit);
	double theta = FMath::Acos(costheta);

	theta = FMath::RadiansToDegrees((theta));

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0 )
	{
		theta *= -1.f;
	}

	FName Section("FromBack");
	if (theta >= -45.f && theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (theta >= 45.f && theta < 135.f)
	{
		Section = FName("FromRight");
	}
	else if (theta >= -135.f && theta < -45.f )
	{
		Section = FName("FromLeft");
	}
	PlayHitReactMontage(Section);
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::PlaySound(const FVector& Impactpoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, Impactpoint, FRotator(0.f,0.f,0.f));
	}
}

void ABaseCharacter::PlayHitParticles(const FVector& Impactpoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, Impactpoint);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)	Attributes->RecieveDamage(DamageAmount);

}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontage(UAnimMontage* Montage, const TArray<FName> SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	int32 MontageSectionIndex = SectionNames.Num() - 1;
	int32 Selection = FMath::RandRange(0, MontageSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

FVector ABaseCharacter::GetTranslationTarget()
{
	if (CombatTarget == nullptr) return FVector();
	const FVector TargetLocation = CombatTarget->GetActorLocation();
	const FVector Location =	GetActorLocation();

	FVector TargettoMe = (Location - TargetLocation).GetSafeNormal();
	TargettoMe *= 75.f;
	return TargetLocation + TargettoMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
			return CombatTarget->GetActorLocation();
	}
return FVector();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox() )
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoredActors.Empty();
	}
}



