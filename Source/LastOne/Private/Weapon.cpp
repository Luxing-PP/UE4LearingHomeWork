// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "AI/EnemyA.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(MeshComp);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(MeshComp);
}

void AWeapon::WeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyA* Enemy = Cast<AEnemyA>(OtherActor);
	ALastOneCharacter* player = Cast<ALastOneCharacter>(UGameplayStatics::GetPlayerPawn(this,0)) ;
	
	if(Enemy&&player->canMakeDamage)
	{
		USAttributeComponent* enemyAttr = USAttributeComponent::GetAttributes(Enemy);
		USAttributeComponent* playerAttr = USAttributeComponent::GetAttributes(player);
		enemyAttr->ApplyHealthChange(player, playerAttr->GetDamage());
	}
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor)
	{
		ALastOneCharacter* LastOneCharacter = Cast<ALastOneCharacter>(OtherActor);
		if(LastOneCharacter)
		{
			Equip(LastOneCharacter);	
		}
	}
}

void AWeapon::Equip(ALastOneCharacter* MOwner)
{
	int32 m=0;
	if(MOwner)
	{
		//如果不想无效化呢？
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetSimulatePhysics(false);
		AttachToComponent(MOwner->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,"WeaponSocket");
		SphereComp->OnComponentBeginOverlap.RemoveDynamic(this,&AItem::OnOverlapBegin);
		SphereComp->OnComponentBeginOverlap.RemoveDynamic(this,&AWeapon::OnOverlapBegin);
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::WeaponOverlap);
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

