// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Components/ActorComponent.h"
//#include "Actor.h"
#include "GameFramework/Actor.h"
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

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class SPACECOMBAT_API ABaseWeaponC : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ABaseWeaponC();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaTime) override;

	virtual void Fire();

	virtual bool CanFire();

	virtual void UpdateCooldown();

	virtual bool TraceCamera(FVector& HitLocation, TWeakObjectPtr<AActor> HitActor);

	virtual void RotateWeapon(FVector TargetLocation);

	WeaponType GetWeaponType();

protected:
	//Base mesh of the Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Mesh")
		UStaticMeshComponent* RootMesh;

	//How much damage a single shot of the weapon deals
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float damage = 10.0f;

	//The time at which the weapon will finish cooling down and be able to fire again.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float CooldownTime = 0.0f;

	//How long it will take for the weapon to cool down. 
	//This is added to the game time to get CooldownTime whenever the weapon is fired.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float MaxCooldown = 10.0f;

	//Weapons rotate to fire at their target
	//This limits how far they are allowed to rotate
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	FRotator MaxRotation = FRotator(15.0f, 90.0f, 0.0f);

	//For hitscan weapons, this is how far they fire in world units
	//For projectile weapons, this is how fast the projectile moves
	float Speed = 10000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Type")
	WeaponType WType;

};
