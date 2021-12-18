// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyCharger.h"

#include <string>

#include "Components/CapsuleComponent.h"
#include "LastOne/LastOneCharacter.h"

AEnemyCharger::AEnemyCharger()
{
	isAlert = false;
	canMakeDamage = false;
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComp->ForceStrength=0.0f;
}

void AEnemyCharger::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this,&AEnemyA::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharger::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this,&AEnemyCharger::OnOverlapEnd);
}

void AEnemyCharger::resetForce()
{
	RadialForceComp->ForceStrength=0.0f;
}

void AEnemyCharger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	int32 m = 0;
	if(!canMakeDamage){
		UE_LOG(LogTemp,Warning,TEXT("Can'tMake Damage"))
		return;
	}

	ALastOneCharacter* mCharacter = Cast<ALastOneCharacter>(OtherActor);
	if(!mCharacter)
	{
		mCharacter = Cast<ALastOneCharacter>(OtherActor->GetOwner());
	}
	
	if(mCharacter)
	{
		USAttributeComponent* playerAttr = USAttributeComponent::GetAttributes(mCharacter);
		playerAttr->ApplyHealthChange(this,AttributeComp->GetDamage());

		if(isCharging){
			RadialForceComp->ForceStrength = 8000000.0f * 2 ;
			UE_LOG(LogTemp,Warning,TEXT("Charge %s"),*FString(__FUNCTION__));
			UE_LOG(LogTemp,Warning,TEXT("Charge %d"),RadialForceComp->ForceStrength);
		}
	}
	
	
}

void AEnemyCharger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// FTimerHandle Force_handler;
	// GetWorldTimerManager().SetTimer(Force_handler,this,&AEnemyCharger::resetForce,3.0f,false);
	// UE_LOG(LogTemp,Warning,TEXT("Charge %d"),RadialForceComp->ForceStrength);
}
