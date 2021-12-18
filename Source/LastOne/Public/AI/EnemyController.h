// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class LASTONE_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BT")
	UBehaviorTree* BehaviorTree;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// UAIPerceptionComponent* PerceptionComponent;
	
	virtual void BeginPlay() override;
};
