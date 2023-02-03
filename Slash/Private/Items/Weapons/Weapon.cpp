#include "Items/Weapons/Weapon.h"
#include "Character/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Interfaces/HitInterface.h>


AWeapon::AWeapon() {
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

	Super::BeginPlay();

	WeaponBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

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

void AWeapon::Equip(USceneComponent* InParent, const FName& InSocketName) {
	AttachMeshToSocket(InParent, InSocketName);
}

void AWeapon::Drop(const FVector& PlayerLocation, const FVector& PlayerForward){
	const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
	this->DetachFromActor(DetachmentRules);
	this->ItemState = EItemState::EIS_Hovering;

	FHitResult Hit;
	FVector StartLoc = PlayerLocation + FVector(PlayerForward.X * 200, PlayerForward.Y * 200, 150.f);
	FVector EndLoc = StartLoc + FVector(0.f, 0.f, -500.f);
	FRotator Rotation = FRotator(0.f, 0.f, 0.f);

	TObjectPtr<UWorld> World = this->GetWorld();
	if (World) {
		GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECC_Visibility);
		if (Hit.bBlockingHit) {
			this->SetActorLocation(FVector(Hit.Location.X, Hit.Location.Y, Hit.Location.Z + 150.f));
			this->SetActorRotation(Rotation);
		}
	}
}

bool AWeapon::AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName)
{
	if (AItem::Item) {
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		AItem::Item->AttachToComponent(InParent, TransformRules, InSocketName);
		return true;
	}
	return false;
}

bool AWeapon::DetachFromComponent(TObjectPtr<USceneComponent>& InParent, const FName& InSocketName)
{
	if (AItem::Item) {
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		AItem::Item->AttachToComponent(InParent, TransformRules, InSocketName);
		return true;
	}
	return false;
}


