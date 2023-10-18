// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Health.h"
#include "Framework/Drone.h"

void AHealth::DroneApplyBonus(ADrone* Drone)
{
	Drone->SetHealth(100.f);
	Super::DroneApplyBonus(Drone);
}