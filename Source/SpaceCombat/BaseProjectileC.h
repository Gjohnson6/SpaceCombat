// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseProjectileC.generated.h"

UCLASS()
class SPACECOMBAT_API ABaseProjectileC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectileC();

	ABaseProjectileC(float Damage, float Health, float Speed);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Mesh")
	UStaticMeshComponent* RootMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	//Damage dealt by the projectile when it overlaps or hits
	float Damage = 10.0f;
	//How much damage the projectile can take before being destroyed, for destroyable projectils
	float Health = 10.0f;

	float Speed = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals and Sounds")
	TWeakObjectPtr<UParticleSystem> TrailParticle;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "Visuals and Sounds")
	TWeakObjectPtr<UParticleSystem> ImpactParticle;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "Visuals and Sounds")
	TWeakObjectPtr<UMaterial> ImpactDecal;

	TWeakObjectPtr<UDecalComponent> ImpactDecalComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals and Sounds")
		FVector TrailParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals and Sounds")
	FVector DecalMaterialScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals and Sounds")
	FVector ImpactParticleScale;
};
