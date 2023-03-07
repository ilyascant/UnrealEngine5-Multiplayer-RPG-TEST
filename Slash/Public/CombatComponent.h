#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;
class AItem;
class AGunWeapon;
class ASlashCharacter;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	public:	
		UCombatComponent();
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		friend class ASlashCharacter;


	protected:
		virtual void BeginPlay() override;
		void EquipWeapon(AItem* EquipItem);
		void EquipWeapon(AWeapon* EquipItem);
		void EquipWeapon(AGunWeapon* EquipItem);
		void DropWeapon();
		void PlayEquipMontage(const FName Selection);

		void SetAiming(bool bIsAiming);
		UFUNCTION(Server, Reliable)
		void ServerSetAiming(bool bIsAiming);

		UFUNCTION()
		void OnRep_EquippedGunWeapon();	
		UFUNCTION()
		void OnRep_EquippedWeapon();

	private:
		// Post Initialized in SlashCharacter.cpp
		ASlashCharacter* Character;

		UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon, VisibleAnywhere, Category = "Weapon")
		TObjectPtr<AWeapon> EquippedWeapon;		
		UPROPERTY(ReplicatedUsing = OnRep_EquippedGunWeapon, VisibleAnywhere, Category = "Weapon")
		TObjectPtr<AGunWeapon> EquippedGunWeapon;

		bool CanDisarm();
		bool CanArm();
		void AttackWeapon();
		void AttackGunWeapon(float AttackValue);
		void AttackEnd();

		UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
		UFUNCTION(BlueprintCallable)
		void Disarm();
		UFUNCTION(BlueprintCallable)
		void Arm();
		UFUNCTION(BlueprintCallable)
		void ArmEnd();

		UPROPERTY(Replicated)
		bool bAiming;

		UPROPERTY(EditAnywhere)
		float BaseWalkSpeed;
		UPROPERTY(EditAnywhere)
		float AimWalkSpeed;



		UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TObjectPtr<UAnimMontage> EquipMontage;
		
};
