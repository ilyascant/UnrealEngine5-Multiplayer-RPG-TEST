#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "Item.generated.h"

class UStaticMeshComponent;
class USphereComponent;

enum class EItemState {
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

	EItemState ItemState;

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
	TObjectPtr<USphereComponent> Sphere;

private:

	float RunningTime;
	UPROPERTY(EditAnywhere, Category = "Floating in Air")
	float Amplitude = 2.f;
	UPROPERTY(EditAnywhere, Category = "Floating in Air")
	float TimeConstant = 5.f;


};
