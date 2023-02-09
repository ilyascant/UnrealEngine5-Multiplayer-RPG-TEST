#include "Character/SlashAnimInstance.h"
#include "Character/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ActorComponent.h"


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

		if (TObjectPtr<AWeapon> EquippedWeapon = SlashCharacter->GetEquippedWeapon()) {
			ActionState = EquippedWeapon->GetWeaponActionState();
		}
	}
	
}

