// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "AI/EnemyA.h"
#include "LastOne/LastOneCharacter.h"
#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
	HealthMax = 100;
	Health = HealthMax;

	Rage = 20;
	RageMax = 100;
}

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}


bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}


float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}


void USAttributeComponent::SetDamage(float newDamamge)
{
	Damage = newDamamge;
}

float USAttributeComponent::GetDamage()
{
	return Damage;
}

void USAttributeComponent::InitHealth(float newHealthMax)
{
	HealthMax = newHealthMax;
	Health = HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged())
	{
		UE_LOG(LogTemp,Warning,TEXT("Invincible %s"),*FString(GetOwner()->GetName()));
		return false;
	}
	

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health - Delta, 0.0f, HealthMax);
	
	float ActualDelta = NewHealth - OldHealth;
	Health = NewHealth;
	MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	
	return ActualDelta != 0;
}


float USAttributeComponent::GetRage() const
{
	return Rage;
}


bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0;
}

void USAttributeComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
}


USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	int32 m=0;
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}


bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}


void USAttributeComponent::MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

bool USAttributeComponent::TurnOnEnergyMode()
{
	if(Rage==0.0f)
	{
		//Energy Not Enough
		return false;
	}

	GetWorld()->GetTimerManager().SetTimer(EnergyMode_Handle,this,&USAttributeComponent::MaintainEnergyMode,1.0f,true);
	return true;
}

void USAttributeComponent::MaintainEnergyMode()
{
	if(ApplyRage(GetOwner(),-5.0f))
	{
		//do nothing
	}else
	{
		TurnOffEnergyMode();
	}
}

bool USAttributeComponent::TurnOffEnergyMode()
{
	GetWorld()->GetTimerManager().PauseTimer(EnergyMode_Handle);
	ALastOneCharacter* mCharacter = Cast<ALastOneCharacter>(GetOwner());
	mCharacter->TurnEnergyModeOff();
	return true;
}




//Energy Mode Related




