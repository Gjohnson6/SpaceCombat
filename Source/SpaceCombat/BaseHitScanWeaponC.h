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

	virtual void Init() override;

	virtual void Fire() override;

protected:

	//Used to select collision type for the weapon. Piercing hitscan weapons will overlap instead of block
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Type")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Pawn;

	UPROPERTY(EditDefaultSOnly, Category = "Visuals and Sounds")
	TWeakObjectPtr<UParticleSystem> BeamParticle;
};
