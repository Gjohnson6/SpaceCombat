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
	Scalar = DeltaTime / .16f;
	float dist = GetOffsetDist(CrosshairOffset);

	if (!bFreeTurnHeld)
	{
		AddActorLocalRotation(FRotator(0, (CrosshairOffset.X / 5) * Scalar, 0));
		AddActorLocalRotation(FRotator((-CrosshairOffset.Y / 5) * Scalar, 0, 0));
	}
}

// Called to bind functionality to input
void ABaseShipC::SetupPlayerInputComponent(UInputComponent* uInputComponent)
{
	APawn::SetupPlayerInputComponent(uInputComponent);
	check(uInputComponent);

	//Configure the input component to bind the input axes to this pawn's functions
	uInputComponent->BindAxis("MoveForward", this, &ABaseShipC::MoveForward);
	uInputComponent->BindAxis("MoveUp", this, &ABaseShipC::MoveUp);
	uInputComponent->BindAxis("MoveRight", this, &ABaseShipC::MoveRight);
	uInputComponent->BindAxis("Pitch", this, &ABaseShipC::Pitch);
	uInputComponent->BindAxis("Yaw", this, &ABaseShipC::Yaw);
	uInputComponent->BindAxis("Roll", this, &ABaseShipC::RollRight);

	//Bind action mappings to this pawn's functions
	uInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseShipC::WeaponTypePressed<WeaponType::PrimaryWeapon, true>);
	uInputComponent->BindAction("Fire", IE_Released, this, &ABaseShipC::WeaponTypePressed<WeaponType::PrimaryWeapon, false>);
	uInputComponent->BindAction("SecondaryFire", IE_Pressed, this, &ABaseShipC::WeaponTypePressed<WeaponType::SecondaryWeapon, true>);
	uInputComponent->BindAction("SecondaryFire", IE_Released, this, &ABaseShipC::WeaponTypePressed<WeaponType::SecondaryWeapon, false>);
	uInputComponent->BindAction("MissileFire", IE_Pressed, this, &ABaseShipC::WeaponTypePressed<WeaponType::MissileWeapon, true>);
	uInputComponent->BindAction("MissileFire", IE_Released, this, &ABaseShipC::WeaponTypePressed<WeaponType::MissileWeapon, false>);
	uInputComponent->BindAction("Afterburner", IE_Pressed, this, &ABaseShipC::AfterburnerPressed);
	uInputComponent->BindAction("Afterburner", IE_Released, this, &ABaseShipC::AfterburnerReleased);
	uInputComponent->BindAction("Brake", IE_Pressed, this, &ABaseShipC::BrakePressed);
	uInputComponent->BindAction("Brake", IE_Released, this, &ABaseShipC::BrakeReleased);
	uInputComponent->BindAction("Free", IE_Pressed, this, &ABaseShipC::FreeTurnPressed);
	uInputComponent->BindAction("Free", IE_Released, this, &ABaseShipC::FreeTurnReleased);
}

//UMovementComponent* ABaseShipC::GetMovementComponent()
//{
	//return MovementComponent;
//}
void ABaseShipC::SetMaxSpeed(float speed)
{
	MovementComponent->MaxSpeed = speed;
}

void ABaseShipC::MoveForward(float AxisValue)
{
	if (!bFreeTurnHeld)
	{
		AddMovementInput(GetActorForwardVector(), AxisValue);
	}	
}

void ABaseShipC::MoveUp(float AxisValue)
{
	if (!bFreeTurnHeld)
	{
		AddMovementInput(GetActorUpVector(), AxisValue);
	}
}

void ABaseShipC::MoveRight(float AxisValue)
{
	if (!bFreeTurnHeld)
	{
		AddMovementInput(GetActorRightVector(), AxisValue);
	}
}

//Rotate by AxisValue multiplied by RollSpeed
void ABaseShipC::RollRight(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		AddActorLocalRotation(FRotator(0, 0, AxisValue * MovementVariables.RollSpeed * Scalar));
	}
}

void ABaseShipC::Pitch(float AxisValue)
{
	if (AxisValue != 0.0f)
	{	
		if (!bFreeTurnHeld)
		{
			CrosshairOffset.Y -= AxisValue;
			ClampToCrosshairRange(CrosshairOffset);
		}
		else
		{
			AddActorLocalRotation(FRotator(AxisValue * Scalar * 10, 0.f, 0.f));
		}
		//AddActorLocalRotation(FRotator(AxisValue, 0, 0));
	}
}

void ABaseShipC::Yaw(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		if (!bFreeTurnHeld)
		{	CrosshairOffset.X += AxisValue;
			ClampToCrosshairRange(CrosshairOffset);			
		}
		else
		{
			AddActorLocalRotation(FRotator(0.f, AxisValue * Scalar * 10, 0.f));
		}
		//AddActorLocalRotation(FRotator(0, AxisValue, 0));
	}
}

void ABaseShipC::BrakePressed()
{
	MovementComponent->Deceleration = MovementVariables.BrakingDeceleration;
}

void ABaseShipC::BrakeReleased()
{
	MovementComponent->Deceleration = 0;
}

void ABaseShipC::AfterburnerPressed()
{
	AfterburnerHeld = true;
	MovementComponent->MaxSpeed *= 2;
	MovementComponent->Acceleration *= 2;

}

void ABaseShipC::AfterburnerReleased()
{
	AfterburnerHeld = false;
	MovementComponent->MaxSpeed /= 2;
	MovementComponent->Acceleration /= 2;
}

void ABaseShipC::FreeTurnPressed()
{
	bFreeTurnHeld = true;
	CrosshairOffset.X = 0;
	CrosshairOffset.Y = 0;
	MovementComponent->Deceleration = 0;
}

void ABaseShipC::FreeTurnReleased()
{
	bFreeTurnHeld = false;
	MovementComponent->Deceleration = MovementVariables.Deceleration;
}
template<WeaponType WepType, bool IsPressed>
void ABaseShipC::WeaponTypePressed()
{
	WeaponTypeFiringMap[WepType] = IsPressed;

	if (!IsPressed)
	{
		GetWorld();
	}

	//Set all weapons of type WepType
	if (GetWorld())
	{
		for (ABaseWeaponC* CurrWep : EquippedWeapons.FilterByPredicate([](ABaseWeaponC* w) { return w->GetWeaponType() == WepType; }))
		{
			if (CurrWep->IsValidLowLevel())
			{
				CurrWep->SetFiring(IsPressed);
			}
		}
	}
}

void ABaseShipC::ClampToCrosshairRange(FVector2D & Offset)
{
	float X = Offset.X;
	float Y = Offset.Y;
	float dist = GetOffsetDist(Offset);
	if (dist > CrosshairRange)
	{
		Offset.X = X / dist * CrosshairRange;
		Offset.Y = Y / dist * CrosshairRange;
	}
}

float ABaseShipC::GetOffsetDist(FVector2D & Offset)
{
	return sqrt(Offset.X * Offset.X + Offset.Y * Offset.Y);
}

void ABaseShipC::AddToIgnoreArray(AActor* actorToAdd)
{
	ActorsToIgnore.Add(actorToAdd);
}

void ABaseShipC::RemoveFromIgnoreArray(AActor* actorToRemove)
{
	ActorsToIgnore.Remove(actorToRemove);
}

FVector2D ABaseShipC::GetCrosshairOffset()
{
	return CrosshairOffset;
}

FVector2D ABaseShipC::GetCrosshairPosition()
{
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);


	return FVector2D(ViewportCenter.X + CrosshairOffset.X, ViewportCenter.Y + CrosshairOffset.Y);
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

