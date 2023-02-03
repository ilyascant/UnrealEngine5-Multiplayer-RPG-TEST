#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
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
		bool AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName);
		bool DetachFromComponent(TObjectPtr<USceneComponent>& InParent, const FName& InSocketName);


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

	public:
		FORCEINLINE TObjectPtr<UBoxComponent> GetWeaponBox() const { return WeaponBoxComp; };

		
		
	

		
	
};
