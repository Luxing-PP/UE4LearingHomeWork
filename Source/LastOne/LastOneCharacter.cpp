// Copyright Epic Games, Inc. All Rights Reserved.

#include "LastOneCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "LastOneGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ALastOneCharacter
void ALastOneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(MINION_HEALTH_WIDGET_CLASS!=nullptr)
	{
		MINION_HEALTH_WIDGET = CreateWidget(GetWorld(),MINION_HEALTH_WIDGET_CLASS);
		MINION_HEALTH_WIDGET->AddToViewport();
	}
}

void ALastOneCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALastOneCharacter::debug()
{
	ALastOneGameMode* GM =  GetWorld()->GetAuthGameMode<ALastOneGameMode>();
	if(GM)
	{
		GM->showGameOverGUI();
	}
}

ALastOneCharacter::ALastOneCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// set Component
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	AttributeComp->SetDamage(5.0f);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure Custom Constants
	MaxWalkSpeed_Guard  = 300.f;
	MaxWalkSpeed_Normal = 600.f;
	MaxWalkSpeed_Energy = 900.f;
	MaxWalkSpeed_Slide  = 1200.f;
	canMakeDamage = false;
	isEnergyMode = false;
	isSlashing = false;
	GuardingState = false;

	Invincible_Callback.BindLambda([&]
	{
		GetMesh()->SetMaterial(0,Material_Normal);
		SetCanBeDamaged(true);
	});
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Normal;
	

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}



bool ALastOneCharacter::TurnEnergyModeOn()
{
	
	if(AttributeComp->TurnOnEnergyMode())
	{
		// 加速
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Energy;
		// 加下特效
		UCameraComponent* CameraComponent = GetFollowCamera();
		CameraComponent->PostProcessSettings.SceneColorTint=FLinearColor(FColor(200,200,0));
		CameraComponent->PostProcessSettings.bOverride_SceneColorTint=true;
		
		// GetFollowCamera()->RefreshVisualRepresentation();
		GetMesh()->SetMaterial(0,Material_Energy);
		// EnergyPostProcessComp->SetPhysicsVolume(,)
		isEnergyMode = true;
		return true;
	}

	return false;
}

bool ALastOneCharacter::TurnEnergyModeOff()
{
	// Feature: Velocity Up
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Normal;

	// Rendering: 
	UCameraComponent* CameraComponent = GetFollowCamera();
	CameraComponent->PostProcessSettings.SceneColorTint=FLinearColor(FColor(111,111,111));
	CameraComponent->PostProcessSettings.bOverride_SceneColorTint=false;
	GetMesh()->SetMaterial(0,Material_Normal);

	
	isEnergyMode = false;
	return true;
}

bool ALastOneCharacter::CanPlayMontage()
{
	return !(isSlashing||GuardingState);
}

bool ALastOneCharacter::isSliding()
{
	return isSlashing;
}

void ALastOneCharacter::Slide_On()
{
	isSlashing = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Slide;
}

void ALastOneCharacter::Slide_Off()
{
	isSlashing = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Normal;
}

bool ALastOneCharacter::isGuarding()
{
	return GuardingState;
}

void ALastOneCharacter::Guard_On()
{
	GuardingState = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Guard;
}

void ALastOneCharacter::Guard_Off()
{
	GuardingState = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed_Normal;
}

USAttributeComponent* ALastOneCharacter::getAttributeComp()
{
	return AttributeComp;
}

USActionComponent* ALastOneCharacter::getActionComp()
{
	return ActionComp;	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALastOneCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALastOneCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALastOneCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALastOneCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALastOneCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALastOneCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALastOneCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALastOneCharacter::OnResetVR);

	// handle slide
	PlayerInputComponent->BindAction("Slide",IE_Pressed,this,&ALastOneCharacter::Slide_On);
	PlayerInputComponent->BindAction("Slide",IE_Released,this,&ALastOneCharacter::Slide_Off);

	// handle guard
	PlayerInputComponent->BindAction("Guard",IE_Pressed,this,&ALastOneCharacter::Guard_On);
	PlayerInputComponent->BindAction("Guard",IE_Released,this,&ALastOneCharacter::Guard_Off);

	// debug
	PlayerInputComponent->BindAction("Debug", IE_Pressed, this, &ALastOneCharacter::debug);
}



void ALastOneCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this,&ALastOneCharacter::OnHealthChanged);
}

void ALastOneCharacter::OnResetVR()
{
	// If LastOne is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in LastOne.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALastOneCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALastOneCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALastOneCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALastOneCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALastOneCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALastOneCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//Delegate Function
void ALastOneCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	int32 m=0;
	if(Delta < -3.0f)
	{
		// 开无敌
		SetCanBeDamaged(false);
		GetMesh()->SetMaterial(0,Material_Invincible);
		GetWorld()->GetTimerManager().SetTimer(Invincible_Handle,Invincible_Callback, 3.0f,false);
	}
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		GetMesh()->SetSimulatePhysics(true);
		ALastOneGameMode* GM =  GetWorld()->GetAuthGameMode<ALastOneGameMode>();
		if(GM)
		{
			GM->showGameOverGUI();
		}
	}
}



