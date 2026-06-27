#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

enum class EItemStates : uint8
{
	EIS_Hovering,
	EIS_NotHovering,
};

UCLASS()
class GAMEDEV_API AItem : public AActor
{
	GENERATED_BODY()
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 5;
                                      
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 20.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template<typename  T>
	T Avg(T First, T Second);

	UFUNCTION()
	virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSpheretEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SpawnPickupEffect();
	void PlayPickupSound();
	UPROPERTY( BlueprintReadOnly, EditAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	class USoundBase* PickupSound;
	
	EItemStates ItemState;
	
	UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, Category = "Niagara")
	class UNiagaraComponent* Particles;
private:
    UPROPERTY(VisibleAnywhere, Category = "Sine Parameters", meta = (AllowPrivateAccess = "true"))
	float RunningTime;

	UPROPERTY(EditAnywhere, Category = "Controls", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 45.f ; 

};

template <typename T>
inline T AItem::Avg(T First, T Second)
{
	return ((First + Second) / 2);
}
