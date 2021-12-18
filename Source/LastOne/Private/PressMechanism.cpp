// Fill out your copyright notice in the Description page of Project Settings.


#include "PressMechanism.h"

// Sets default values
APressMechanism::APressMechanism()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	RootComponent = HitBoxComponent;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APressMechanism::BeginPlay()
{
	Super::BeginPlay();

	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this,&APressMechanism::OnOverlapBegin);
	HitBoxComponent->OnComponentEndOverlap.AddDynamic(this,&APressMechanism::OnOverlapEnd);
}

// Called every frame
void APressMechanism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APressMechanism::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnMechanismPress.Broadcast(OtherActor);
	MeshComp->SetMaterial(0,Material_Press_On);
}

void APressMechanism::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MeshComp->SetMaterial(0,Material_Press_Off);
}
