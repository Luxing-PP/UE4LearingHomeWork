// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerAttack);

UCLASS()
class LASTONE_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerAttack OnPlayerAttack;

	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* PrimaryAttackAnim;


	
	UFUNCTION(BlueprintNativeEvent)
	void triggerPrimaryAttack();
	virtual void triggerPrimaryAttack_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void triggerEnergy();
	virtual void triggerEnergy_Implementation();
	
	virtual  void SetupInputComponent() override;
};
