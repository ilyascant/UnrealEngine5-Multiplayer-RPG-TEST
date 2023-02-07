#pragma once

UENUM(BlueprintType)
enum class EActionState : uint8 {
	EAS_Unoccupied UMETA(DisplayName = "Unoccupid"),
	EAS_Attack01 UMETA(DisplayName = "Attack01"),
	EAS_Attack02 UMETA(DisplayName = "Attack02")
};