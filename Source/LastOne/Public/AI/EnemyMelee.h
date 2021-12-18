// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyA.h"
#include "EnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class LASTONE_API AEnemyMelee : public AEnemyA
{
	GENERATED_BODY()

	AEnemyMelee();
protected:
	virtual void BeginPlay() override;

	// 怪
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	// UStaticMeshComponent* WeaponComp;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	// UCapsuleComponent* WeaponCapsuleComp;
};
