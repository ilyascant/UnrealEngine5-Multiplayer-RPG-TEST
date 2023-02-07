#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../Items/Weapons/WeaponTypes.h"
#include "SlashAnimNotifyState.generated.h"

UCLASS()
class SLASH_API USlashAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TObjectPtr<class ASlashCharacter> SlashChr;
	EActionState NextActionState = EActionState::EAS_Unoccupied;
	EActionState PrevActionState = EActionState::EAS_Unoccupied;
	EActionState ActionState = EActionState::EAS_Unoccupied;
	bool bComboPerm;
	bool triggered;

};
