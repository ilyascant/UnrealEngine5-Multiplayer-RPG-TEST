#include "Character/SlashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Weapons/GunWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Slash/DebugMacros.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Net/UnrealNetwork.h"


ASlashCharacter::ASlashCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	TObjectPtr<UCharacterMovementComponent> CharMov = GetCharacterMovement();
	CharMov->bOrientRotationToMovement = true;
	CharMov->RotationRate = FRotator(0.f, 360.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetRootComponent());

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

}


void ASlashCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->Character = this;
	}
}

void ASlashCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASlashCharacter, OverlappingItem, COND_OwnerOnly);

}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	ZoomFactor = 0;

	if (TObjectPtr<APlayerController> PC = Cast<APlayerController>(GetController())) {
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			Subsystem->AddMappingContext(SlashCharacterMappingContext, 0);
	}
	
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
}


/**
*	CHARACTER MOVEMENT
*/

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller && MovementVector.IsZero() != true){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller && LookAxisValue.IsZero() != true) {
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);

	}
}

void ASlashCharacter::Zoom(const FInputActionValue& Value)
{

	const float FloatValue = Value.Get<float>();
	ZoomFactor += FloatValue * 4;
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.f, 1.0f);

	if (Controller)
		ViewCamera->SetFieldOfView(FMath::Lerp(90.f, 60.f, ZoomFactor));

}

/**
*	CHARACTER INTERACTION
*/

void ASlashCharacter::Equip(const FInputActionValue& Value)
{
	if (Controller) {
		EquipWeapon(OverlappingItem);
	}
}

void ASlashCharacter::EquipWeapon(AItem* EquipItem) {
	if (HasAuthority()) {
		CombatComponent->EquipWeapon(EquipItem);
	}
	else {
		ServerEquipButtonPressed();
	}
}

void ASlashCharacter::Drop(const FInputActionValue& Value)
{
	if (Controller) {
		DropWeapon();
	}
}

void ASlashCharacter::DropWeapon()
{
	if (HasAuthority()) {
		CombatComponent->DropWeapon();
	}
	else {
		ServerDropButtonPressed();
	}
}

void ASlashCharacter::CrouchPressed(const FInputActionValue& Value)
{
	float crouchValue = Value.Get<float>();
	if (Controller) {
		crouchValue > 0 ? Crouch() : UnCrouch();
		
	}
}

void ASlashCharacter::AimPressed(const FInputActionValue& Value)
{
	float aimValue = Value.Get<float>();
	if (Controller && CombatComponent) {
		aimValue > 0 ? CombatComponent->SetAiming(true) : CombatComponent->SetAiming(false);

	}
}

void ASlashCharacter::Attack(const FInputActionValue& Value) {
	if (Controller && CombatComponent->EquippedWeapon) AttackWeapon();
}

void ASlashCharacter::AttackWeapon() 
{
	if (HasAuthority()) {
		CombatComponent->AttackWeapon();
	}
	else {
		ServerAttackButtonPressed();
	}
}

void ASlashCharacter::AimOffset(float Delta)
{
	if (CombatComponent && CombatComponent->EquippedGunWeapon == nullptr) return;
	FVector Velocity = this->GetVelocity();
	float GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
	bool bIsInAir = this->GetCharacterMovement()->IsFalling();

	if (GroundSpeed == 0.f && !bIsInAir) // Standing still, not jumping
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		bUseControllerRotationYaw = false;

	}
	if (GroundSpeed > 0.f || bIsInAir) // Running or Jumpping
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if (!IsLocallyControlled() && AO_Pitch > 90.f)
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 370.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

/**
*	CHARACTER INPUT SETUP
*/


void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Zoom);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Drop);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ASlashCharacter::CrouchPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ASlashCharacter::AimPressed);
	}

}

/**
*	REPLICATION
*/


TObjectPtr<AWeapon> ASlashCharacter::GetEquippedWeapon()
{
	return CombatComponent->EquippedWeapon;
}

void ASlashCharacter::SetEquippedWeapon(TObjectPtr<AWeapon> Weapon)
{
	CombatComponent->EquippedWeapon = Weapon;
}

TObjectPtr<AGunWeapon> ASlashCharacter::GetEquippedGunWeapon()
{
	return CombatComponent->EquippedGunWeapon;
}

void ASlashCharacter::SetEquippedGunWeapon(TObjectPtr<AGunWeapon> Weapon)
{
	CombatComponent->EquippedGunWeapon = Weapon;
}

void ASlashCharacter::SetOverlappingItem(AItem* Item) {
	if (OverlappingItem) {
		OverlappingItem->SetPickUpWidgetVisiblity(false);
	}
	OverlappingItem = Item;
	if (IsLocallyControlled()) {
		if (OverlappingItem) {
			OverlappingItem->SetPickUpWidgetVisiblity(true);
		}
	}
}

void ASlashCharacter::OnRep_OverlappingItem(AItem* LastOverlappingWeapon)
{
	if (OverlappingItem) {
		OverlappingItem->SetPickUpWidgetVisiblity(true);
	}
	else if (LastOverlappingWeapon) {
		LastOverlappingWeapon->SetPickUpWidgetVisiblity(false);
	}
}

void ASlashCharacter::ServerEquipButtonPressed_Implementation()
{
	if(CombatComponent)
		CombatComponent->EquipWeapon(OverlappingItem);
}

void ASlashCharacter::ServerDropButtonPressed_Implementation()
{
	if (CombatComponent)
		CombatComponent->DropWeapon();
}
void ASlashCharacter::ServerAttackButtonPressed_Implementation()
{
	if (CombatComponent)
		CombatComponent->AttackWeapon();
}

bool ASlashCharacter::IsAiming() {
		return CombatComponent && CombatComponent->bAiming;
}
