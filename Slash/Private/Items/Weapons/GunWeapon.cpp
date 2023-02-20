#include "Items/Weapons/GunWeapon.h"

AGunWeapon::AGunWeapon() {
	ItemName = FName(TEXT("Gun"));
	ItemType = EItemType::EIT_Gun;

	GunWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunWeaponMesh"));
	SetRootComponent(GunWeaponMesh);
}

void AGunWeapon::BeginPlay() 
{
	AItem::BeginPlay();
}

void AGunWeapon::Tick(float DeltaTime)
{
	AItem::Tick(DeltaTime);
}

void AGunWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	AItem::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGunWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItem::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AGunWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AItem::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AGunWeapon::Equip(USceneComponent* InParent, const FName& InSocketName)
{
	AItem::Equip(InParent, InSocketName);
}

void AGunWeapon::Drop(const FVector& PlayerLocation, const FVector& PlayerForward)
{
	AItem::Drop(PlayerLocation, PlayerForward);
}

bool AGunWeapon::AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName)
{
	return AItem::AttachMeshToSocket(InParent, InSocketName);
}

bool AGunWeapon::DetachFromComponent(TObjectPtr<USceneComponent>& InParent, const FName& InSocketName)
{
	return AItem::DetachFromComponent(InParent, InSocketName);
}
