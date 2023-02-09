#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;
class AItem;
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
		void DropWeapon();
		void PlayEquipMontage(const FName Selection);

	private:
		ASlashCharacter* Character;
		bool CanDisarm();
		bool CanArm();
		void AttackWeapon();
		void AttackEnd();

		UFUNCTION(BlueprintCallable)
		void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
		UFUNCTION(BlueprintCallable)
		void Disarm();
		UFUNCTION(BlueprintCallable)
		void Arm();
		UFUNCTION(BlueprintCallable)
		void ArmEnd();


		UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TObjectPtr<UAnimMontage> EquipMontage;
		
};
