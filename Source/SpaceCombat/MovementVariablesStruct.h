#pragma once
#include "MovementVariablesStruct.generated.h"

//Variables relating to ship movement
USTRUCT(BlueprintType)
struct FMovementVariablesStruct
{
	GENERATED_BODY()
	
	FMovementVariablesStruct() {}

	//Unmodified max speed the ship can move
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float BaseMaxSpeed = 150.0f;
	//Max speed the ship can move at the current time
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxSpeed = 150.0f;
	//Current speed the ship is moving forward
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CurrentSpeed = 0.0f;
	//Speed the ship wants to move forward
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DesiredSpeed = 0.0f;
	//How fast the ship can change speed
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AccelerationInterp = 0.5f;
	//How fast the ship can strafe
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StrafeSpeed = 25.0f;
	//Whether the ship can turn freely
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	bool FreeTurn = false;
	//Ship's current velocity in world space;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector MovementVector = FVector();
	//Ship's forward vector except when FreeTurn is true; 
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector DirectionHeaded = FVector();
	//How fast the ship can roll
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RollSpeed = 1.5f;
	//Maximum yaw the ship can turn per delta
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxYawRadius = 1.0f;
	//Maximum pitch the ship can turn per delta
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxPitchRadius = 0.5f;
	//How long boost lasts
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxBoostduration = 5.0f;
	//How long boost has been held for
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BoostDuration = 0.0f;
	//How long it takes boost to cooldown
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxBoostCooldown = 10.0f;
	//How long until boost can be used again
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BoostCooldown = 0.0f;
};