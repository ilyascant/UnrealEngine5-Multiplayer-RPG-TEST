#include "Character/SlashAnimInstance.h"
#include "Character/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"
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
		GroundSpeed =  UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
		bIsFalling = SlashCharacterMovement->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState();
		
		if (TObjectPtr<AWeapon> EquippedWeapon = SlashCharacter->GetEquippedWeapon()) {
			ActionState = SlashCharacter->GetEquippedWeapon()->GetWeaponActionState();
		}
	}
}

