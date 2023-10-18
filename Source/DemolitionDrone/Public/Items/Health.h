// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Health.generated.h"

/**
 * 
 */
UCLASS()
class DEMOLITIONDRONE_API AHealth : public AItemBase
{
	GENERATED_BODY()
	
protected:
	virtual void DroneApplyBonus(ADrone* Drone) override;

};
