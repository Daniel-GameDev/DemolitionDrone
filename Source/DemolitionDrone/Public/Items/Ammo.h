// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS()
class DEMOLITIONDRONE_API AAmmo : public AItemBase
{
	GENERATED_BODY()
	
protected:
	virtual void DroneApplyBonus(ADrone* Drone) override;

};
