// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceCombat.h"
#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	RootComponent = RootMesh;
	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovementComponent->UpdatedComponent = RootComponent;

	//MovementVariables = FMovementVariablesStruct();
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Approach desired speed
	mv.CurrentSpeed = FMath::FInterpTo(mv.CurrentSpeed, mv.DesiredSpeed, GetWorld()->DeltaTimeSeconds, mv.AccelerationInterp / 2);

	
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent);

	//Configure the input component to bind the input axes to this pawn's functions
	InputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	InputComponent->BindAxis("MoveUp", this, &AMyPawn::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
	InputComponent->BindAxis("Pitch", this, &AMyPawn::Pitch);
	InputComponent->BindAxis("Yaw", this, &AMyPawn::Yaw);

	//Bind action mappings to this pawn's functions
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMyPawn::PrimaryFirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AMyPawn::PrimaryFireReleased);
	InputComponent->BindAction("SecondaryFire", IE_Pressed, this, &AMyPawn::SecondaryFirePressed);
	InputComponent->BindAction("SecondaryFire", IE_Released, this, &AMyPawn::SecondaryFireReleased);
	InputComponent->BindAction("MissileFire", IE_Pressed, this, &AMyPawn::MissileFirePressed);
	InputComponent->BindAction("MissileFire", IE_Released, this, &AMyPawn::MissileFireReleased);
	InputComponent->BindAction("Afterburner", IE_Pressed, this, &AMyPawn::AfterburnerPressed);
	InputComponent->BindAction("Afterburner", IE_Released, this, &AMyPawn::AfterburnerReleased);
	
}

void AMyPawn::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void AMyPawn::MoveUp(float AxisValue)
{
	AddMovementInput(GetActorUpVector(), AxisValue);
}

void AMyPawn::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

//Rotate by AxisValue multiplied by RollSpeed
void AMyPawn::RollRight(float AxisValue)
{
	if (AxisValue != 0)
	{
		AddActorLocalRotation(FRotator(0, 0, AxisValue * mv.RollSpeed));
	}
}

void AMyPawn::Pitch(float AxisValue)
{
	if (AxisValue != 0)
	{
		if (!mv.FreeTurn)
		{
			FMath::Clamp(AxisValue, -mv.MaxPitchRadius, mv.MaxPitchRadius);
		}

		AddActorLocalRotation(FRotator(AxisValue, 0, 0));
	}
}

void AMyPawn::Yaw(float AxisValue)
{
	if (AxisValue != 0)
	{
		if (!mv.FreeTurn)
		{
			FMath::Clamp(AxisValue, -mv.MaxYawRadius, mv.MaxYawRadius);
		}

		AddActorLocalRotation(FRotator(0, AxisValue, 0));
	}
}

void AMyPawn::PrimaryFirePressed()
{
	PrimaryFiring = true;
}

void AMyPawn::PrimaryFireReleased()
{
	PrimaryFiring = false;
}

void AMyPawn::SecondaryFirePressed()
{
	SecondaryFiring = true;
}

void AMyPawn::SecondaryFireReleased()
{
	SecondaryFiring = false;
}

void AMyPawn::MissileFirePressed()
{
	MissileFiring = true;
}

void AMyPawn::MissileFireReleased()
{
	MissileFiring = false;
}

void AMyPawn::AfterburnerPressed()
{
	AfterburnerHeld = true;
}

void AMyPawn::AfterburnerReleased()
{
	AfterburnerHeld = false;
}

