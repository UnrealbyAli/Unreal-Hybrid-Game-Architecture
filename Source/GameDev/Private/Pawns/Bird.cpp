// Fill out your copyright notice in the Description page of Project Settings.
#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include  "Components/SkeletalMeshComponent.h"
#include  "GameFramework/SpringArmComponent.h"
#include  "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include  "Components/InputComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    BirdMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BirdMesh);
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}
void ABird::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	
	if (DirectionValue != 0.f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward , DirectionValue);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	const FVector2D Look = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(Look.X);
		AddControllerPitchInput(Look.Y);
	}}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}

	
}

