// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include  "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Implicit/SDFCalculationUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(FName("StartBoxTrace"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(FName("EndBoxTrace"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
	}
}

void AWeapon::DisbaleSphereNoCollision()
{
	if(SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemStates::EIS_NotHovering;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	PlayEquipSound();
	DisbaleSphereNoCollision();
	if (Particles )Particles->Deactivate();
}
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);

	FAttachmentTransformRules TransformRules2(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules2, InSocketName);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"))) return;
	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && BoxHit.GetActor()->ActorHasTag(TEXT("Enemy"))) return;
		
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
		}
		CreateFields(BoxHit.ImpactPoint);

	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> Actorstoignore;
	Actorstoignore.Add(this);
	Actorstoignore.AddUnique(GetOwner());
	for(AActor* Actor : IgnoredActors)
	{
		Actorstoignore.AddUnique(Actor);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
	this,
	Start,
	End,
	BoxExtent,
	BoxTraceStart->GetComponentRotation(),
	ETraceTypeQuery::TraceTypeQuery1,
	false,
	Actorstoignore,
	bIsShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
	BoxHit,
	true
 );
	IgnoredActors.AddUnique(BoxHit.GetActor());
}
