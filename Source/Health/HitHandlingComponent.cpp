// Fill out your copyright notice in the Description page of Project Settings.


#include "HitHandlingComponent.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "AffiliationComponent.h"

// Sets default values for this component's properties
UHitHandlingComponent::UHitHandlingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHitHandlingComponent::Initialize(const TArray<UShapeComponent*>& inColliders, UAffiliationComponent* inAffiliationComponent)
{
	Colliders = inColliders;
	AffiliationComponent = inAffiliationComponent;

	for (UShapeComponent* Collider : Colliders)
	{
		Collider->OnComponentBeginOverlap.AddDynamic(this, &UHitHandlingComponent::OnColliderOverlap);
	}
}

// Called when the game starts
void UHitHandlingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHitHandlingComponent::OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseSpell* spell = Cast<ABaseSpell>(OtherActor);
	if (!spell)
		return;

	UAffiliationComponent* SpellAffiliation = spell->GetOwner() ? spell->GetOwner()->GetComponentByClass<UAffiliationComponent>() : nullptr;

	if ((!SpellAffiliation || SpellAffiliation->GetAffiliation() != AffiliationComponent->GetAffiliation()) 
		&& !spell->WasActorHit(GetOwner()))
	{
		spell->AddHitActor(GetOwner());
		GetOwner()->TakeDamage(spell->GetFinalDamage(), FDamageEvent{}, spell->GetInstigatorController(), spell);
		//ReapplyStatusEffects(spell->GetStatusEffects());
		//activates caster on hit trigger
		spell->OnHit(GetOwner());
	}
}

// Called every frame
void UHitHandlingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

