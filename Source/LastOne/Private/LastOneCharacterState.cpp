// Fill out your copyright notice in the Description page of Project Settings.


#include "LastOneCharacterState.h"

int32 ALastOneCharacterState::GetCredits() const
{
	return credits;
}

void ALastOneCharacterState::AddCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f))
	{
		return;
	}

	credits += Delta;

	OnCreditsChanged.Broadcast(this, credits, Delta);
}

bool ALastOneCharacterState::RemoveCredits(int32 Delta)
{
	
	if (!ensure(Delta >= 0.0f))
	{
		return false;
	}

	if (credits < Delta)
	{
		// Not enough credits available
		return false;
	}

	credits -= Delta;

	OnCreditsChanged.Broadcast(this, credits, -Delta);

	return true;
}
