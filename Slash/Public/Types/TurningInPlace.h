#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_LEFT UMETA(DisplayName = "Turning Left"),
	ETIP_RIGHT UMETA(DisplayName = "Turning Right"),
	ETIP_NOT_TURNING UMETA(DisplayName = "Turning Not Turning"),

	ETIP_MAX UMETA(DisplayName = "DefaultMAX"),
};