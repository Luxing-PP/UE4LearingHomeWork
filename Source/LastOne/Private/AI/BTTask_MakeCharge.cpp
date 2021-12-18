// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MakeCharge.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyCharger.h"

EBTNodeResult::Type UBTTask_MakeCharge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AEnemyCharger* Enemy = Cast<AEnemyCharger>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	
	Enemy->canMakeDamage = true;
	Enemy->isCharging = true;
	
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	const AActor* EnemyActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("EnemyActor"));
	BlackBoardComp->SetValueAsVector(ChargeLocationKey.SelectedKeyName, EnemyActor->GetActorLocation());

	Enemy->AddMovementInput(EnemyActor->GetActorLocation(),1,true);
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Enemy);
	if (ensure(AttributeComp))
	{
		AttributeComp->SetDamage(20.0f);
	}
	return EBTNodeResult::Succeeded;
}
