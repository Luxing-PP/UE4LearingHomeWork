// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class LASTONE_API UBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckAttackRange();
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;
	
	/* Max desired attack range of AI pawn */
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxAttackRange;

	/* Min desired attack range of AI pawn */
	UPROPERTY(EditAnywhere, Category = "AI")
	float MinAttackRange;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
