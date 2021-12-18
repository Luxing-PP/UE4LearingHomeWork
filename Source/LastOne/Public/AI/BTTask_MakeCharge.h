// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask_MakeAttack_Base.h"
#include "BTTask_MakeCharge.generated.h"

/**
 * 
 */
UCLASS()
class LASTONE_API UBTTask_MakeCharge : public UBTTask_MakeAttack_Base
{
	GENERATED_BODY()

public:
	//Optional
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector ChargeLocationKey;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
