// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHealthComponent.h"

// Sets default values for this component's properties
UBaseHealthComponent::UBaseHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	AActor* Owner = GetOwner();
	if (!Owner->OnTakeAnyDamage.IsBound())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UBaseHealthComponent::TakeDamage);
	}

	if (bStartFullHealth)
	{
		CurrentHealth = MaxHealth;
	}

	if (bStartDepleted)
	{
		bIsDepleted = true;
		CurrentHealth = 0;
	}
}


// Called every frame
void UBaseHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	DamageTakenDelegate.Broadcast(this, Damage, DamageType, InstigatedBy, DamageCauser);

	if (CurrentHealth < 0)
	{
		const float OverkillDamage = -1 * CurrentHealth;
		CurrentHealth = 0.f;
		bIsDepleted = true;
		FatalDamageTakenDelegate.Broadcast(this, Damage, OverkillDamage, DamageType, InstigatedBy, DamageCauser);
	}
}

float UBaseHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UBaseHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	ensure(NewMaxHealth > 0);
	MaxHealth = NewMaxHealth;
}

float UBaseHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UBaseHealthComponent::SetCurrentHealth(float NewCurrentHealth, bool ClampToMaxHealth)
{
	ensure(NewCurrentHealth > 0);
	CurrentHealth = ClampToMaxHealth ? FMath::Min(NewCurrentHealth, MaxHealth) : NewCurrentHealth;

	bIsDepleted = !(CurrentHealth > 0);
}

float UBaseHealthComponent::GetHealthPercentage() const
{
	return CurrentHealth/MaxHealth;
}

void UBaseHealthComponent::SetCurrentHealthPercentage(float NewPercentage)
{
	CurrentHealth = MaxHealth * NewPercentage;

	if (bIsDepleted && CurrentHealth > 0)
	{
		bIsDepleted = true;
	}
}

void UBaseHealthComponent::Kill(const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth = 0;
	bIsDepleted = true;
	DamageTakenDelegate.Broadcast(this, -1, DamageType, InstigatedBy, DamageCauser);
	FatalDamageTakenDelegate.Broadcast(this, -1, -1, DamageType, InstigatedBy, DamageCauser);
}

bool UBaseHealthComponent::IsDepleted() const
{
	return bIsDepleted;
}

float UBaseHealthComponent::GetPriority() const
{
	return Priority;
}

bool UBaseHealthComponent::GetBlocksDamage() const
{
	return bBlockDamage;
}

bool UBaseHealthComponent::GetOverflows() const
{
	return bOverflow;
}