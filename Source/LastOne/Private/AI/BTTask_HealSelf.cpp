// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_HealSelf.h"
#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type UBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, -10);
	}
	return EBTNodeResult::Succeeded;
}
