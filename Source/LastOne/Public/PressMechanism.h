// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PressMechanism.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMechanismPress,AActor*,InstigatorActor);

UCLASS()
class LASTONE_API APressMechanism : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressMechanism();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* HitBoxComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mat")
	UMaterial* Material_Press_Off;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mat")
	UMaterial* Material_Press_On;

	UPROPERTY(BlueprintAssignable)
	FOnMechanismPress OnMechanismPress;
	
	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
