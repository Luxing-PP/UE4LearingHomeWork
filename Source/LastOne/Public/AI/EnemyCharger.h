// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyA.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "EnemyCharger.generated.h"

/**
 * 
 */
UCLASS()
class LASTONE_API AEnemyCharger : public AEnemyA
{
	GENERATED_BODY()
	
public:
	AEnemyCharger();
	
	UPROPERTY(BlueprintReadWrite,Category="State")
	bool isCharging;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Force")
	URadialForceComponent* RadialForceComp;
	
protected:
	virtual void BeginPlay() override;

	void resetForce();
	
public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
