// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyA.h"

#include "LastOneCharacterState.h"
#include "AI/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LastOne/LastOneCharacter.h"
#include "LastOne/LastOneGameMode.h"

class AGameStateBase;

// Sets default values
AEnemyA::AEnemyA()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	// Can be set in blueprint also
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetMesh()->SetGenerateOverlapEvents(true);

	EnemyActorKey = "EnemyActor";

	//
	isAlert = false;
}

// Called when the game starts or when spawned
void AEnemyA::BeginPlay()
{
	Super::BeginPlay();
	AttributeComp->InitHealth(FMath::Clamp(GetWorld()->GetTimeSeconds(),20.0f,300.0f));
	AttributeComp->ApplyHealthChange(this,0.0f);
}

void AEnemyA::SetEnemyActor(AActor* NewTarget)
{
	AEnemyController* AEC = Cast<AEnemyController>(GetController());
	if(AEC)
	{
		AEC->GetBlackboardComponent()->SetValueAsObject(EnemyActorKey,NewTarget);
	}
}

AActor* AEnemyA::GetEnemyActor() const
{
	AEnemyController* AEC = Cast<AEnemyController>(GetController());
	if(AEC)
	{
		return Cast<AActor>(AEC->GetBlackboardComponent()->GetValueAsObject(EnemyActorKey));
	}
	return nullptr;
}

void AEnemyA::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//UE4 不允许歧义名字只能叫OnPawnSeen了...
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyA::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AEnemyA::OnHealthChanged);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyA::OnOverlapBegin);
}

void AEnemyA::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(!AttributeComp->IsAlive())
	{
		UE_LOG(LogTemp,Warning,TEXT("Died of Enemy %s"),*(this->GetName()));

		AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
		ALastOneGameMode* GameMode = Cast<ALastOneGameMode>(GameModeBase);
		APawn* InstigatorPawn =  Cast<APawn>(InstigatorActor);
		//不知道为何不能直接获取PlayerState
		
		if(IsValid(GameMode))
		{
			GameMode->SpawnHealthActor(this->GetActorLocation(),this->GetActorRotation(),4);
			// Credit is depend on the health
			
			if(ensure(InstigatorActor)&&IsValid(InstigatorPawn))
			{
				ALastOneCharacterState* PS = InstigatorPawn->GetPlayerState<ALastOneCharacterState>();
				PS->AddCredits(AttributeComp->GetHealthMax()*10);

				USAttributeComponent* playerAttr =  USAttributeComponent::GetAttributes(InstigatorActor);
				playerAttr->ApplyRage(this,20.0f);
			}
		}else
		{
			UE_LOG(LogTemp,Warning,TEXT("Cast Fail %s"),*(this->GetName()));
		}
		
		this->Destroy();
	}
	
}

void AEnemyA::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!canMakeDamage){
	    UE_LOG(LogTemp,Warning,TEXT("Can'tMake Damage"))
	    return;
	}

	ALastOneCharacter* mCharacter = Cast<ALastOneCharacter>(OtherActor);
	if(mCharacter)
	{
		USAttributeComponent* playerAttr = USAttributeComponent::GetAttributes(mCharacter);
		playerAttr->ApplyHealthChange(this,AttributeComp->GetDamage());
	}
}

void AEnemyA::OnPawnSeen(APawn* Pawn)
{
	ResetTaunt_TimerHandle.Invalidate();
	if (GetEnemyActor() != Pawn)
	{
		SetEnemyActor(Pawn);
		GetWorldTimerManager().SetTimer(ResetTaunt_TimerHandle,this,&AEnemyA::ResetTaunt,10.0f);
	}
}

void AEnemyA::ResetTaunt()
{
	//TODO 不好处理这= =
	SetEnemyActor(nullptr);
}

void AEnemyA::UpdateWalkSpeed(float newSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed=newSpeed;
}


// Called every frame
void AEnemyA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyA::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

