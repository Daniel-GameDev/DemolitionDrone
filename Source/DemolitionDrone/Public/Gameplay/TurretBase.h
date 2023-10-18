// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "TurretBase.generated.h"

UENUM(BlueprintType)
enum class ETurretState : uint8
{
	ETS_Searching UMETA(DisplayName = "Searching Target"),
	ETS_Attacking UMETA(DisplayName = "Attacking Target")
};

class UArrowComponent;
class UCapsuleComponent;
class UPawnSensingComponent;
class AProjectileBase;

UCLASS()
class DEMOLITIONDRONE_API ATurretBase : public APawn
{
	GENERATED_BODY()

public:
	ATurretBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretLeg;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretBarrel;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> FireLineArrow;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Turret State")
	ETurretState TurretState = ETurretState::ETS_Searching;

	UPROPERTY(EditAnywhere, Category = Timeline)
	TObjectPtr<UCurveFloat> RotationFloatCurve;

	UPROPERTY(EditAnywhere, Category = Timeline)
	TObjectPtr<UCurveVector> ShootVectorCurve;

	UPROPERTY(VisibleAnywhere)
	bool bReloading = false;

	UPROPERTY(VisibleAnywhere)
	bool bFireReady = true;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> TurretProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Settings")
	float ReloadTime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Settings")
	uint32 MagazineSize = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Projectile Settings")
	uint32 ShootNum;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float DefaultHealth = 100.f;

	UFUNCTION()
	void TimelineRotate(float value);

	UFUNCTION()
	void TimelineShoot(FVector value);

	UFUNCTION()
	void ShootFinished();

	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
	void Shooting();

	FTimeline RotationTimeline;
	FOnTimelineFloat TimelineRotationProgress;
	FTimeline ShootTimeline;
	FOnTimelineVector TimelineShootProgress;
	FOnTimelineEvent TimelineOnShootFinish;
	FRotator CurrentPosition;
	FTimerHandle LosingTargetTimerHandle;
	FTimerHandle ReloadingTimerHandle;

	void RotateTurret();
	void LostTarget();
	void Reload();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetHealth() { return Health; };

};
