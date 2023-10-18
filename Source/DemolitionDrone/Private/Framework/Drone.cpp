// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Drone.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/ProjectileBase.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Box);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Camera);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(Camera);

	RightWing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWing"));
	RightWing->SetupAttachment(Box);

	LeftWing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWing"));
	LeftWing->SetupAttachment(Box);

	RightPropeller = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPropeller"));
	RightPropeller->SetupAttachment(RightWing);

	LeftPropeller = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPropeller"));
	LeftPropeller->SetupAttachment(LeftWing);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = 300.f;
	FloatingPawnMovement->Acceleration = 2500.f;
	FloatingPawnMovement->Deceleration = 100.f;
	FloatingPawnMovement->TurningBoost = 0.5f;

	this->bUseControllerRotationYaw = true;
	this->Tags.Add(FName("Target"));

	FireLineArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FireArrow"));
	FireLineArrow->SetupAttachment(GunMesh);

	ShootNum = MagazineSize;
	Health = DefaultHealth;
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		if (HUDWidget)
		{
			UserWidget = CreateWidget<UUserWidget>(PlayerController, HUDWidget);
			UserWidget->AddToViewport();
		}
	}
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnchancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerEnchancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADrone::Look);
		PlayerEnchancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADrone::Move);
		PlayerEnchancedInputComponent->BindAction(TakeoffAction, ETriggerEvent::Triggered, this, &ADrone::Takeoff);
		PlayerEnchancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ADrone::Fire);
	}
}

void ADrone::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookVector.X);
	const float CameraRotationPitch = UKismetMathLibrary::FClamp(LookVector.Y + Camera->GetRelativeRotation().Pitch, MinCameraPitch, MaxCameraPitch);
	Camera->SetRelativeRotation(FRotator(CameraRotationPitch, 0.f, 0.f));
}

void ADrone::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	const FVector ForwardVector = GetActorForwardVector() * MoveVector.Y;
	const FVector RightVector = GetActorRightVector() * MoveVector.X;
	FloatingPawnMovement->AddInputVector(ForwardVector);
	FloatingPawnMovement->AddInputVector(RightVector);
}

void ADrone::Takeoff(const FInputActionValue& Value)
{
	const float TakeoffValue = Value.Get<float>();
	const FVector TakeoffVector = GetActorUpVector() * TakeoffValue;
	FloatingPawnMovement->AddInputVector(TakeoffVector);
}

void ADrone::Fire(const FInputActionValue& Value)
{
	if (bEmptyMagazine == false && bFireReady == true)
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

		if (DroneProjectile)
		{
			FHitResult HitResult;
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(this);
			const FVector CameraForwardLineVector = Camera->GetComponentLocation() + Camera->GetForwardVector() * 5000.f;
			UKismetSystemLibrary::LineTraceSingle(this, Camera->GetComponentLocation(), CameraForwardLineVector, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
			if (HitResult.bBlockingHit)
			{
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(FireLineArrow->GetComponentTransform().GetLocation(), HitResult.ImpactPoint);
				GetWorld()->SpawnActor<AProjectileBase>(DroneProjectile, FTransform(TargetRotation, FireLineArrow->GetComponentTransform().GetLocation(), FVector(1.0f)));
			}
		}

		ShootNum--;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shooting!"));
		}
			
		if (ShootNum == 0)
		{
			bEmptyMagazine = true;
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Empty!"));
			}
		}
	}
}

void ADrone::TimelineShoot(FVector value)
{
	GunMesh->SetRelativeLocation(FVector(value.X, GunMesh->GetRelativeLocation().Y, GunMesh->GetRelativeLocation().Z));
}

void ADrone::ShootFinished()
{
	bFireReady = true;
}

void ADrone::RotatePropellers(float Tick)
{
	RightPropeller->SetRelativeRotation(FRotator(RightPropeller->GetRelativeRotation().Pitch, RightPropeller->GetRelativeRotation().Yaw - PropellersSpeed * Tick, RightPropeller->GetRelativeRotation().Roll));
	LeftPropeller->SetRelativeRotation(FRotator(LeftPropeller->GetRelativeRotation().Pitch, LeftPropeller->GetRelativeRotation().Yaw + PropellersSpeed * Tick, LeftPropeller->GetRelativeRotation().Roll));
}

void ADrone::AltitudeLoss(float tick)
{
	const FVector AltitudeValue = this->GetActorUpVector() * AltitudeLossSpeed * -1 * tick;
	FloatingPawnMovement->AddInputVector(AltitudeValue);
}

float ADrone::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Drone Health: %f"), Health);

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Drone Dead!"));
		UserWidget->RemoveFromParent();
		Destroy();
	}

	return DamageAmount;
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RotatePropellers(DeltaTime);
	AltitudeLoss(DeltaTime);
	if (bFireReady == false)
	{
		ShootTimeline.TickTimeline(DeltaTime);
	}
}