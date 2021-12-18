// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBall.h"

// Sets default values
ADamageBall::ADamageBall()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	// Enabling Simulate physics automatically changes the Profile to PhysicsActor in Blueprint, in C++ we need to change this manually.
	// MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName); 
	RootComponent = MeshComp;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	HitBox->OnComponentHit.AddDynamic(this, &ADamageBall::OnHit);

	InitialForce = FVector(0);
	InitialTorque = FVector(0);
}

// Called when the game starts or when spawned
void ADamageBall::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimeHandle_ChangedMaterial;
	GetWorldTimerManager().SetTimer(TimeHandle_ChangedMaterial, this, &ADamageBall::ChangeMaterial,
									FMath::RandRange(1, 2), true);

	MeshComp->AddForce(InitialForce);
	MeshComp->AddTorqueInRadians(InitialTorque);
}

// Called every frame
void ADamageBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageBall::ChangeMaterial()
{
	if(times%2==0)
	{
		MeshComp->SetMaterial(0,MaterialA);
	}else
	{
		MeshComp->SetMaterial(0,MaterialB);
	}

	times++;
}

void ADamageBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::White,
		FString::Printf(TEXT("Hit:%s"),*OtherActor->GetName()));
}
