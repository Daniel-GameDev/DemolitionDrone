// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "Drone.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpotLightComponent;
class UFloatingPawnMovement;
class UInputMappingContext;
class UInputAction;
class InputActionValue;
class AProjectileBase;
class UArrowComponent;

UCLASS()
class DEMOLITIONDRONE_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> GunMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpotLightComponent> SpotLight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> RightWing;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LeftWing;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> RightPropeller;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LeftPropeller;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> FireLineArrow;

	UPROPERTY(EditDefaultsOnly)
	float PropellersSpeed = 1500.f;

	UPROPERTY(EditDefaultsOnly)
	float AltitudeLossSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> TakeoffAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	float MinCameraPitch = -70.f;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	float MaxCameraPitch = -10.f;

	UPROPERTY(EditDefaultsOnly, Category = UserWidget)
	TSubclassOf<UUserWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = UserWidget)
	TObjectPtr<UUserWidget> UserWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> DroneProjectile;

	UPROPERTY(EditAnywhere, Category = Fire)
	TObjectPtr<UCurveVector> ShootVectorCurve;

	UPROPERTY(VisibleAnywhere, Category = Fire)
	uint32 ShootNum;

	UPROPERTY(EditDefaultsOnly, Category = Fire)
	bool bEmptyMagazine = false;

	UPROPERTY(EditDefaultsOnly, Category = Fire)
	bool bFireReady = true;

	UPROPERTY(EditDefaultsOnly, Category = Fire)
	uint32 MagazineSize = 10.f;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float DefaultHealth = 100.f;

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Takeoff(const FInputActionValue& Value);

	UFUNCTION()
	void Fire(const FInputActionValue& Value);

	UFUNCTION()
	void TimelineShoot(FVector value);

	UFUNCTION()
	void ShootFinished();

	FTimeline ShootTimeline;
	FOnTimelineVector TimelineShootProgress;
	FOnTimelineEvent TimelineOnShootFinish;

	void RotatePropellers(float Tick);
	void AltitudeLoss(float tick);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetShootNum() { return ShootNum; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetHealth() { return Health; };

};