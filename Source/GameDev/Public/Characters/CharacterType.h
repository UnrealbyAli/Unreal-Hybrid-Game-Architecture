#pragma once

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOneHandWeapon UMETA(DisplayName = "Equipped One Hand"),
	ECS_EquippedTwoHandedWeapons UMETA(DisplayName = "Equipped Two Handed")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_UnOccupied UMETA(DisplayName = "UnOccupied"),
	EAS_HitReacting UMETA(DisplayName = "HitReact"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquppingWeapon UMETA(DisplayName = "EquppingWeapon"),
	EAS_Dodging UMETA(DisplayName = "Dodging"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum EDeathStates 
{
	EAS_Dead1 UMETA(DisplayName = "Dead1"),
	EAS_Dead2 UMETA(DisplayName = "Dead2"),
	EAS_Dead3 UMETA(DisplayName = "Dead3"),
	EAS_Dead4 UMETA(DisplayName = "Dead4"),
	EAS_Dead5 UMETA(DisplayName = "Dead5"),
	EAS_Dead6 UMETA(DisplayName = "Dead6"), 

	EDP_MAX	UMETA(DisplayName = "DefaultMAX")
	
};

UENUM(BlueprintType)
enum class EEnemyStates : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
};