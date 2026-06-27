// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include  "Characters/CharacterType.h"
#include "Interfaces/PickupInterface.h"
#include "MainCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class AItem;
class UAnimMontage;
class UGroomComponent;


UCLASS()
class GAMEDEV_API AMainCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	void ChangeHUD();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetOverlappingItem(class AItem* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void Jump() override;

	virtual void AddGold(class ATreasure* Gold) override;
	
	UFUNCTION()
	void EnemySeen(APawn* SeenPawn);
	bool IsOccupied();
	bool HasStamina();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Die_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* EKeyPressed;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeInput;

	void Dodge();

	UPROPERTY(BlueprintReadOnly)
	AActor* Enemy;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;
	
	
	void Look(const FInputActionValue& Value);
	void Moveforward(const FInputActionValue& Value);
	void EPressed();
	virtual void Attack() override;
	
	void PlayBigSwordMontage();
	void PlayEquipMontage(FName EquipSectionName);
	
	bool CanDisarm();
	bool CanArm();

	virtual void AttackEnd() override;

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	
	UFUNCTION(BlueprintCallable)
	FVector GetEchoRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetEchoTranslationTarget();

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* DodgeMontage;

	void PlayDodgeMontage();

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();
	
	UFUNCTION(BlueprintCallable)
	void FinishEqupping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	virtual bool CanAttack() override;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
	class UGameOverlay* GameOverlay;
private:

	void IntializeGameOverlay();
	ECharacterStates CharacterState = ECharacterStates::ECS_UnEquipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true));
	EActionState ActionState = EActionState::EAS_UnOccupied;

	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* BigSwordMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
class 	ABigSword* EquippedBigSword;
	

public:
	FORCEINLINE ECharacterStates GetCharacterState() const {return CharacterState; }
	FORCEINLINE EActionState GetActionState() const {return ActionState; }
};
