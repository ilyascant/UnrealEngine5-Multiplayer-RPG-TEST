#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "InputAction.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

	public:
		AWeapon();
		
		virtual void Equip(USceneComponent* InParent, const FName& InSocketName) override;
		virtual void Drop(const FVector& PlayerLocation, const FVector& PlayerForward) override;

		bool CanAttack();
		void Attack();
		void PlayAttackMontage(const FName& SectionName);

		bool AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName);
		bool DetachFromComponent(TObjectPtr<USceneComponent>& InParent, const FName& InSocketName);
		void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		TArray<TObjectPtr<AActor>> IgnoreActors;

	protected:
		virtual void BeginPlay() override;

		virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

		UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
		TObjectPtr<USceneComponent> BoxTraceStart;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties")
		TObjectPtr<USceneComponent> BoxTraceEnd;

	private:
		UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		TObjectPtr<UBoxComponent> WeaponBoxComp;

		TObjectPtr<class ASlashCharacter> SlashCharacter;
		UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TObjectPtr<UAnimMontage> AttackMontage;
		EActionState WeaponActionState = EActionState::EAS_Unoccupied;
		bool bComboPerm = false;


	public:
		FORCEINLINE TObjectPtr<UBoxComponent> GetWeaponBox() const { return WeaponBoxComp; };
		FORCEINLINE EActionState GetWeaponActionState() { return WeaponActionState; }
		FORCEINLINE void SetWeaponActionState(const EActionState State) { WeaponActionState = State; }
		FORCEINLINE bool GetbComboPerm() { return bComboPerm; }
		FORCEINLINE void SetbComboPerm(const bool Perm) { bComboPerm = Perm; }	
	
};
