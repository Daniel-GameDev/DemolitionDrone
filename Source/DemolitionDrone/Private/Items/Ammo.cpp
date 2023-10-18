// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Ammo.h"
#include "Framework/Drone.h"

void AAmmo::DroneApplyBonus(ADrone* Drone)
{
	Drone->SetEmptyMagazine(false);
	Drone->SetShootNum(10);
	Super::DroneApplyBonus(Drone);
}
