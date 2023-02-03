#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:

	ABird();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> BirdMappingContext;

	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);

private:	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BirdMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;

};
