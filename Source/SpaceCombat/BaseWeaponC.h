// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BaseWeaponC.generated.h"


UENUM(BlueprintType)
enum class WeaponType : uint8
{
	//Consistent Damage Weapons
	PrimaryWeapon,
	//Burst Damage Weapons
	SecondaryWeapon,
	//Explosive Weapons
	MissileWeapon,
	//Special weapons such as mines and sonar drills
	AuxiliaryWeapon
};

UCLASS()
class SPACECOMBAT_API UBaseWeaponC : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseWeaponC();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void Fire(FVector StartLocation);

	virtual bool CanFire();

	virtual void UpdateCooldown();

	virtual bool TraceCamera(FVector& HitLocation, TWeakObjectPtr<AActor> HitActor);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float damage;

	//The time at which the weapon will finish cooling down and be able to fire again.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float CooldownTime = 0.0f;

	//How long it will take for the weapon to cool down. 
	//This is added to the game time to get CooldownTime whenever the weapon is fired.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float MaxCooldown = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Type")
	WeaponType WType;
};
