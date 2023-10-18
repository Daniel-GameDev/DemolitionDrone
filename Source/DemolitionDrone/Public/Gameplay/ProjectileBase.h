// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class DEMOLITIONDRONE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectileBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> ApplyDamageTag;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float DamageValue = 20.f;

};
