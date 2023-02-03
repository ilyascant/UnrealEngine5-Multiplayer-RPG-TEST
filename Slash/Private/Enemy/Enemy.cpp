#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera ,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DractionHitReact(ImpactPoint);
	
	if (HitSound) UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	if (HitParticles && GetWorld()) UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HitParticles,
		ImpactPoint
	);
}

void AEnemy::DractionHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Poiwnt to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * Hit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// conver from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta Should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	CrossProduct.Z < 0 ? Theta *= -1.f : Theta;

	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)  Section = FName("FromFront");
	if (Theta < -45.f && Theta >= -135.f)  Section = FName("FromLeft");
	if (Theta < 135.f && Theta >= 45.f)  Section = FName("FromRight");

	PlayHitReactMontage(Section);
}

