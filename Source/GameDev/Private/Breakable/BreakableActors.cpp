// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActors.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Treasure/Treasure.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABreakableActors::ABreakableActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollection");
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);

	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(RootComponent);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}
void ABreakableActors::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABreakableActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActors::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;
	bBroken = true;
	UWorld* World = GetWorld();
	
	if (World && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 50.f;
		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
	
}

