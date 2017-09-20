// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceCombat.h"
#include "BaseProjectileC.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
#define ECC_Projectile ECC_GameTraceChannel1
// Sets default values
ABaseProjectileC::ABaseProjectileC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	RootMesh->IgnoreActorWhenMoving(GetOwner(), true);
	SetActorEnableCollision(false);
	RootMesh->SetNotifyRigidBodyCollision(false);
	RootMesh->SetCollisionProfileName("Projectile");
	RootMesh->SetCollisionObjectType(ECC_Projectile);
	RootMesh->OnComponentHit.AddDynamic(this, &ABaseProjectileC::OnHit);
	RootComponent = RootMesh;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));	
	ProjectileMovement->bIsHomingProjectile = bIsHoming;
	ProjectileMovement->HomingTargetComponent = Target;
	ProjectileMovement->UpdatedComponent = RootComponent;

	ImpactParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_IH.P_AssaultRifle_IH'")).Object;
	ImpactDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));	
	auto AttachedEmitter = UGameplayStatics::SpawnEmitterAttached(
		TrailParticle.Get(),
		RootMesh
	);
	if (AttachedEmitter)
	{
		AttachedEmitter->SetRelativeScale3D(TrailParticleScale);
	}
}

ABaseProjectileC::ABaseProjectileC(float Damage, float Health, float Speed, bool bIsHoming, USceneComponent* TargetComponent)
{
	this->Damage = Damage;
	this->Health = Health;
	this->Speed = Speed;
	this->ProjectileMovement->bIsHomingProjectile = bIsHoming;
	this->ProjectileMovement->HomingTargetComponent = TargetComponent;
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
	auto ProjOwner = GetOwner();
	if (OtherActor != ProjOwner)
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
				ProjOwner
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

void ABaseProjectileC::SetDamage(float & DamageVal)
{
	Damage = DamageVal;
}

void ABaseProjectileC::SetHealth(float & HealthVal)
{
	Health = HealthVal;
}

void ABaseProjectileC::SetSpeed(float & SpeedVal)
{
	Speed = SpeedVal;
}

void ABaseProjectileC::SetIsHoming(bool IsHoming)
{
	bIsHoming = IsHoming;
}

void ABaseProjectileC::SetHomingTarget(USceneComponent * TargetComponent)
{
	Target = TargetComponent;
}


