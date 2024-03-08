// Fill out your copyright notice in the Description page of Project Settings.

#include "AffiliationComponent.h"

// Sets default values for this component's properties
UAffiliationComponent::UAffiliationComponent()
	: Affiliation(EAffiliation::None)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAffiliationComponent::SetAffiliation(EAffiliation NewAffiliation)
{
	Affiliation = NewAffiliation;
}

EAffiliation UAffiliationComponent::GetAffiliation()
{
	return Affiliation;
}
