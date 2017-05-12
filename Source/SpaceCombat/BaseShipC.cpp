// Fill out your copyright notice in the Description page of Project Settings.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#include "SpaceCombat.h"
#include "BaseShipC.h"
#include <exception>


ABaseShipC::ABaseShipC()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	RootComponent = RootMesh;
	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovementComponent->UpdatedComponent = RootComponent;

	ExplosionParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(
		TEXT(
			"ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"
		)
	).Object;
	
	ExplosionSound = ConstructorHelpers::FObjectFinder<USoundBase>(
		TEXT(
			"SoundWave'/Game/StarterContent/Audio/Explosion02.Explosion02'"
		)
	).Object;

	WeaponTypeFiringMap[WeaponType::PrimaryWeapon] = false;
	WeaponTypeFiringMap[WeaponType::SecondaryWeapon] = false;
	WeaponTypeFiringMap[WeaponType::MissileWeapon] = false;
	WeaponTypeFiringMap[WeaponType::AuxiliaryWeapon] = false;
}

// Called when the game starts or when spawned
void ABaseShipC::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseShipC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Get all children that have the "Weapon" tag
	auto WeaponChildren = RootComponent->GetAttachChildren().FilterByPredicate(
		[](const USceneComponent* c) 
		{ 
			return c->ComponentHasTag(TEXT("Weapon"));
		}
	);

	//Iterate over all the weapon children
	for (auto t : WeaponChildren)
	{
		auto wep = t->GetAttachChildren();
		//For each child (there should normally be only one), check if it's valid
		//Then cast it to an ABaseWeaponC, check if it can fire and if the
		//corresponding firing button is pressed. If both are true, fire the weapon
		for (auto& w : wep)
		{
			if (w->IsValidLowLevel())
			{
				ABaseWeaponC* AsWeapon = Cast<ABaseWeaponC>(w->GetOwner());
				WeaponType type = AsWeapon->GetWeaponType();
				if (AsWeapon->CanFire() && WeaponTypeFiringMap[type])
				{
					AsWeapon->Fire();
				}
			}
		}
	}	
}

// Called to bind functionality to input
void ABaseShipC::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent);

	//Configure the input component to bind the input axes to this pawn's functions
	InputComponent->BindAxis("MoveForward", this, &ABaseShipC::MoveForward);
	InputComponent->BindAxis("MoveUp", this, &ABaseShipC::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &ABaseShipC::MoveRight);
	InputComponent->BindAxis("Pitch", this, &ABaseShipC::Pitch);
	InputComponent->BindAxis("Yaw", this, &ABaseShipC::Yaw);
	InputComponent->BindAxis("Roll", this, &ABaseShipC::RollRight);

	//Bind action mappings to this pawn's functions
	InputComponent->BindAction("Fire", IE_Pressed, this, &ABaseShipC::WeaponTypePressed<WeaponType::PrimaryWeapon, true>);
	InputComponent->BindAction("Fire", IE_Released, this, &ABaseShipC::WeaponTypePressed<WeaponType::PrimaryWeapon, false>);
	InputComponent->BindAction("SecondaryFire", IE_Pressed, this, &ABaseShipC::WeaponTypePressed<WeaponType::SecondaryWeapon, true>);
	InputComponent->BindAction("SecondaryFire", IE_Released, this, &ABaseShipC::WeaponTypePressed<WeaponType::PrimaryWeapon, false>);
	InputComponent->BindAction("MissileFire", IE_Pressed, this, &ABaseShipC::WeaponTypePressed<WeaponType::MissileWeapon, true>);
	InputComponent->BindAction("MissileFire", IE_Released, this, &ABaseShipC::WeaponTypePressed<WeaponType::MissileWeapon, false>);
	InputComponent->BindAction("Afterburner", IE_Pressed, this, &ABaseShipC::AfterburnerPressed);
	InputComponent->BindAction("Afterburner", IE_Released, this, &ABaseShipC::AfterburnerReleased);
}

void ABaseShipC::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ABaseShipC::MoveUp(float AxisValue)
{
	AddMovementInput(GetActorUpVector(), AxisValue);
}

void ABaseShipC::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

//Rotate by AxisValue multiplied by RollSpeed
void ABaseShipC::RollRight(float AxisValue)
{
	if (AxisValue != 0)
	{
		AddActorLocalRotation(FRotator(0, 0, AxisValue * mv.RollSpeed));
	}
}

void ABaseShipC::Pitch(float AxisValue)
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

void ABaseShipC::Yaw(float AxisValue)
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

template<WeaponType WepType, bool IsPressed>
void ABaseShipC::WeaponTypePressed()
{
	WeaponTypeFiringMap[WepType] = IsPressed;
}

void ABaseShipC::AfterburnerPressed()
{
	AfterburnerHeld = true;
}

void ABaseShipC::AfterburnerReleased()
{
	AfterburnerHeld = false;
}

float ABaseShipC::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0.0f)
	{
		this->Destroy();
	}

	return DamageAmount;
}

void ABaseShipC::Destroyed()
{
	FTransform EmitterTransform;
	EmitterTransform.SetScale3D({ 5.0f, 5.0f, 5.0f });
	EmitterTransform.SetLocation(RootComponent->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(), 
		ExplosionParticle.Get(),
		EmitterTransform, 
		true
	);
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound.Get(), EmitterTransform.GetLocation());
}

