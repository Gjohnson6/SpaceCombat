// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseWeaponC.h"
#include "BaseHitScanWeaponC.generated.h"

/**
 * 
 */
UCLASS()
class SPACECOMBAT_API ABaseHitScanWeaponC : public ABaseWeaponC
{
	GENERATED_BODY()

	virtual void Fire() override;
};
