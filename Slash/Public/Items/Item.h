#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "Item.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EItemState : uint8 {
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipping  UMETA(DisplayName = "Equipping"),
	EIS_Equiped UMETA(DisplayName = "Equiped"),
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
	public:	
		AItem();
		virtual void Tick(float DeltaTime) override;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemName {TEXT("Item")};

		void SetPickUpWidgetVisiblity(const bool setValue);
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	protected:
		virtual void BeginPlay() override;
	
		UFUNCTION()
		virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
		UFUNCTION()
		virtual void Equip(USceneComponent* InParent, const FName& InSocketName);
		UFUNCTION()
		virtual void Drop(const FVector& PlayerLocation, const FVector& PlayerForward);


		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UStaticMeshComponent> Item;

		UFUNCTION()
		void FloatingItem(float& Time, const float& Amplitude, const float& TimeConstant);

		UPROPERTY(VisibleAnywhere)
		TObjectPtr<USphereComponent> SphereArea;

		// Widget
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UWidgetComponent> PickupWidget;

	private:

		float RunningTime;
		UPROPERTY(EditAnywhere, Category = "Floating in Air")
		float Amplitude = 2.f;
		UPROPERTY(EditAnywhere, Category = "Floating in Air")
		float TimeConstant = 5.f;

		UPROPERTY(ReplicatedUsing = OnRep_ItemState, VisibleAnywhere, Category = "Item Properties")
		EItemState ItemState;
		UFUNCTION()
		void OnRep_ItemState();
	public:
		UFUNCTION(BlueprintCallable)
		void SetItemState(EItemState State);
		FORCEINLINE EItemState GetItemState() { return ItemState; };
};
