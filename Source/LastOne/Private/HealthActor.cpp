// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthActor.h"

#include "LastOne/LastOneCharacter.h"

// Sets default values
AHealthActor::AHealthActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealthActor::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this,&AHealthActor::OnOverlapBegin);
}

void AHealthActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{   
    ALastOneCharacter* Player =  Cast<ALastOneCharacter>(OtherActor);
	if(IsValid(Player))
	{
		Player->getAttributeComp()->ApplyHealthChange(this,-10);	
	}
	OverlappedComp->GetAttachmentRootActor()->Destroy();
}



