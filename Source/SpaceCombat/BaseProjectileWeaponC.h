// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseProjectileC.h"
#include "BaseWeaponC.h"
#include "BaseProjectileWeaponC.generated.h"

/**
 */
UCLASS()
class SPACECOMBAT_API ABaseProjectileWeaponC : public ABaseWeaponC
{
	GENERATED_BODY()
	
	virtual void Fire() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	TSubclassOf<ABaseProjectileC>  Projectile;
};
