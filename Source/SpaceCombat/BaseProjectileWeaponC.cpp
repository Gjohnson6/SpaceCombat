// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceCombat.h"
#include "BaseProjectileWeaponC.h"
#include "BaseShipC.h"
#include "Kismet/KismetMathLibrary.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
void ABaseProjectileWeaponC::Fire()
{
	//Call ABaseWeaponC's fire for 
	Super::Fire();

	if(GetWorld())
	{
		FVector Location = RootMesh->GetSocketLocation(FName("Barrel"));
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Location, HitLocation);
		//print(HitLocation.ToString());
		//FRotator Rotation = RootMesh->GetSocketRotation(FName("Barrel"));
		FActorSpawnParameters SpawnInfo;
		//DrawDebugSphere(GetWorld(), Location, 3, 8, FColorList::Red, false, 2, 1, 5);
		FTransform Transform = FTransform(Rotation, Location);
		FActorSpawnParameters f;
		f.Owner = this;
		//ABaseProjectileC* a = GetWorld()->SpawnActor<ABaseProjectileC>(Cast<UClass>(*Projectile), Location, Rotation, f);
		auto DeferredProjectile = Cast<ABaseProjectileC>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, Projectile, Transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, GetOwner()));
		if (DeferredProjectile)
		{
			DeferredProjectile->SetOwner(GetOwner());
			DeferredProjectile->SetActorEnableCollision(true);
			DeferredProjectile->SetDamage(Damage);
			DeferredProjectile->SetSpeed(Speed);
			auto ship = Cast<ABaseShipC>(GetOwner());
			ship->MoveIgnoreActorAdd(DeferredProjectile);
			UGameplayStatics::FinishSpawningActor(DeferredProjectile, Transform);
		}
	}

}
