// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(false);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(ProjectileMesh);
	Capsule->SetGenerateOverlapEvents(true);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnCapsuleBeginOverlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1000.f;
	ProjectileMovementComponent->MaxSpeed = 1000.f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(LifeTime);
}

void AProjectileBase::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (FName Tag : ApplyDamageTag)
	{
		if (OtherActor->ActorHasTag(Tag))
		{
			OtherActor->TakeDamage(DamageValue, FDamageEvent(), GetInstigatorController(), this);
			Destroy();
		}
	}
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

