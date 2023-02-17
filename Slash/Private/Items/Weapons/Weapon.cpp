#include "Items/Weapons/Weapon.h"
#include "Character/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Animation/AnimMontage.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"


AWeapon::AWeapon() {
	ItemName = FName(TEXT("Sword"));
	ItemType = EItemType::EIT_Sword;

	WeaponBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX COMPONENT"));
	WeaponBoxComp->SetupAttachment(GetRootComponent());
	WeaponBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//WeaponBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	AItem::BeginPlay();
	WeaponBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	AItem::GetLifetimeReplicatedProps(OutLifetimeProps);
}

/**
*	OVERLAP DELEGATES
*/

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItem::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex,bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AItem::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(this->GetAttachParentActor())
	{
		const FVector Start = BoxTraceStart->GetComponentLocation();
		const FVector End = BoxTraceEnd->GetComponentLocation();

		TArray<TObjectPtr<AActor>> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		ActorsToIgnore.Add(this->GetAttachParentActor());

		for (TObjectPtr<AActor> Actor: IgnoreActors) {
			ActorsToIgnore.AddUnique(Actor);
		}

		FHitResult BoxHit;

		bool bTraceHit = UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			FVector(5.f, 5.f, 5.f),
			BoxTraceStart->GetComponentRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			BoxHit,
			true
		);

		if (BoxHit.GetActor()) {
			TObjectPtr<IHitInterface> HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
			if (HitInterface) {
				HitInterface->GetHit(BoxHit.ImpactPoint);
			}
			IgnoreActors.AddUnique(BoxHit.GetActor());
		}
	}

}

/**
*	WEAPON INTERACTIONS
*/

void AWeapon::Equip(USceneComponent* InParent, const FName& InSocketName) {
	AItem::Equip(InParent, InSocketName);
}


bool AWeapon::AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName)
{
	return AItem::AttachMeshToSocket(InParent, InSocketName);
}


void AWeapon::Drop(const FVector& PlayerLocation, const FVector& PlayerForward){
	AItem::Drop(PlayerLocation, PlayerForward);

}

bool AWeapon::DetachFromComponent(TObjectPtr<USceneComponent>& InParent, const FName& InSocketName)
{
	return AItem::DetachFromComponent(InParent, InSocketName);

}


/**
*	WEAPON COMBAT
*	Since every weapon has unique combat options
*	Function is implemanted in Weapon Class
*/


bool AWeapon::CanAttack()
{
	/**
	* bComboPerm USED FOR DETERMINING IF IT IS IN COMBO STAGE
	* IF IN COMBO STAGE ALWAYS RETURN TRUE
	*/
	
	return  (this->GetbComboPerm() ||
			(WeaponActionState == EActionState::EAS_Unoccupied
				&& SlashCharacter->GetCharacterState() != ECharacterState::ECS_Unequipped
				&& this->GetItemState() == EItemState::EIS_Equiped));
}

void AWeapon::Attack()
{
	SlashCharacter = Cast<ASlashCharacter>(this->GetAttachParentActor());
	if (!SlashCharacter || !CanAttack()) return;

	/**
	* SELECTS NEXT ATTACK DEFAULT EActionState::EAS_Unoccupied
	*/
	FName SectionName = FName();
	switch (WeaponActionState) {
	case EActionState::EAS_Attack01:
		WeaponActionState = EActionState::EAS_Attack02;
		SectionName = FName("Attack02");
		break;
	case EActionState::EAS_Attack02:
		WeaponActionState = EActionState::EAS_Attack01;
		SectionName = FName("Attack01");
		break;
	case EActionState::EAS_Unoccupied:
		WeaponActionState = EActionState::EAS_Attack01;
		SectionName = FName("Attack01");
		break;
	default:
		return;
		break;
	}

	/**
	* PLAYS THE MONTAGE THEN JUMPS TO SECTION NAME MONTAGE
	* NEXT ACTION SECTION IS DETERMINED IN ANIM NOTIFY STATE
	*/
	PlayAttackMontage(SectionName);
}
void AWeapon::PlayAttackMontage(const FName& SectionName)
{
	TObjectPtr<UAnimInstance> AnimInstance = SlashCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage) {
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AWeapon::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (this->GetWeaponBox()) {
		this->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		this->IgnoreActors.Empty();
	}
}

