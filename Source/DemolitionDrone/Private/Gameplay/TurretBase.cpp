// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TurretBase.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gameplay/ProjectileBase.h"

ATurretBase::ATurretBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());

	TurretLeg = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretLeg"));
	TurretLeg->SetupAttachment(Capsule);

	TurretBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBody"));
	TurretBody->SetupAttachment(TurretLeg);

	TurretBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBarrel"));
	TurretBarrel->SetupAttachment(TurretBody);

	FireLineArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	FireLineArrow->SetupAttachment(TurretBarrel);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComponent->HearingThreshold = 0.f;
	PawnSensingComponent->LOSHearingThreshold = 0.f;
	PawnSensingComponent->SightRadius = 1000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(180.f);
	PawnSensingComponent->SensingInterval = 0.02f;
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATurretBase::OnSeePawn);
}

void ATurretBase::BeginPlay()
{
	Super::BeginPlay();
	RotateTurret();
}

void ATurretBase::OnSeePawn(APawn* OtherPawn)
{
	if (OtherPawn->ActorHasTag(FName("Target")))
	{
		if (LosingTargetTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(LosingTargetTimerHandle);
		}

		TurretState = ETurretState::ETS_Attacking;
		const FVector TargetLoc = OtherPawn->GetActorLocation();
		const FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLoc);
		const float LookRotationClampPitch = UKismetMathLibrary::Clamp(LookRotation.Pitch, -30.f, 60.f);
		const FRotator TurretTargetRotation = FRotator(LookRotationClampPitch, LookRotation.Yaw, TurretBody->GetComponentRotation().Roll);
		const FRotator InverseTrasformRotation = UKismetMathLibrary::InverseTransformRotation(GetRootComponent()->GetComponentTransform(), TurretTargetRotation);
		TurretBody->SetRelativeRotation(InverseTrasformRotation);

		Shooting();

		GetWorld()->GetTimerManager().SetTimer(LosingTargetTimerHandle, this, &ATurretBase::LostTarget, 1.f, false);
	}
}

void ATurretBase::LostTarget()
{
	TurretState = ETurretState::ETS_Searching;
	RotateTurret();
}

void ATurretBase::Reload()
{
	if (ReloadingTimerHandle.IsValid())
	{
		ShootNum = 0;
		bReloading = false;
		GetWorldTimerManager().ClearTimer(ReloadingTimerHandle);
	}
}

void ATurretBase::Shooting()
{
	if (bReloading == false && bFireReady == true)
	{
		bFireReady = false;

		if (ShootVectorCurve)
		{
			TimelineShootProgress.BindUFunction(this, FName("TimelineShoot"));
			ShootTimeline.AddInterpVector(ShootVectorCurve, TimelineShootProgress);
			TimelineOnShootFinish.BindUFunction(this, FName("ShootFinished"));
			ShootTimeline.SetTimelineFinishedFunc(TimelineOnShootFinish);
			ShootTimeline.SetLooping(false);
			ShootTimeline.PlayFromStart();
		}

		if (TurretProjectile)
		{
			AProjectileBase* LaunchedProjectile = GetWorld()->SpawnActor<AProjectileBase>(TurretProjectile, FireLineArrow->GetComponentTransform());
			const FVector ShootingVector = FireLineArrow->GetForwardVector() * ProjectileSpeed;
			const FVector ShootingVectorWithSpread = FVector(ShootingVector.X + FMath::RandRange(MinSpread, MaxSpread), ShootingVector.Y + FMath::RandRange(MinSpread, MaxSpread), ShootingVector.Z + FMath::RandRange(MinSpread, MaxSpread));
			if (LaunchedProjectile)
			{
				LaunchedProjectile->LaunchProjectile(ShootingVectorWithSpread);
			}
		}

		ShootNum++;
		if (ShootNum == MagazineSize)
		{
			bReloading = true;
			GetWorld()->GetTimerManager().SetTimer(ReloadingTimerHandle, this, &ATurretBase::Reload, ReloadTime, false);
		}
	}
}

void ATurretBase::RotateTurret()
{
	CurrentPosition = TurretBody->GetRelativeRotation();

	if (RotationFloatCurve)
	{
		TimelineRotationProgress.BindUFunction(this, FName("TimelineRotate"));
		RotationTimeline.AddInterpFloat(RotationFloatCurve, TimelineRotationProgress);
		RotationTimeline.SetLooping(true);
		RotationTimeline.PlayFromStart();
	}
}

void ATurretBase::TimelineRotate(float value)
{
	if (TurretState == ETurretState::ETS_Searching)
	{
		const FRotator TurretRotator = FRotator(CurrentPosition.Pitch, CurrentPosition.Yaw + value, CurrentPosition.Roll);
		TurretBody->SetRelativeRotation(TurretRotator);
	}
	else
	{
		RotationTimeline.Stop();
		TimelineRotationProgress.Unbind();
	}
}

void ATurretBase::TimelineShoot(FVector value)
{
	TurretBarrel->SetRelativeLocation(FVector(value.X, 0.f, 0.f));
}

void ATurretBase::ShootFinished()
{
	bFireReady = true;
}

void ATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TurretState == ETurretState::ETS_Searching)
	{
		RotationTimeline.TickTimeline(DeltaTime);
	}
	else
	{
		ShootTimeline.TickTimeline(DeltaTime);
	}
}