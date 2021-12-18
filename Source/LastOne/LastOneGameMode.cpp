// Copyright Epic Games, Inc. All Rights Reserved.


#include "LastOneGameMode.h"
#include "LastOneCharacter.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

ALastOneGameMode::ALastOneGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALastOneGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



void ALastOneGameMode::BeginPlay()
{
	Super::BeginPlay();


	FTimerHandle SpawnEnemy_Handle;
	GetWorldTimerManager().SetTimer(SpawnEnemy_Handle, this, &ALastOneGameMode::SpawnEnemy,
	                                FMath::RandRange(3, 8), true);
	
}

void ALastOneGameMode::SpawnEnemy()
{
	if(!canSpawnEnemy)return;
	
	// Count alive Enemies before spawning
	int32 NrOfAliveBots = 0;
	for (TActorIterator<AEnemyA> It(GetWorld()); It; ++It)
	{
		AEnemyA* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Num of alive bots is %i"), NrOfAliveBots);

	if(NrOfAliveBots<8)
	{
		//在PC（人物）的周围+-500随机刷怪
		const APlayerController* PC = GetWorld()->GetFirstPlayerController();
		const FVector LocalPosition = PC->GetFocalLocation();
		FVector SpawnPosition;
		do
		{
			float RandX = FMath::RandRange(LocalPosition.X - Spawn_Range, LocalPosition.X + Spawn_Range);
			float RandY = FMath::RandRange(LocalPosition.Y - Spawn_Range, LocalPosition.Y + Spawn_Range);
			SpawnPosition = FVector(RandX, RandY, Spawn_Z);
		}while (FVector::Distance(LocalPosition,SpawnPosition)<=400);
		
		
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,TEXT("SpawningActor"));

		int8 rand = FMath::RandRange(0,2);
		switch (rand)
		{
			case 0:
				GetWorld()->SpawnActor(EnemyActorMelee, &SpawnPosition, &SpawnRotation);
				break;
			case 1:
				GetWorld()->SpawnActor(EnemyActorCharger, &SpawnPosition, &SpawnRotation);
				break;
			default:
				break;
		}
		
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Too Many bots:%i , Can't Spawn"), NrOfAliveBots);
	}
}

void ALastOneGameMode::SpawnHealthActor(FVector SpawnPosition, FRotator SpawnRotation, int32 num)
{
	for (int i=0;i<num;++i)
	{
		const float RandX = FMath::RandRange(SpawnPosition.X - 200, SpawnPosition.X + 200);
		const float RandY = FMath::RandRange(SpawnPosition.Y - 200, SpawnPosition.Y + 200);

		FVector realPosition = FVector(RandX, RandY, SpawnPosition.Z+200);
		GetWorld()->SpawnActor(HealthActor, &realPosition, &SpawnRotation);	
	}
}

