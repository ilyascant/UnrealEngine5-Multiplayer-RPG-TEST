#include "CombatComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Weapons/GunWeapon.h"
#include "Character/SlashCharacter.h"
#include "Animation/AnimMontage.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include "Slash/DebugMacros.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character) Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, EquippedGunWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
}

void UCombatComponent::EquipWeapon(AItem* EquipItem)
{
	if (!EquipItem) {
		if (CanDisarm()) {
			PlayEquipMontage(FName("Unequip"));
			Character->SetCharacterState(ECharacterState::ECS_Unequipped);
		}
		else if (CanArm()) {
			PlayEquipMontage(FName("Equip"));
			Character->SetCharacterState(ECharacterState::ECS_EquippedOneHandedWeapon);
		}
		return;
	};

	switch (EquipItem->GetItemType()) {
		case EItemType::EIT_Sword:
		{
			EquippedWeapon = Cast<AWeapon>(EquipItem);
			EquipWeapon(EquippedWeapon);
			break;
		}
		case EItemType::EIT_Gun:
		{
			EquippedGunWeapon = Cast<AGunWeapon>(EquipItem);
			EquipWeapon(EquippedGunWeapon);
			break;
		}
		default:
			break;
	}	
}

void UCombatComponent::EquipWeapon(AWeapon* EquipItem) {

	//If already have EquippedWeapon or EquippedGunWeapon drop it.
	DropWeapon();

	EquippedWeapon->Equip(Character->GetMesh(), FName("RightHandSocket"));
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->SetPickUpWidgetVisiblity(false);
	EquippedWeapon->SetItemState(EItemState::EIS_Equiped);

	Character->SetCharacterState(ECharacterState::ECS_EquippedOneHandedWeapon);
}

void UCombatComponent::EquipWeapon(AGunWeapon* EquipItem)
{
	//If already have EquippedWeapon or EquippedGunWeapon drop it.
	DropWeapon();

	EquippedGunWeapon->Equip(Character->GetMesh(), FName("RightHandGunSocket"));
	EquippedGunWeapon->SetOwner(Character);
	EquippedGunWeapon->SetPickUpWidgetVisiblity(false);
	EquippedGunWeapon->SetItemState(EItemState::EIS_Equiped);

	Character->SetCharacterState(ECharacterState::ECS_EquippedOneHandedWeapon);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}

void UCombatComponent::DropWeapon()
{
	if (EquippedWeapon && CanDisarm()) {
		EquippedWeapon->SetOwner(nullptr);
		EquippedWeapon->SetPickUpWidgetVisiblity(false);
		EquippedWeapon->Drop(Character->GetActorLocation(), Character->GetActorForwardVector());
		Character->SetCharacterState(ECharacterState::ECS_Unequipped);
		EquippedWeapon = nullptr;
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->bUseControllerRotationYaw = false;
	}
	else if (EquippedGunWeapon && CanDisarm()) {
		EquippedGunWeapon->SetOwner(nullptr);
		EquippedGunWeapon->SetPickUpWidgetVisiblity(false);
		EquippedGunWeapon->Drop(Character->GetActorLocation(), Character->GetActorForwardVector());
		Character->SetCharacterState(ECharacterState::ECS_Unequipped);
		EquippedGunWeapon = nullptr;
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->bUseControllerRotationYaw = false;
	}
}

bool UCombatComponent::CanDisarm() {
	if (EquippedWeapon)
		return (EquippedWeapon->GetItemState() == EItemState::EIS_Equiped &&
				EquippedWeapon->GetWeaponActionState() == EActionState::EAS_Unoccupied &&
				Character->GetCharacterState() != ECharacterState::ECS_Unequipped);
	else if (EquippedGunWeapon)
		return (EquippedGunWeapon->GetItemState() == EItemState::EIS_Equiped &&
				Character->GetCharacterState() != ECharacterState::ECS_Unequipped);
	return false;
}

