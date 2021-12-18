// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Attack.h"

#include "LastOne/LastOneCharacter.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Actor  = MeshComp->GetOwner();
	ALastOneCharacter* mCharacter = Cast<ALastOneCharacter>(Actor);

	//感觉加在这个地方很怪
	if(IsValid(mCharacter))
	{
		mCharacter->canMakeDamage=false;
	}
}
