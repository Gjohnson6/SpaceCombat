// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceCombat.h"
#include "BaseProjectileC.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
ABaseProjectileC::ABaseProjectileC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	RootMesh->IgnoreActorWhenMoving(GetOwner(), true);
	RootMesh->SetNotifyRigidBodyCollision(true);
	RootMesh->SetCollisionProfileName("Projectile");
	RootMesh->OnComponentHit.AddDynamic(this, &ABaseProjectileC::OnHit);
	RootComponent = RootMesh;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ImpactParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_IH.P_AssaultRifle_IH'")).Object;
	ImpactDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));



	//	ParticleSystem'/Game/Effects/P_AssaultRifle_Trail.P_AssaultRifle_Trail'
	ProjectileMovement->UpdatedComponent = RootComponent;
	
	auto AttachedEmitter = UGameplayStatics::SpawnEmitterAttached(
		TrailParticle.Get(),
		RootMesh
	);
	if (AttachedEmitter)
	{
		AttachedEmitter->SetRelativeScale3D(TrailParticleScale);
	}
}

ABaseProjectileC::ABaseProjectileC(float Damage, float Health, float Speed)
{
	this->Damage = Damage;
	this->Health = Health;
	this->Speed = Speed;
}

// Called when the game starts or when spawned
void ABaseProjectileC::BeginPlay()
{
	Super::BeginPlay();	
	if (ImpactDecalComponent->IsValidLowLevel())
	{
		ImpactDecalComponent->SetDecalMaterial(ImpactDecal.Get());
		ImpactDecalComponent->DecalSize = { 3, 3, 3 };
		ImpactDecalComponent->FadeScreenSize = 10000000.f;

	}
}

// Called every frame
void ABaseProjectileC::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABaseProjectileC::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetOwner())
	{
		FTransform HitTransform;
		HitTransform.SetLocation(Hit.Location);
		HitTransform.SetRotation(Hit.ImpactNormal.Rotation().Quaternion());
		auto Emitter = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ImpactParticle.Get(),
			HitTransform,
			true
		);
		Emitter->SetWorldScale3D(ImpactParticleScale);

		if (OtherActor->IsValidLowLevel())
		{
			FPointDamageEvent PointDamageEvent;
			PointDamageEvent.Damage = Damage;
			OtherActor->TakeDamage(
				this->Damage,
				PointDamageEvent,
				GetWorld()->GetFirstPlayerController(),
				GetOwner()
			);
		}

		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(), 
			ImpactDecalComponent->DecalMaterial, 
			ImpactDecalComponent->DecalSize, 
			HitTransform.GetLocation(), 
			HitTransform.Rotator(),
			10.0f
		);
		this->Destroy();
	}
}

