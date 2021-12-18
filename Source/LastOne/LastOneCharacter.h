// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/Character.h"
#include "LastOneCharacter.generated.h"



UCLASS(config=Game)
class ALastOneCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:
	/*Custom Component*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mat")
	UMaterial* Material_Normal;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mat")
	UMaterial* Material_Energy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mat")
	UMaterial* Material_Invincible;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent * ActionComp;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UStaticMeshComponent *Weapon;
	
	UPROPERTY(VisibleAnywhere, Category="EnergyMode")
	bool isEnergyMode;

	/*Constant*/
	UPROPERTY(VisibleAnywhere,Category="Speed")
	float MaxWalkSpeed_Energy;

	UPROPERTY(VisibleAnywhere,Category="Speed")
	float MaxWalkSpeed_Slide;

	UPROPERTY(VisibleAnywhere,Category="Speed")
	float MaxWalkSpeed_Guard;
	
	UPROPERTY(VisibleAnywhere,Category="Speed")
	float MaxWalkSpeed_Normal;

	
	
	// 不知道怎么处理连续动作的冲突关系
	TAtomic<bool> isSlashing;
	TAtomic<bool> GuardingState;
	
public:
	ALastOneCharacter();

	//Costom Function
	bool TurnEnergyModeOn();

	bool TurnEnergyModeOff();

	//没找到官方适合的API，自己控制把...
	bool CanPlayMontage();

	//如果动作更多可以考虑用TArray存起来进行抽象
	UFUNCTION(BlueprintCallable)
	bool isSliding();

	void Slide_On();

	void Slide_Off();
	UFUNCTION(BlueprintCallable)
	bool isGuarding();

	void Guard_On();

	void Guard_Off();
	//CompGetter
	USAttributeComponent* getAttributeComp();

	USActionComponent * getActionComp();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere,  Category="HUD")
	TSubclassOf<UUserWidget> MINION_HEALTH_WIDGET_CLASS;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool canMakeDamage;
	
	UPROPERTY()
	UUserWidget* MINION_HEALTH_WIDGET;
	
	virtual void PostInitializeComponents() override;
	
protected:
	FTimerHandle Invincible_Handle;

	FTimerDelegate Invincible_Callback;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void debug();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};

