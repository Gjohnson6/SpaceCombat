// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MovementVariablesStruct.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class SPACECOMBAT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

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

	//Handle PrimaryFire onpress and onrelease
	void PrimaryFirePressed();
	void PrimaryFireReleased();

	//Handle SecondaryFire onpress and onrelease
	void SecondaryFirePressed();
	void SecondaryFireReleased();

	//Handle MissileFire onpress and onrelease
	void MissileFirePressed();
	void MissileFireReleased();

	//Handle Afterburner input
	void AfterburnerPressed();
	void AfterburnerReleased();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FMovementVariablesStruct mv;
private:
	//Base mesh of the ship
	UPROPERTY(EditDefaultsOnly, Category = "Default Mesh")
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
};
