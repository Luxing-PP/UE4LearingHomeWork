// Fill out your copyright notice in the Description page of Project Settings.


#include "EjectMechanism.h"

AEjectMechanism::AEjectMechanism()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComp->SetupAttachment(RootComponent);
}

void AEjectMechanism::BeginPlay()
{
	Super::BeginPlay();
	RadialForceComp->ForceStrength=0.0f;
}

void AEjectMechanism::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	GetWorld()->GetTimerManager().SetTimer(Eject_Handle,this,&AEjectMechanism::Eject,1.5f,false);
}

void AEjectMechanism::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	GetWorld()->GetTimerManager().PauseTimer(Eject_Handle);
	RadialForceComp->ForceStrength = 0.0f;
}

void AEjectMechanism::Eject()
{
	RadialForceComp->ForceStrength = 8000000.0f;
}
