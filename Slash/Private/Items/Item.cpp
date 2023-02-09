#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Character/SlashCharacter.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"


AItem::AItem()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(Item);

	SphereArea = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	SphereArea->SetupAttachment(RootComponent);
	SphereArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());
	PickupWidget->SetVisibility(false);
	
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		SphereArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SphereArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		SphereArea->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
		SphereArea->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	}
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemState == EItemState::EIS_Hovering) {
		RunningTime = RunningTime + DeltaTime;
		FloatingItem(RunningTime, Amplitude, TimeConstant);
	}
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, ItemState);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TObjectPtr<ASlashCharacter> SlashChar = Cast<ASlashCharacter>(OtherActor)) {
		SlashChar->SetOverlappingItem(this);
	}
	
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TObjectPtr<ASlashCharacter> SlashChar = Cast<ASlashCharacter>(OtherActor)) {
		SlashChar->SetOverlappingItem(nullptr);
	}

}

void AItem::Equip(USceneComponent* InParent, const FName& InSocketName)
{
}


void AItem::Drop(const FVector& PlayerLocation, const FVector& PlayerForward) {

}

void AItem::FloatingItem(float& _Time, const float& _Amplitude, const float& _TimeConstant)
{
	float Sine = _Amplitude * FMath::Sin(_Time * _TimeConstant);
	AddActorWorldOffset(FVector(0.f, 0.f, Sine));

	if (_Time * _TimeConstant >= 2 * PI) _Time = 0;

	
}

/**
	REPLICATION
*/

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	switch (ItemState)
	{
	case EItemState::EIS_Equiped:
		SetPickUpWidgetVisiblity(false);
		SphereArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AItem::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::EIS_Equiped:
		SetPickUpWidgetVisiblity(false);
		break;
	}
}

void AItem::SetPickUpWidgetVisiblity(const bool setValue)
{
	if (PickupWidget) {
		PickupWidget->SetVisibility(setValue);
	}
}


