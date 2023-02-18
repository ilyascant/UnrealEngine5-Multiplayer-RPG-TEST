#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "InputAction.h"
#include "CharacterTypes.h"
#include "../Items/Weapons/WeaponTypes.h"

#include "SlashAnimInstance.generated.h"

UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


	public:
		
		virtual void NativeInitializeAnimation() override;
		virtual void NativeUpdateAnimation(float DeltaTime) override;

		UPROPERTY(BlueprintReadOnly)
		TObjectPtr<class ASlashCharacter> SlashCharacter;

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		TObjectPtr<class UCharacterMovementComponent> SlashCharacterMovement;
		
		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float GroundSpeed;
		
		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsAccelerating;
		
		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsFalling;

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsCrouching;		
		
		UPROPERTY(BlueprintReadOnly, Category = "Equipment")
		bool bWeaponEquipped;
		
		UPROPERTY(BlueprintReadOnly, Category = "Equipment")
		bool bGunEquipped;
		
		UPROPERTY(BlueprintReadOnly, Category = "Equipment")
		bool bIsAiming;		
		
		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float YawOffset;		
		
		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float Lean;

		FRotator CharacterRotationLastFrame;
		FRotator CharacterRotation;
		FRotator DeltaRotation;

		UPROPERTY(BlueprintReadOnly, Category = "Movement|Character State")
		ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
		UPROPERTY(BlueprintReadOnly, Category = "Movement|Action State")
		EActionState ActionState = EActionState::EAS_Unoccupied;

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float AO_Yaw;

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float AO_Pitch;

};
