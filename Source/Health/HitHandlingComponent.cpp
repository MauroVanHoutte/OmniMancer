// Fill out your copyright notice in the Description page of Project Settings.


#include "HitHandlingComponent.h"
#include "AffiliationComponent.h"
#include "SpellCasting/Spells/BaseSpell.h"
#include "Health/HitTriggerInterface.h"

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

void UHitHandlingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (UShapeComponent* Collider : Colliders)
	{
		Collider->OnComponentBeginOverlap.RemoveDynamic(this, &UHitHandlingComponent::OnColliderOverlap);
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
	if (!OtherActor->Implements<UHitTriggerInterface>())
		return;

	UAffiliationComponent* Affiliation = IHitTriggerInterface::Execute_GetAffiliation(OtherActor);

	if ((!Affiliation || Affiliation->GetAffiliation() != AffiliationComponent->GetAffiliation()) //Damage collider is not affiliated with hit actor
		&& !IHitTriggerInterface::Execute_WasActorHitBefore(OtherActor, GetOwner(), OtherComp)) //Hit Actor was not hit before
	{
		IHitTriggerInterface::Execute_OnTriggered(OtherActor, GetOwner(), OtherComp);
		OnHitRegisteredDelegate.Broadcast(OtherActor, GetOwner());
	}
}

// Called every frame
void UHitHandlingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

