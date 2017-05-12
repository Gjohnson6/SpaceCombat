// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovementVariablesStruct.h"
#include "BaseWeaponC.h"
#include <map>
#include "GameFramework/Pawn.h"
#include "BaseShipC.generated.h"

UCLASS()
class SPACECOMBAT_API ABaseShipC : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseShipC();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	//Passes the input axis for moving forwards and backwards to the pawn controller
	void MoveForward(float AxisValue);

	//Passes the input axis for moving up and down to the pawn controller
	void MoveUp(float AxisValue);

	//Passes the input axis for moving right and left to the pawn controller.
	void MoveRight(float AxisValue);

	//Roll ship based on AxisValue. >0 rolls right, <0 rolls left
	void RollRight(float AxisValue);

	//Adjust ship's pitch based on AxisValue, >0 rolls up <0 rolls down
	void Pitch(float AxisValue);

	//Adjust ship's yaw based on AxisValue, >0 turns right, <0 turns left
	void Yaw(float AxisValue);

	template<WeaponType WepType, bool IsPressed>
	void WeaponTypePressed();
	
	//Handle Afterburner input
	void AfterburnerPressed();
	void AfterburnerReleased();

	//Handle taking damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FMovementVariablesStruct mv;

protected:
	//Base mesh of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Mesh")
		UStaticMeshComponent* RootMesh;

	//Controls movement
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		UFloatingPawnMovement* FloatingPawnMovementComponent;

	//Health and Damage Variables
	//Maximum health of the ship
	float MaxHealth = 100.0f;
	//Current health of the ship
	float CurrentHealth = MaxHealth;

	bool PrimaryFiring = false;
	bool SecondaryFiring = false;
	bool MissileFiring = false;
	bool AfterburnerHeld = false;
	TMap<WeaponType, bool> WeaponTypeFiringMap;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	TArray<UChildActorComponent*> Weapons;

	UPROPERTY(EditDefaultsOnly, Category = "Particles & Sounds")
	TWeakObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particles & Sounds")
	TWeakObjectPtr<USoundBase> ExplosionSound;

};
