// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ProjectileBase.h"
#include "Components/CapsuleComponent.h"

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
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnCapsuleBeginOverlap);
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
			Destroy();
		}
	}
}

void AProjectileBase::LaunchProjectile(FVector Direction)
{
	ProjectileMesh->SetPhysicsLinearVelocity(Direction);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

