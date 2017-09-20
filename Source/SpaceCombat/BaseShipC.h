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

	UMovementComponent* GetMovementComponent();

	UFUNCTION(BlueprintCallable , Category = "Getters")
	void SetMaxSpeed(float speed);

	UFUNCTION(BlueprintPure, Category = "Getters")
	FVector2D GetCrosshairOffset();

	FVector2D GetCrosshairPosition();

	//Handle taking damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FMovementVariablesStruct MovementVariables;

protected:
	//Base mesh of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Mesh")
	UStaticMeshComponent* RootMesh;

	//Controls movement
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UFloatingPawnMovement* MovementComponent;

	//Array of actors to pass to weapons to be ignored by weapons and projectiles
	//Includes the ship itself and projectiles fired
	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	TArray<AActor*> ActorsToIgnore;

	//Health and Damage Variables
	//Maximum health of the ship
	float MaxHealth = 100.0f;
	//Current health of the ship
	float CurrentHealth = MaxHealth;

	//Maps weapon types to bools to indicate whether that weapon type is supposed to be firing
	TMap<WeaponType, bool> WeaponTypeFiringMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<ABaseWeaponC*> EquippedWeapons;

	bool AfterburnerHeld = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	//TArray<TPair<TSubclassOf<ABaseWeaponC>, FName>> WeaponLoadout;
	TMap<FName, TSubclassOf<ABaseWeaponC>> WeaponLoadout;

	UPROPERTY(EditDefaultsOnly, Category = "Particles & Sounds")
	TWeakObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particles & Sounds")
	TWeakObjectPtr<USoundBase> ExplosionSound;


	FVector2D CrosshairOffset;

private:

	template <WeaponType WepType, bool IsPressed>
	void WeaponTypePressed();

	void AddToIgnoreArray(AActor* actorToAdd);
	void RemoveFromIgnoreArray(AActor* actorToRemove);

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

	void BrakePressed();
	void BrakeReleased();

	//Handle Afterburner input
	void AfterburnerPressed();
	void AfterburnerReleased();

	//Handle free turn held
	void FreeTurnPressed();
	void FreeTurnReleased();

	//Radius the crosshair is allowed to move
	float CrosshairRange = 150.0f;
	
	float OffsetDist = 0.0f;

	//The closest point to the given offset while still being within crosshair range
	void ClampToCrosshairRange(FVector2D& Offset);
	
	float GetOffsetDist(FVector2D& Offset);

	bool bFreeTurnHeld = false;

	float Scalar = 0.0f;
};
