#include "Character/SlashAnimNotifyState.h"
#include "Engine.h"
#include "Character/SlashCharacter.h"


void USlashAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	SlashChr = Cast<ASlashCharacter>(MeshComp->GetOwner());
	if (SlashChr) {
		SlashChr->SetbComboPerm(true);
		PrevActionState = SlashChr->GetActionState();
	}
}

void USlashAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{

}

void USlashAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(SlashChr) 
	{
		SlashChr->SetbComboPerm(false);
		ActionState = SlashChr->GetActionState();

		if (ActionState == PrevActionState) {
			SlashChr->SetActionState(EActionState::EAS_Unoccupied);
		}
		
	}
}


