#include "Character/SlashAnimNotifyState.h"
#include "Engine.h"
#include "Items/Weapons/Weapon.h"
#include "Character/SlashCharacter.h"


void USlashAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	SlashChr = Cast<ASlashCharacter>(MeshComp->GetOwner());
	if (SlashChr) 
		if (TObjectPtr<AWeapon> EquippedWeapon = SlashChr->GetEquippedWeapon()) {
			EquippedWeapon->SetbComboPerm(true);
			PrevActionState = EquippedWeapon->GetWeaponActionState();
		}
}

void USlashAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{

}

void USlashAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(SlashChr) 
		if (TObjectPtr<AWeapon> EquippedWeapon = SlashChr->GetEquippedWeapon()) {
			EquippedWeapon->SetbComboPerm(false);
			ActionState = EquippedWeapon->GetWeaponActionState();
			if (ActionState == PrevActionState) {
				EquippedWeapon->SetWeaponActionState(EActionState::EAS_Unoccupied);
			}
		}		
}


