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

	virtual void Init();

	virtual void Fire();

	virtual void TryFire();

	virtual bool CanFire();

	virtual void UpdateCooldown(float DeltaTime);

	virtual void UpdateCharge(float DeltaTime);

	virtual void SetCooldownToMax();

	virtual bool TraceCamera();

	virtual void RotateWeapon();

	virtual void SetFiring(bool isFiring);

	virtual bool CooldownOk();
	
	virtual bool ChargEOk();

	WeaponType GetWeaponType();

protected:
	//Base mesh of the Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Mesh")
	UStaticMeshComponent* RootMesh;

	//How much damage a single shot of the weapon deals
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float Damage = 10.0f;

	//Cooldown
	//The time at which the weapon will finish cooling down and be able to fire again.
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float CooldownRemaining = 0.0f;

	//How much cooldown to add to the current cooldown when the weapon fires
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float MaxCooldown = 10.0f;

	//Charging
	//For weapons that can't fire immediately like lock on missiles or miniguns, 
	//this determines how long it takes for them to be able to fire from 0, Always 0 for weapons that can fire instantly
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float MinChargeToFire = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float MaxCharge = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon Stats")
	float CurrentCharge = 0.0f;

	bool bFireHeld = false;

	bool bCanCharge = true;


	//For hitscan weapons, this is how far they fire in world units
	//For projectile weapons, this is how fast the projectile moves
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float Speed = 500000.0f;

	//How far the camera trace goes. For hitscan weapons this will be the same as Speed
	//But projectile weapons will generally have much lower speeds, which wouldn't be useable for TraceCamera
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float TraceDistance = Speed;

	//Weapons rotate to fire at their target
	//This limits how far they are allowed to rotate
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	FRotator MaxRotation = FRotator(15.0f, 90.0f, 15.0f);


	UPROPERTY(EditDefaultsOnly, Category = "Weapon Type")
	WeaponType WType;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals and Sounds")
	TWeakObjectPtr<UParticleSystem> MuzzleParticle;
	
	//UPROPERTY(EditDefaultsOnly, Category = "Visuals and Sounds")
	TWeakObjectPtr<UAudioComponent> FireSoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals and Sounds")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals and Sounds")
	FVector MuzzleParticleScale;

	//Actor hit by CameraTrace, used to determine where to point the weapon
	TWeakObjectPtr<AActor> HitActor;

	TWeakObjectPtr<USceneComponent> HitComponent;

	//Location where the camera trace hit or, if no hit occured, the end of the trace
	FVector HitLocation;
};
