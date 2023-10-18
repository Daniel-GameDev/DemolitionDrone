// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Components/SphereComponent.h"
#include "Framework/Drone.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereBeginOverlap);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Sphere);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::DroneApplyBonus(ADrone* Drone)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("BonusApplied!"));
	}
	Destroy();
}

void AItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Target")))
	{
		if (ADrone* Drone = Cast<ADrone>(OtherActor))
		{
			DroneApplyBonus(Drone);
		}
	}
}

void AItemBase::RotateItem(float Tick)
{
	const float ItemYaw = ItemMesh->GetRelativeRotation().Yaw + RotationSpeed * Tick;
	ItemMesh->SetRelativeRotation(FRotator(0.f, ItemYaw, 0.f));
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateItem(DeltaTime);
}