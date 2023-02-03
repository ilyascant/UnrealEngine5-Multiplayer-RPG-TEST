#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Character/SlashCharacter.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(Item);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
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

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ItemState == EItemState::EIS_Hovering) {
		RunningTime = RunningTime + DeltaTime;
		FloatingItem(RunningTime, Amplitude, TimeConstant);
	}
}


