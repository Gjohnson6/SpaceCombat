// Fill out your copyright notice in the Description page of Project Settings.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#include "SpaceCombat.h"
#include "BaseWeaponC.h"
#include "BaseShipC.h"


// Sets default values for this component's properties
ABaseWeaponC::ABaseWeaponC()
{
	//No need for most weapons to tick
	PrimaryActorTick.bCanEverTick = false;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	
	WType = WeaponType::PrimaryWeapon;
}

// Called when the game starts
void ABaseWeaponC::BeginPlay()
{
	Super::BeginPlay();
	// ...	
}

// Called every frame
void ABaseWeaponC::Tick( float DeltaTime)
{
	Super::Tick( DeltaTime );

}

//Fires the weapon if the weapon can be fired
//Different weapon types such as hitscan and projectile weapons will override this with their own way of firing
void ABaseWeaponC::Fire()
{

}

//Whether the weapon can fire
bool ABaseWeaponC::CanFire()
{
	return CooldownTime <= GetWorld()->GetUnpausedTimeSeconds();
}

//Sets the time when the weapon will finish cooling down
void ABaseWeaponC::UpdateCooldown()
{
	CooldownTime = GetWorld()->GetUnpausedTimeSeconds() + MaxCooldown;
}

//Trace a line from the camera to the center of the screen
//If it successfully hits an object, that is the weapons target, otherwise the target is the end of the trace
bool ABaseWeaponC::TraceCamera(FVector & HitLocation, TWeakObjectPtr<AActor> HitActor)
{
	FHitResult HitResult;
	bool HitSuccess = false;
	if (GEngine)
	{
		auto camera = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
		auto CameraTransform = camera->GetTransform();
		auto CameraLoc = CameraTransform.GetLocation();
		auto forwardVector = camera->GetActorForwardVector();
		auto CameraRot = CameraTransform.GetRotation();
		auto target = CameraLoc + (CameraRot.Vector() * 1);
		FName TraceTag("MyTraceTag");

		GetWorld()->DebugDrawTraceTag = TraceTag;

		FCollisionQueryParams CollisionParams;
		CollisionParams.TraceTag = TraceTag;
		HitSuccess = GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			CameraLoc, 
			CameraLoc + (forwardVector * Speed), 
			ECollisionChannel::ECC_Visibility,
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
void ABaseWeaponC::RotateWeapon(FVector TargetLocation)
{
	//Limit rotation of weapon
	FVector WepLoc = RootMesh->GetComponentLocation();
	FRotator NewRot = FRotationMatrix::MakeFromX(WepLoc - TargetLocation).Rotator();
	//FMath::Clamp(NewRot.Pitch, -MaxRotation.Pitch, MaxRotation.Pitch);
	//FMath::Clamp(NewRot.Yaw, -MaxRotation.Yaw, MaxRotation.Yaw);
	//FMath::Clamp(NewRot.Roll, -MaxRotation.Roll, MaxRotation.Roll);
	

	//Rotate weapon mesh
	RootMesh->AddLocalRotation(NewRot);
}

WeaponType ABaseWeaponC::GetWeaponType()
{
	return WType;
}

