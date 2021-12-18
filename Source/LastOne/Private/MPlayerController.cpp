// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LastOne/LastOneCharacter.h"


void AMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&AMPlayerController::triggerPrimaryAttack);
	InputComponent->BindAction("Energy",IE_Pressed,this,&AMPlayerController::triggerEnergy);
}

void AMPlayerController::triggerPrimaryAttack_Implementation()
{
	APawn* pawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );
	ALastOneCharacter *mCharacter = Cast<ALastOneCharacter>(pawn);
	USActionComponent *actComp = Cast<USActionComponent>(pawn->GetComponentByClass(USActionComponent::StaticClass()));
	if(IsValid(mCharacter))
	{
		if(PrimaryAttackAnim && mCharacter->CanPlayMontage())
		{
			mCharacter->canMakeDamage = true;
			mCharacter->PlayAnimMontage(PrimaryAttackAnim);
		}
		actComp->setAttack();
	}
}

void AMPlayerController::triggerEnergy_Implementation()
{
	APawn* pawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );
	ALastOneCharacter *mCharacter = Cast<ALastOneCharacter>(pawn);
	mCharacter->TurnEnergyModeOn();
}
