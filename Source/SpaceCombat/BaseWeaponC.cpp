// Fill out your copyright notice in the Description page of Project Settings.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

//#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "SpaceCombat.h"
#include "BaseWeaponC.h"
#include "BaseShipC.h"
#include "Kismet/KismetMathLibrary.h"

#define ECC_TargetTrace ECC_GameTraceChannel15

// Sets default values for this component's properties
ABaseWeaponC::ABaseWeaponC()
{
	//No need for most weapons to tick
	PrimaryActorTick.bCanEverTick = true;
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	FireSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireSoundComponent"));
	//FireSound = CreateDefaultSubobject<USoundBase>(TEXT("FireSound"));{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleParticleOb(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_MF.P_AssaultRifle_MF'"));
	static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundOb(TEXT("SoundWave'/Game/Resistance-AssaultRifle_03-Single_Shot-04.Resistance-AssaultRifle_03-Single_Shot-04'"));
	

	MuzzleParticle = MuzzleParticleOb.Object;
	FireSound = FireSoundOb.Object;
	RootComponent = RootMesh;
	Init();
	if (FireSoundComponent->IsValidLowLevel())
	{
		FireSoundComponent->bAutoActivate = false;
		//FireSoundComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("Barrel"));
		FireSoundComponent->AttachTo(RootComponent, FName("Barrel"));
		FireSoundComponent->SetSound(FireSound);
		FireSoundComponent->Activate();
	}
	Init();
}

// Called when the game starts
void ABaseWeaponC::BeginPlay()
{
	Super::BeginPlay();
	// ...
	FireSoundComponent->SetSound(FireSound);
}

// Called every frame
void ABaseWeaponC::Tick( float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCooldown(DeltaTime);
	UpdateCharge(-DeltaTime);
}

void ABaseWeaponC::Init(){
}

//Fires the weapon
//Different weapon types such as hitscan and projectile weapons will override this with their own way of firing
void ABaseWeaponC::Fire()
{
	TraceCamera();
	SetCooldownToMax();
	if(MuzzleParticle->IsValidLowLevel())
	{
		FTransform EmitterTransform;
		EmitterTransform.SetScale3D(MuzzleParticleScale);
		EmitterTransform.SetLocation(RootMesh->GetSocketLocation(FName("Barrel")));
		auto a = UGameplayStatics::SpawnEmitterAttached(
			MuzzleParticle.Get(),
			RootMesh,
			FName("Barrel")
		);
		a->SetRelativeScale3D(MuzzleParticleScale);
	}

	if(FireSoundComponent->IsValidLowLevel())
	{
		FireSoundComponent->Play();
	}
}

void ABaseWeaponC::TryFire()
{
	UpdateCharge(GetWorld()->GetDeltaSeconds);
	if (CanFire())
	{
		Fire();
	}
}

//Whether the weapon can fire
bool ABaseWeaponC::CanFire()
{
	return CooldownRemaining <= 0.0f && CurrentCharge > MinChargeToFire;
}

//Sets the time when the weapon will finish cooling down
void ABaseWeaponC::UpdateCooldown(float DeltaTime)
{
	CooldownRemaining = FMath::Clamp(CooldownRemaining - DeltaTime, 0.0f, MaxCooldown);
}

void ABaseWeaponC::UpdateCharge(float DeltaTime)
{
	CurrentCharge = FMath::Clamp(CurrentCharge + DeltaTime, 0.0f, MaxCharge);
}

void ABaseWeaponC::SetCooldownToMax()
{
	CooldownRemaining = MaxCooldown;
}

//Trace a line from the camera to the center of the screen
//If it successfully hits an object, that is the weapons target, otherwise the target is the end of the trace
bool ABaseWeaponC::TraceCamera()
{
	FHitResult HitResult;
	bool HitSuccess = false;
	if (GEngine)
	{
		auto Camera = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
		auto CameraTransform = Camera->GetTransform();
		auto CameraLoc = CameraTransform.GetLocation();
		auto ForwardVector = Camera->GetActorForwardVector();
		FName TraceTag("MyTraceTag");
		
		//GetWorld()->DebugDrawTraceTag = TraceTag;

		FCollisionQueryParams CollisionParams;
		//CollisionParams.TraceTag = TraceTag;
		HitSuccess = GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			CameraLoc, 
			CameraLoc + (ForwardVector * TraceDistance), 
			ECC_TargetTrace,
			CollisionParams
		);

		//If the trace hit an object, set HitLocation to the location where the hit occured, otherwise set it to the end of the trace
		HitLocation = HitSuccess ? HitResult.Location : HitResult.TraceEnd;
		
		HitActor = HitResult.Actor;
	}

	return HitSuccess;
}

//Rotates the weapon to face its target, limited by MaxRotation
//Currently doesn't work correctly
void ABaseWeaponC::RotateWeapon()
{
	//Limit rotation of weapon
	FVector WepLoc = RootMesh->GetComponentLocation();
	FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(WepLoc, HitLocation);
	
	FRotator CurrRot = this->GetActorRotation();
	//NewRot.Pitch = FMath::Clamp(NewRot.Pitch, CurrRot.Pitch - MaxRotation.Pitch, CurrRot.Pitch + MaxRotation.Pitch);
	//NewRot.Yaw = FMath::Clamp(NewRot.Yaw, CurrRot.Yaw - MaxRotation.Yaw, CurrRot.Yaw + MaxRotation.Yaw);
	//NewRot.Roll = FMath::Clamp(NewRot.Roll, CurrRot.Roll - MaxRotation.Roll, CurrRot.Roll + MaxRotation.Roll);
	
	//Rotate weapon mesh
	RootMesh->SetWorldRotation(NewRot);

}

WeaponType ABaseWeaponC::GetWeaponType()
{
	return WType;
}

