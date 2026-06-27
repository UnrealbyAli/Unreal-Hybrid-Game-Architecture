// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/MainCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include  "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include  "GameFramework/CharacterMovementComponent.h"
#include "Item.h"
#include "Weapons/Weapon.h"
#include  "Characters/CharacterType.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "GroomComponent.h"
#include "Soul.h"
#include "HUD/GameHUD.h"
#include "HUD/GameOverlay.h"
#include "Weapons/BigSword.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributesComponent.h"
#include "Treasure/Treasure.h"
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,360.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCom"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");
	
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrow"));
	Eyebrows->SetupAttachment((GetMesh()));
	Eyebrows->AttachmentName = FString("head");

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bSeePawns = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}
// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Character"));
	IntializeGameOverlay();

	if (PawnSensing)
		{
		PawnSensing->OnSeePawn.AddDynamic(this, &AMainCharacter::EnemySeen);
		UE_LOG(LogTemp, Warning, TEXT("pAWN seen  set"));
		}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsytem->AddMappingContext(MappingContext, 0);
		}
	}
}

void AMainCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState  = EActionState::EAS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::EnemySeen(APawn* SeenPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("before reach"));
	if (SeenPawn->ActorHasTag(FName("Enemy")))
	{
		Enemy = SeenPawn;
		UE_LOG(LogTemp, Warning, TEXT("sEEN YOU"));
	}
}

bool AMainCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_UnOccupied;
}

bool AMainCharacter::HasStamina()
{
	return Attributes && Attributes->StaminaGet() > Attributes->GetDodgeCost();
}

void AMainCharacter::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT("It didnt even called"))
	if (IsOccupied() || !HasStamina() ) return;

	UE_LOG(LogTemp, Warning, TEXT("Hey its not getting called"))
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodging;
	if (Attributes && GameOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		GameOverlay->SetStaminaPercent(Attributes->GetStamina());
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Look = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(Look.X);
		AddControllerPitchInput(Look.Y);
	}
}
void AMainCharacter::Moveforward(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_UnOccupied) return;
	
	const FVector2D Movement = Value.Get<FVector2D>();

	const  FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Movement.Y);

	const FVector DirectionRight = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(DirectionRight, Movement.X);
}
void AMainCharacter::EPressed()
{

	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	ABigSword* OverlappingBigSword = Cast<ABigSword>(OverlappingItem);

	
	if (OverlappingWeapon)
	{
		// It's a weapon (could be regular or BigSword)
		if (OverlappingBigSword)
		{
			// Specific BigSword handling
			OverlappingBigSword->Equip(GetMesh(), FName("BigSword"), this, this);
			CharacterState = ECharacterStates::ECS_EquippedTwoHandedWeapons;// Or whatever state fits
			EquippedBigSword = OverlappingBigSword;  
			
		}
		else 
		{
			// Regular one-handed weapon
			OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
			CharacterState = ECharacterStates::ECS_EquippedOneHandWeapon;
			EquippedWeapon = OverlappingWeapon;
		
		}

		// Clear overlap reference (good practice!)
		OverlappingItem = nullptr;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterStates::ECS_UnEquipped;

			ActionState = EActionState::EAS_EquppingWeapon;


			
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterStates::ECS_EquippedOneHandWeapon;

			ActionState = EActionState::EAS_EquppingWeapon;
			
		}
	}
}
bool AMainCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_UnOccupied && CharacterState != ECharacterStates::ECS_UnEquipped;
	
}

bool AMainCharacter::CanArm()
{
	return CharacterState == ECharacterStates::ECS_UnEquipped && ActionState == EActionState::EAS_UnOccupied && EquippedWeapon;
}

void AMainCharacter::Attack()
{
	if (CanAttack() && CharacterState != ECharacterStates::ECS_EquippedTwoHandedWeapons)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	else if (CanAttack() && CharacterState != ECharacterStates::ECS_EquippedOneHandWeapon)
	{
		PlayBigSwordMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}
bool AMainCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_UnOccupied && CharacterState != ECharacterStates::ECS_UnEquipped;

}

