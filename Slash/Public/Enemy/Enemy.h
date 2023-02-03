#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

	void DractionHitReact(const FVector& ImpactPoint);

protected:
	virtual void BeginPlay() override;

	/**
		Play Montage Functions
	*/
	void PlayHitReactMontage(const FName& SectionName);

private:

	/**
		 Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> HitSound;

	/**
		Particles
	*/
	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> HitParticles;
	

};
