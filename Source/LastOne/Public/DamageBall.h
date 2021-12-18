// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DamageBall.generated.h"
class UStaticMeshComponent;
class UCapsuleComponent;
class UBoxComponent;

UCLASS()
class LASTONE_API ADamageBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* HitBox;

	int32 times=0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mat")
	UMaterial* MaterialA;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mat")
	UMaterial* MaterialB;

	UPROPERTY(EditAnywhere,Category="ForceV")
	FVector InitialForce;

	UPROPERTY(EditAnywhere,Category="ForceV")
	FVector InitialTorque;
	
	
private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor*  OtherActor,
		UPrimitiveComponent*  OtherComp, FVector  NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ChangeMaterial();
};
