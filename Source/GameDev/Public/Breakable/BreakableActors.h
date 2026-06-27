// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActors.generated.h"

UCLASS()
class GAMEDEV_API ABreakableActors : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActors();
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;
private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere, Category = "Treasure")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	bool bBroken = false;

};