bool UCombatComponent::CanArm() {
	if (EquippedWeapon)
		return (EquippedWeapon->GetItemState() == EItemState::EIS_Equiped &&
				EquippedWeapon->GetWeaponActionState() == EActionState::EAS_Unoccupied &&
				Character->GetCharacterState() == ECharacterState::ECS_Unequipped);
	else if (EquippedGunWeapon)
		return (EquippedGunWeapon->GetItemState() == EItemState::EIS_Equiped &&
				Character->GetCharacterState() == ECharacterState::ECS_Unequipped);
	return false;
}


void UCombatComponent::PlayEquipMontage(const FName Selection) {
	TObjectPtr<UAnimInstance> AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {
		if (EquippedWeapon) EquippedWeapon->SetItemState(EItemState::EIS_Equipping);
		else if (EquippedGunWeapon) EquippedGunWeapon->SetItemState(EItemState::EIS_Equipping);
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(Selection, EquipMontage);
	}

}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (Character) Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;

}


void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character) Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
}

void UCombatComponent::Disarm()
{
	if (EquippedWeapon) {
		EquippedWeapon->SetItemState(EItemState::EIS_Equiped);
		EquippedWeapon->AttachMeshToSocket(Character->GetMesh(), FName("WeaponSocket"));
	}
	else if (EquippedGunWeapon) {
		EquippedGunWeapon->SetItemState(EItemState::EIS_Equiped);
		EquippedGunWeapon->AttachMeshToSocket(Character->GetMesh(), FName("GunSocket"));
	}
}

void UCombatComponent::OnRep_EquippedGunWeapon()
{
	if (EquippedGunWeapon && Character) {
		//If already have EquippedWeapon or EquippedGunWeapon drop it.
		DropWeapon();

		EquippedGunWeapon->Equip(Character->GetMesh(), FName("RightHandGunSocket"));
		EquippedGunWeapon->SetPickUpWidgetVisiblity(false);
		EquippedGunWeapon->SetItemState(EItemState::EIS_Equiped);

		Character->SetCharacterState(ECharacterState::ECS_EquippedOneHandedWeapon);
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		//If already have EquippedWeapon or EquippedGunWeapon drop it.
		DropWeapon();

		EquippedWeapon->Equip(Character->GetMesh(), FName("RightHandSocket"));
		EquippedWeapon->SetPickUpWidgetVisiblity(false);
		EquippedWeapon->SetItemState(EItemState::EIS_Equiped);

		Character->SetCharacterState(ECharacterState::ECS_EquippedOneHandedWeapon);
	}
}

void UCombatComponent::Arm()
{
	if (EquippedWeapon) EquippedWeapon->AttachMeshToSocket(Character->GetMesh(), FName("RightHandSocket"));
	else if (EquippedGunWeapon) EquippedGunWeapon->AttachMeshToSocket(Character->GetMesh(), FName("RightHandGunSocket"));

}
void UCombatComponent::ArmEnd()
{
	if (EquippedWeapon) EquippedWeapon->SetItemState(EItemState::EIS_Equiped);
	else if (EquippedGunWeapon) EquippedGunWeapon->SetItemState(EItemState::EIS_Equiped);
}


/**
*	CHARACTER COMBAT
*	Since Combat depands on Weapon selection
*	Character class call Weapon class for Attack options
*	Each Weapon have their own unique combat options
*/

void UCombatComponent::AttackWeapon()
{
	if(EquippedWeapon)
		EquippedWeapon->Attack();
}

void UCombatComponent::AttackGunWeapon(float AttackValue)
{
	if (EquippedGunWeapon && AttackValue > 0)
	{
		if(Character){
			Character->SetIsFiring(true);
			Character->PlayFireMontage(bAiming);
			//EquippedGunWeapon->Attack();
		}
	}
	else {
		if (Character) Character->SetIsFiring(false);
	}
}

void UCombatComponent::AttackEnd()
{

	if (EquippedWeapon)
		EquippedWeapon->SetWeaponActionState(EActionState::EAS_Unoccupied);
}


void UCombatComponent::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon)
		EquippedWeapon->SetWeaponCollision(CollisionEnabled);
}