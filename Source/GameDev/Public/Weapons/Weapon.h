// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class GAMEDEV_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	virtual void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	TArray<AActor*> IgnoredActors;
	
protected:
	virtual void BeginPlay() override;
	void PlayEquipSound();
	void DisbaleSphereNoCollision();
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	
private:
	void BoxTrace(FHitResult& BoxHit);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapons Properties")
	bool bIsShowDebug = false;
	
	UPROPERTY(EditAnywhere, Category = "SOUND", meta = (AllowPrivateAccess = "true"))
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	float Damage = 20;


public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return BoxComponent; }
};
