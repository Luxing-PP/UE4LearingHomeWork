// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MakeAttack_Base.h"

#include "SAttributeComponent.h"
#include "AIController.h"
#include "AI/EnemyA.h"

//后来发现没什么必要... 这个继承没法很好去重，因为中间节点要复用
EBTNodeResult::Type UBTTask_MakeAttack_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyA* Enemy = Cast<AEnemyA>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	Enemy->canMakeDamage = true;
	
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Enemy);
	if (ensure(AttributeComp))
	{
		AttributeComp->SetDamage(10.0f);
	}
	return EBTNodeResult::Succeeded;
}
