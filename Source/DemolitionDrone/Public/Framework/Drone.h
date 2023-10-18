// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpotLightComponent;
class UFloatingPawnMovement;
class UInputMappingContext;
class UInputAction;
class InputActionValue;

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
	float MinCameraPitch = -70.f;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	float MaxCameraPitch = -10.f;

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Takeoff(const FInputActionValue& Value);

	void RotatePropellers(float Tick);
	void AltitudeLoss(float tick);

};
