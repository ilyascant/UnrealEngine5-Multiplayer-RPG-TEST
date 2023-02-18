#include "Character/SlashAnimInstance.h"
#include "Character/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Weapons/GunWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ActorComponent.h"
#include <Slash/DebugMacros.h>
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner())) {
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SlashCharacterMovement) {
		bIsFalling = SlashCharacterMovement->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState();
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
		bIsCrouching = SlashCharacter->bIsCrouched;

		bIsAccelerating = SlashCharacterMovement->GetCurrentAcceleration().Size() > 0.f ? true : false;
		bWeaponEquipped = false;
		bGunEquipped = false;
		bIsAiming = SlashCharacter->IsAiming();

		if (TObjectPtr<AWeapon> EquippedWeapon = SlashCharacter->GetEquippedWeapon()) {
			ActionState = EquippedWeapon->GetWeaponActionState();
			if (SlashCharacter->GetCharacterState() != ECharacterState::ECS_Unequipped) bWeaponEquipped = true;
		}
		if (TObjectPtr<AGunWeapon> EquippedWeapon = SlashCharacter->GetEquippedGunWeapon()) {
			if(SlashCharacter->GetCharacterState() != ECharacterState::ECS_Unequipped) bGunEquipped = true;
		}

		// Offset Yaw for Strafing
		FRotator AimRotation = SlashCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(SlashCharacter->GetVelocity());
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
		DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
		YawOffset = FMath::Clamp(DeltaRotation.Yaw, -180.f, 180.f);

		CharacterRotationLastFrame = CharacterRotation;
		CharacterRotation = SlashCharacter->GetActorRotation();
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotationLastFrame, CharacterRotation);
		const float Target = Delta.Yaw / DeltaTime;
		const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
		Lean = FMath::Clamp(Interp, -180.f, 180.f);

		AO_Yaw = SlashCharacter->GetAO_Yaw();
		AO_Pitch = SlashCharacter->GetAO_Pitch();
	}
	
}

