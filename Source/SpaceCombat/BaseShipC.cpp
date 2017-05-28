// Fill out your copyright notice in the Description page of Project Settings.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#include "SpaceCombat.h"
#include "BaseShipC.h"
#include <exception>

#define ECC_Ship ECC_GameTraceChannel13

ABaseShipC::ABaseShipC()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	RootComponent = RootMesh;
	RootMesh->SetCollisionObjectType(ECC_Ship);
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	MovementComponent->UpdatedComponent = RootComponent;
	
	ExplosionParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Effects/P_Explosion_Large.P_Explosion_Large'")).Object;


	WeaponTypeFiringMap.Add(WeaponType::PrimaryWeapon, false);
	WeaponTypeFiringMap.Add(WeaponType::SecondaryWeapon, false);
	WeaponTypeFiringMap.Add(WeaponType::MissileWeapon, false);
	WeaponTypeFiringMap.Add(WeaponType::AuxiliaryWeapon, false);
	ActorsToIgnore.Add(this);
}

// Called when the game starts or when spawned
void ABaseShipC::BeginPlay()
{
	Super::BeginPlay();

	//When spawned, equip the weapon that corresponsd to each socket in WeaponLoadout
	for(FName SocketName : RootComponent->GetAllSocketNames())
	{
		TSubclassOf<ABaseWeaponC>* Wep = WeaponLoadout.Find(SocketName);

		if(Wep)
		{
			auto w = GetWorld()->SpawnActor(Cast<UClass>(*Wep));
			EquippedWeapons.Add(Cast<ABaseWeaponC>(w));
			w->AttachToComponent(
				RootComponent,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				SocketName
			);
			w->SetOwner(this);
		}
	}
}

// Called every frame
void ABaseShipC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(GetWorld())
	for(ABaseWeaponC* CurrWep : EquippedWeapons)
	{
		if(CurrWep->IsValidLowLevel())
		{
			if(WeaponTypeFiringMap[CurrWep->GetWeaponType()])
			{
				CurrWep->TryFire();
			}
			//print(CurrWep->GetActorLocation().ToString());
		}
	}

}

// Called to bind functionality to input
void ABaseShipC::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	APawn::SetupPlayerInputComponent(InputComponent);
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
	InputComponent->BindAction("SecondaryFire", IE_Released, this, &ABaseShipC::WeaponTypePressed<WeaponType::SecondaryWeapon, false>);
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
		AddActorLocalRotation(FRotator(0, 0, AxisValue * MovementVariables.RollSpeed));
	}
}

void ABaseShipC::Pitch(float AxisValue)
{
	if (AxisValue != 0)
	{
		if (!MovementVariables.FreeTurn)
		{
			FMath::Clamp(AxisValue, -MovementVariables.MaxPitchRadius, MovementVariables.MaxPitchRadius);
		}

		AddActorLocalRotation(FRotator(AxisValue, 0, 0));
	}
}

void ABaseShipC::Yaw(float AxisValue)
{
	if (AxisValue != 0)
	{
		if (!MovementVariables.FreeTurn)
		{
			FMath::Clamp(AxisValue, -MovementVariables.MaxYawRadius, MovementVariables.MaxYawRadius);
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

void ABaseShipC::AddToIgnoreArray(AActor* actorToAdd)
{
	ActorsToIgnore.Add(actorToAdd);
}

void ABaseShipC::RemoveFromIgnoreArray(AActor* actorToRemove)
{
	ActorsToIgnore.Remove(actorToRemove);
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
	
	UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(), 
		ExplosionSound.Get(), 
		EmitterTransform.GetLocation()
	);
}

