#include "CombatComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Character/SlashCharacter.h"
#include "Animation/AnimMontage.h"
#include "Engine/SkeletalMeshSocket.h"
#include <Slash/DebugMacros.h>

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UCombatComponent::EquipWeapon(AItem* EquipItem)
{
	TObjectPtr<AWeapon> OverlappingWeapon = Cast<AWeapon>(EquipItem);
	AWeapon* EquippedWeapon = Character->GetEquippedWeapon();

	 //If already have EquippedWeapon drop it.
	if (EquippedWeapon && OverlappingWeapon) {
		EquippedWeapon->Drop(Character->GetActorLocation(), Character->GetActorForwardVector());
		EquippedWeapon = nullptr;
	};

	if (OverlappingWeapon) {
		OverlappingWeapon->Equip(Character->GetMesh(), FName("RightHandSocket"));
		OverlappingWeapon->SetOwner(Character);
		OverlappingWeapon->SetPickUpWidgetVisiblity(false);
		OverlappingWeapon->SetItemState(EItemState::EIS_Equiped);

		Character->SetEquippedWeapon(OverlappingWeapon);
		Character->SetCharacterState(ECharacterState::ECS_EquippedOneHandedWeapon);
	}
	else {
		if (CanDisarm()) {
			PlayEquipMontage(FName("Unequip"));
			Character->SetCharacterState(ECharacterState::ECS_Unequipped);
		}
		else if (CanArm()) {
			PlayEquipMontage(FName("Equip"));
			Character->SetCharacterState(ECharacterState::ECS_EquippedOneHandedWeapon);
		}
	}
}

void UCombatComponent::DropWeapon()
{
	AWeapon* EquippedWeapon = Character->GetEquippedWeapon();
	if (EquippedWeapon && CanDisarm()) {
		EquippedWeapon->SetOwner(nullptr);
		EquippedWeapon->SetPickUpWidgetVisiblity(false);
		EquippedWeapon->Drop(Character->GetActorLocation(), Character->GetActorForwardVector());
		Character->SetCharacterState(ECharacterState::ECS_Unequipped);
		EquippedWeapon = nullptr;
	}
}

bool UCombatComponent::CanDisarm() {
	AWeapon* EquippedWeapon = Character->GetEquippedWeapon();
	return (EquippedWeapon &&
		EquippedWeapon->GetItemState() == EItemState::EIS_Equiped &&
		EquippedWeapon->GetWeaponActionState() == EActionState::EAS_Unoccupied &&
		Character->GetCharacterState() != ECharacterState::ECS_Unequipped);
}

bool UCombatComponent::CanArm() {
	AWeapon* EquippedWeapon = Character->GetEquippedWeapon();
	return (EquippedWeapon &&
		EquippedWeapon->GetItemState() == EItemState::EIS_Equiped &&
		EquippedWeapon->GetWeaponActionState() == EActionState::EAS_Unoccupied &&
		Character->GetCharacterState() == ECharacterState::ECS_Unequipped);
}


void UCombatComponent::PlayEquipMontage(const FName Selection) {
	TObjectPtr<UAnimInstance> AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {
		AWeapon* EquippedWeapon = Character->GetEquippedWeapon();
		EquippedWeapon->SetItemState(EItemState::EIS_Equipping);
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(Selection, EquipMontage);
	}

}

void UCombatComponent::Disarm()
{
	AWeapon* EquippedWeapon = Character->GetEquippedWeapon();
	if (EquippedWeapon) {
		EquippedWeapon->SetItemState(EItemState::EIS_Equiped);
		EquippedWeapon->AttachMeshToSocket(Character->GetMesh(), FName("WeaponSocket"));
	}
}

void UCombatComponent::Arm()
{
	AWeapon* EquippedWeapon = Character->GetEquippedWeapon();
	if (EquippedWeapon) EquippedWeapon->AttachMeshToSocket(Character->GetMesh(), FName("RightHandSocket"));

}
void UCombatComponent::ArmEnd()
{
	AWeapon* EquippedWeapon = Character->GetEquippedWeapon();
	if (EquippedWeapon) EquippedWeapon->SetItemState(EItemState::EIS_Equiped);
}


/**
*	CHARACTER COMBAT
*	Since Combat depands on Weapon selection
*	Character class call Weapon class for Attack options
*	Each Weapon have their own unique combat options
*/

void UCombatComponent::AttackWeapon()
{
	if(AWeapon* EquippedWeapon = Character->GetEquippedWeapon())
		EquippedWeapon->Attack();
}

void UCombatComponent::AttackEnd()
{

	if (AWeapon* EquippedWeapon = Character->GetEquippedWeapon()) 
		EquippedWeapon->SetWeaponActionState(EActionState::EAS_Unoccupied);
}


void UCombatComponent::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (AWeapon* EquippedWeapon = Character->GetEquippedWeapon()) 
		EquippedWeapon->SetWeaponCollision(CollisionEnabled);
}