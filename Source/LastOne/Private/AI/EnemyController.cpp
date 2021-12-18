// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyController.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyController::AEnemyController()
{
	// PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComp");
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		const bool bSuccess = RunBehaviorTree(BehaviorTree);
		if(!bSuccess)
		{
			UE_LOG(LogTemp,Warning,TEXT("RunBehaviour Tree Flase"));
		}
		
	}
}
