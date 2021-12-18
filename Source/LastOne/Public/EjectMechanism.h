// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PressMechanism.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "EjectMechanism.generated.h"

/**
 * 
 */
UCLASS()
class LASTONE_API AEjectMechanism : public APressMechanism
{
	GENERATED_BODY()
	
	AEjectMechanism();


	
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Force")
	URadialForceComponent* RadialForceComp;
	
	virtual void BeginPlay() override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	FTimerHandle Eject_Handle;
	
	void Eject();
};
