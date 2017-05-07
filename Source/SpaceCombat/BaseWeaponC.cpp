// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceCombat.h"
#include "BaseWeaponC.h"


// Sets default values for this component's properties
UBaseWeaponC::UBaseWeaponC()
{
	//No need for most weapons to tick
	PrimaryComponentTick.bCanEverTick = false;

	WType = WeaponType::PrimaryWeapon;
}


// Called when the game starts
void UBaseWeaponC::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseWeaponC::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}

bool UBaseWeaponC::CanFire()
{
	return CooldownTime <= GetWorld()->GetUnpausedTimeSeconds();
}

void UBaseWeaponC::UpdateCooldown()
{
	CooldownTime = GetWorld()->GetUnpausedTimeSeconds() + MaxCooldown;
}

bool UBaseWeaponC::TraceCamera(FVector & HitLocation, TWeakObjectPtr<AActor> HitActor)
{
	FHitResult hitResult;
	bool hitRet = false;
	//GetWorld()->LineTraceSingleByChannel(hitResult, )
	if (GEngine)
	{
		auto CameraTransform = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetTransform();
		auto CameraLoc = CameraTransform.GetLocation();
		auto CameraRot = CameraTransform.GetRotation();
		hitRet = GetWorld()->LineTraceSingleByChannel(hitResult, CameraLoc, CameraRot.Vector * 100000.0f, ECollisionChannel::ECC_Visibility);

		//If the trace hit an object, set HitLocation to the location where the hit occured, otherwise set it to the end of the trace
		HitLocation = hitRet ? hitResult.Location : hitResult.TraceEnd;

		HitActor = hitResult.Actor;
	}
	return hitRet;
}