void AMainCharacter::IntializeGameOverlay()
{
	APlayerController* PController = Cast<APlayerController>(GetController());
	if (PController)
	{
		AGameHUD* GameHud = Cast<AGameHUD>(PController->GetHUD());
		if (GameHud)
		{
		GameOverlay = GameHud->GetGameOverlay();
			if (GameOverlay)
			{
				GameOverlay->SetSoulsText(0);
				GameOverlay->SetGoldText(0);
				GameOverlay->SetHealthPercent(Attributes->ChangeProgressBar());
				GameOverlay->SetStaminaPercent(1.f);
			}
		}
	}
}


void AMainCharacter::PlayBigSwordMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(BigSwordMontage);
		const int32 Selection = FMath::RandRange(0, 4 );
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
			
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		case 3:
			SectionName = FName("Attack4");
			break;
		case 4:
			SectionName = FName("Attack5");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, BigSwordMontage);
	}
}

void AMainCharacter::PlayEquipMontage(FName EquipSectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(EquipSectionName, EquipMontage);


				
	}
}


void AMainCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AMainCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
	
}
void AMainCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

FVector AMainCharacter::GetEchoRotationWarpTarget()
{
	if (Enemy)
	{
		return Enemy->GetActorLocation();
	}
	return FVector();
}

FVector AMainCharacter::GetEchoTranslationTarget()
{
	/*if (Enemy == nullptr) return FVector(); 	UE_LOG(LogTemp, Warning, TEXT("mE"));
	const FVector TargetLocation = Enemy->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargettoMe = (Location - TargetLocation).GetSafeNormal();
	TargettoMe *= 75.f;
	UE_LOG(LogTemp, Warning, TEXT("TargettoMe"));
	return TargetLocation + TargettoMe;*/

	if (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is nullptr"));
		return FVector();
	}

	const FVector TargetLocation = Enemy->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargettoMe = (Location - TargetLocation).GetSafeNormal();
	TargettoMe *= 75.f;

	return TargetLocation + TargettoMe;
}

void AMainCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

void AMainCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AMainCharacter::FinishEqupping()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AMainCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attributes && GameOverlay)
	{
		Attributes->RegeStamina(DeltaTime);
		GameOverlay->SetStaminaPercent(Attributes->GetStamina());
	}

}

void AMainCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	
	if (Attributes && Attributes->ChangeProgressBar() > 0.f)
	{
		ActionState = EActionState::EAS_HitReacting;
	}
}

void AMainCharacter::ChangeHUD()
{
	if (GameOverlay && Attributes)
	{
		GameOverlay->SetHealthPercent(Attributes->ChangeProgressBar());
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                 class AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	ChangeHUD();
	return DamageAmount;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* MainInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		MainInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AMainCharacter::Moveforward);
		MainInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		MainInputComponent->BindAction((JumpAction), ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
		MainInputComponent->BindAction(EKeyPressed, ETriggerEvent::Triggered, this, &AMainCharacter::EPressed);
		MainInputComponent->BindAction(AttackInput, ETriggerEvent::Triggered, this, &AMainCharacter::Attack);
		MainInputComponent->BindAction(DodgeInput, ETriggerEvent::Triggered, this, &AMainCharacter::Dodge);
	}

}

void AMainCharacter::SetOverlappingItem(class AItem* Item)
{
	OverlappingItem = Item;
}

void AMainCharacter::AddSouls(class ASoul* Soul)
{
	if (Attributes && GameOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		GameOverlay->SetSoulsText(Attributes->GetSoulsCount());
	}
}

void AMainCharacter::AddGold(class ATreasure* Gold)
{
	if (Attributes && GameOverlay)
	{
		Attributes->AddGold(Gold->GetGold());
		GameOverlay->SetGoldText(Attributes->GetGoldCount());
	}
}

void AMainCharacter::Jump()
{
	if (ActionState == EActionState::EAS_UnOccupied)
	{
		Super::Jump();
	}
}



