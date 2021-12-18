// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyMelee.h"

#include "Components/CapsuleComponent.h"

AEnemyMelee::AEnemyMelee()
{
	AttributeComp->SetDamage(10.0f);
	// WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>("WeaponComp");
	//
	// WeaponCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("WeaponCapsuleComp");
	// WeaponCapsuleComp->SetupAttachment(WeaponComp);
}

void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
	// WeaponComp->SetupAttachment(GetMesh(),"MeleeSocket");
}


