// Fill out your copyright notice in the Description page of Project Settings.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#include "SpaceCombat.h"
#include "BaseHitScanWeaponC.h"


void ABaseHitScanWeaponC::Init()
{

	//FireSoundComponent->SetSound(FireSound.Get());
	//MuzzleParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_MF.P_AssaultRifle_MF'")).Object;
	//FireSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("SoundWave'/Game/Resistance-AssaultRifle_03-Single_Shot-04.Resistance-AssaultRifle_03-Single_Shot-04'")).Object;
	//BeamParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_Trail.P_AssaultRifle_Trail'")).Object;
}

void ABaseHitScanWeaponC::Fire()
{
	//Trace for target and rotate weapon to face target
/*	
	RotateWeapon(HitLoc);
*/	
	Super::Fire();
	//FVector HitLoc;
	//TWeakObjectPtr <AActor> HitActor;
	//TraceCamera();
	//Add trace tag so traces are visible
	FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams CollisionParams;
	//CollisionParams.TraceTag = TraceTag;
	//Ignore the weapon and the parent the weapon is attached to
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(GetAttachParentActor());

	//Fire in the direction the weapon is facing.
	TArray<FHitResult> OutHit;
	FVector WeaponLocation = RootMesh->GetComponentLocation();
	FVector TargetLocation = WeaponLocation + RootMesh->GetForwardVector() * Speed;
	bool HitSuccess = GetWorld()->LineTraceMultiByChannel(
		OutHit,
		RootMesh->GetComponentLocation(),
		TargetLocation,
		CollisionChannel,
		CollisionParams
	);

	auto Beam = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		BeamParticle.Get(), 
		RootMesh->GetSocketTransform(FName("Barrel"))
	);
	if (Beam)
	{
		Beam->SetBeamTargetPoint(0, TargetLocation, 0);
		Beam->SetBeamSourcePoint(1, RootMesh->GetSocketLocation(FName("Barrel")), 0);
		Beam->SetBeamTargetPoint(0, TargetLocation, 1);
		Beam->SetBeamSourcePoint(1, RootMesh->GetSocketLocation(FName("Barrel")), 1);
		Beam->SetWorldScale3D(MuzzleParticleScale);
	}
	//If the trace hit an object, deal damage to it
	if (HitSuccess)
	{
		FDamageEvent DamageEvent;
		FPointDamageEvent PointDamageEvent;
		PointDamageEvent.Damage = Damage;
		for (auto HitRes : OutHit)
		{
			HitRes.Actor->TakeDamage(
				Damage,
				DamageEvent,
				GetWorld()->GetFirstPlayerController(),
				this->GetOwner()
			);
		}
	}
}
