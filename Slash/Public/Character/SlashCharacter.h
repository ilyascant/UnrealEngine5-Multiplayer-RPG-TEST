#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "CharacterTypes.h"
#include "Slash/Public/Types/TurningInPlace.h"

#include "SlashCharacter.generated.h"


class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class AWeapon;
class AGunWeapon;
class UAnimMontage;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MovementAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> DropAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> SlashCharacterMappingContext;


	/**
		Combat
	*/
	void EquipWeapon(AItem* EquipItem);
	void DropWeapon();
	void AttackWeapon();
	void AimOffset(float Delta);


	/**
		COMPONENTS
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCombatComponent> CombatComponent;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;
	UPROPERTY(VisibleAnywhere, Category = "Hair")
	TObjectPtr<UGroomComponent> Hair;
	UPROPERTY(VisibleAnywhere, Category = "Hair")
	TObjectPtr<UGroomComponent> Eyebrows;

	float ZoomFactor ;
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	/**
		Callbacks For Input
	*/

	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	virtual void Zoom(const FInputActionValue& Value);
	virtual void Equip(const FInputActionValue& Value);
	virtual void Attack(const FInputActionValue& Value);
	virtual void Drop(const FInputActionValue& Value);
	virtual void CrouchPressed(const FInputActionValue& Value);
	virtual void AimPressed(const FInputActionValue& Value);

	/**
		 Animation Montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> EquipMontage;


	/**
		Widget
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverheadWidget;


	/**
		REPLICATION
	*/

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingItem, VisibleInstanceOnly)
	AItem* OverlappingItem;
	UFUNCTION()
	void OnRep_OverlappingItem(AItem* LastOverlappingWeapon);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	UFUNCTION(Server, Reliable)
	void ServerDropButtonPressed();	
	UFUNCTION(Server, Reliable)
	void ServerAttackButtonPressed();


public:
	virtual void SetOverlappingItem(AItem* Item);
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetCharacterState(ECharacterState State) { CharacterState = State; }
	TObjectPtr<AWeapon> GetEquippedWeapon();
	void SetEquippedWeapon(TObjectPtr<AWeapon> Weapon);	
	TObjectPtr<AGunWeapon> GetEquippedGunWeapon();
	void SetEquippedGunWeapon(TObjectPtr<AGunWeapon> Weapon);
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }


};
