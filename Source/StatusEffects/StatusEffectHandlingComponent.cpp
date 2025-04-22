// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectHandlingComponent.h"
#include "ActiveStatusEffectsBar.h"
#include "StatusEffectPoolingSubsystem.h"
#include "StatusEffect.h"

// Sets default values for this component's properties
UStatusEffectHandlingComponent::UStatusEffectHandlingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStatusEffectHandlingComponent::BindStatusEffectsBar(UActiveStatusEffectsBar* Widget)
{
	StatusEffectsBar = Widget;
}

void UStatusEffectHandlingComponent::ClearStatusEffects()
{
	for (UBaseStatusEffect* StatusEffect : ActiveStatusEffects)
	{
		StatusEffect->Remove(GetOwner(), ActiveStatusEffects);
		StatusEffectPooling->ReturnStatusEffectInstance(StatusEffect);
		if (IsValid(StatusEffectsBar))
		{
			StatusEffectsBar->RemoveStatusEffect(StatusEffect);
		}
	}
	ActiveStatusEffects.Empty();
}


// Called when the game starts
void UStatusEffectHandlingComponent::BeginPlay()
{
	Super::BeginPlay();
	StatusEffectPooling = GetWorld()->GetGameInstance()->GetSubsystem<UStatusEffectPoolingSubsystem>();
	// ...
	
}

void UStatusEffectHandlingComponent::OnUnregister()
{
	Super::OnUnregister();
	for (UBaseStatusEffect* StatusEffect : ActiveStatusEffects)
	{
		StatusEffect->Remove(GetOwner(), ActiveStatusEffects);
		StatusEffectPooling->ReturnStatusEffectInstance(StatusEffect);
	}
}

// Called every frame
void UStatusEffectHandlingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* AffectedActor = GetOwner();

	for (size_t i = 0; i < ActiveStatusEffects.Num(); i++)
	{
		ActiveStatusEffects[i]->Update(DeltaTime, AffectedActor);

		if (ActiveStatusEffects[i]->IsExpired(AffectedActor))
		{
			ActiveStatusEffects[i]->Remove(AffectedActor, ActiveStatusEffects);

			if (IsValid(StatusEffectsBar))
			{
				StatusEffectsBar->RemoveStatusEffect(ActiveStatusEffects[i]);
			}

			ensure(StatusEffectPooling);
			StatusEffectPooling->ReturnStatusEffectInstance(ActiveStatusEffects[i]);
			ActiveStatusEffects[i] = ActiveStatusEffects[ActiveStatusEffects.Num() - 1];
			ActiveStatusEffects.RemoveAt(ActiveStatusEffects.Num() - 1);
			i--;
		}
	}

	if (IsValid(StatusEffectsBar))
	{
		StatusEffectsBar->UpdateTimers(ActiveStatusEffects);
	}
}

void UStatusEffectHandlingComponent::ApplyStatusEffect(UBaseStatusEffect* StatusEffect)
{
	AActor* TargetActor = GetOwner();
	ensure(StatusEffectPooling);
	UBaseStatusEffect* StatusEffectInstance = StatusEffectPooling->GetStatusEffectInstance(StatusEffect);
	if (StatusEffectInstance->Apply(TargetActor, ActiveStatusEffects))
	{
		ActiveStatusEffects.Add(StatusEffectInstance);
		if (IsValid(StatusEffectsBar) && StatusEffectInstance->Icon)
		{
			StatusEffectsBar->AddStatusEffect(StatusEffectInstance);
		}
	}
	else
	{
		StatusEffectPooling->ReturnStatusEffectInstance(StatusEffectInstance);
	}
}

