// Fill out your copyright notice in the Description page of Project Settings.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#include "SpaceCombat.h"
#include "BaseHitScanWeaponC.h"

void ABaseHitScanWeaponC::Fire()
{
	//Trace for target and rotate weapon to face target
	FVector HitLoc;
	TWeakObjectPtr <AActor> HitActor;
	TraceCamera(HitLoc, HitActor);
	//RotateWeapon(HitLoc);
	
	//Add trace tag so traces are visible
	FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams CollisionParams;
	CollisionParams.TraceTag = TraceTag;
	//Ignore the weapon and the parent the weapon is attached to
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(GetParentActor());

	//Fire in the direction the weapon is facing.
	FHitResult OutHit;
	FVector WeaponLocation = RootMesh->GetComponentLocation();
	FVector TargetLocation = WeaponLocation + RootMesh->GetForwardVector() * Speed;
	bool HitSuccess = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		RootMesh->GetComponentLocation(),
		TargetLocation,
		ECC_Pawn,
		CollisionParams
	);

	//If the trace hit an object, deal damage to it
	if (HitSuccess)
	{
		FDamageEvent DamageEvent;
		FPointDamageEvent PointDamageEvent;
		PointDamageEvent.Damage = damage;
		OutHit.Actor->TakeDamage(
			damage, 
			DamageEvent, 
			GetWorld()->GetFirstPlayerController(), 
			this->GetOwner()
		);
	}

	UpdateCooldown();
}
