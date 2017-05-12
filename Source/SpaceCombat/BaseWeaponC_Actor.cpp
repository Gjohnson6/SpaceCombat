// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceCombat.h"
#include "BaseWeaponC_Actor.h"


// Sets default values
ABaseWeaponC_Actor::ABaseWeaponC_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseWeaponC_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeaponC_Actor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

