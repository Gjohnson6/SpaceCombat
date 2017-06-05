// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceCombat.h"
#include "BaseProjectileWeaponC.h"
#include "Kismet/KismetMathLibrary.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
void ABaseProjectileWeaponC::Fire()
{
	//Super
	Super::Fire();

	if(GetWorld())
	{
		FVector Location = RootMesh->GetSocketLocation(FName("Barrel"));
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Location, HitLocation);
		//print(HitLocation.ToString());
		//FRotator Rotation = RootMesh->GetSocketRotation(FName("Barrel"));
		FActorSpawnParameters SpawnInfo;
		//DrawDebugSphere(GetWorld(), Location, 3, 8, FColorList::Red, false, 2, 1, 5);
		
		ABaseProjectileC* a = GetWorld()->SpawnActor<ABaseProjectileC>(Cast<UClass>(*Projectile), Location, Rotation);
		if (a)
		{
			a->SetOwner(GetOwner());
		}
			//a->SetActorScale3D({100, 100, 100});
		//print(a->GetName());
		//	RootMesh->GetSocketLocation(FName("Barrel")),
			//FRotator(),

	}
}
