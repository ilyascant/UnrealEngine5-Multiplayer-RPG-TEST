#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "GunWeapon.generated.h"

UCLASS()
class SLASH_API AGunWeapon : public AItem
{
	GENERATED_BODY()

	public:
		AGunWeapon();
		virtual void Tick(float DeltaTime) override;
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

		virtual void Equip(USceneComponent* InParent, const FName& InSocketName);
		virtual void Drop(const FVector& PlayerLocation, const FVector& PlayerForward);
		virtual bool AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName);
		virtual bool DetachFromComponent(TObjectPtr<USceneComponent>& InParent, const FName& InSocketName);

	protected:
		virtual void BeginPlay() override;

		virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
