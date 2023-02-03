#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
};

UENUM(BlueprintType)
enum class EActionState : uint8 {
	EAS_Unoccupied UMETA(DisplayName = "Unoccupid"),
	EAS_Attack01 UMETA(DisplayName = "Attack01"),
	EAS_Attack02 UMETA(DisplayName = "Attack02")
};