// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyA.generated.h"

UCLASS()
class LASTONE_API AEnemyA : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyA();

	// State Property (东西不多不专门弄State了)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="State")
	bool isAlert;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="State")
	bool canMakeDamage;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	virtual  void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Bot Comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	
	/* Key for AI Blackboard 'EnemyActor' */
	UPROPERTY(VisibleAnywhere, Category = "BlackBoardKey")
	FName EnemyActorKey;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetEnemyActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetEnemyActor() const;

	virtual void PostInitializeComponents() override;
	

	//Seen Relative
	FTimerHandle ResetTaunt_TimerHandle;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void ResetTaunt();
	
	UFUNCTION(BlueprintCallable)
	void UpdateWalkSpeed(float newSpeed);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
