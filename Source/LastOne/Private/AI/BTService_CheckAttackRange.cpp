// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	MaxAttackRange = 200.f;
	MinAttackRange = 0.f;
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* EnemyActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("EnemyActor"));
		if (EnemyActor)
		{
			 
			AAIController* AIC = OwnerComp.GetAIOwner();

			APawn* AIPawn = AIC->GetPawn();
			if (ensure(AIPawn))
			{
				float DistanceTo = FVector::Distance(EnemyActor->GetActorLocation(), AIPawn->GetActorLocation());

				bool bWithinRange = (MinAttackRange < DistanceTo)  && (DistanceTo < MaxAttackRange);

				bool bHasLOS = false;
				if (bWithinRange)
				{
					bHasLOS = AIC->LineOfSightTo(EnemyActor);
				}
				
				BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
			}
		}
	}
}
