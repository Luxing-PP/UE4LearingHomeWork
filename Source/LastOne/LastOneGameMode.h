// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyA.h"
#include "GameFramework/GameModeBase.h"
#include "LastOneGameMode.generated.h"


UCLASS(minimalapi)
class ALastOneGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALastOneGameMode();

	virtual void Tick(float DeltaSeconds) override;
	
	void SpawnHealthActor(FVector SpawnPosition, FRotator SpawnRotation, int32 num);

	UFUNCTION(BlueprintImplementableEvent)
	void showGameOverGUI();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyA> EnemyActorMelee;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyA> EnemyActorCharger;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> HealthActor;

	UPROPERTY(EditDefaultsOnly)
	bool canSpawnEnemy;
	
	UPROPERTY(EditDefaultsOnly)
	float Spawn_Z = 300.0f;

	UPROPERTY(EditDefaultsOnly)
	float Spawn_Range = 800.0f;
	
	void SpawnEnemy();
};



