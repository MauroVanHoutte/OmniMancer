// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementManipulationComponent.h"
#include <Components/BillboardComponent.h>


// Sets default values for this component's properties
UElementManipulationComponent::UElementManipulationComponent()
	: CurrentElements({WizardElement::Fire, WizardElement::Fire})
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UElementManipulationComponent::Initialize(APlayerController* PlayerController, UBillboardComponent* FirstBillboard, UBillboardComponent* SecondBillboard)
{
	Controller = PlayerController;
	FirstElementBillboard = FirstBillboard;
	SecondElementBillboard = SecondBillboard;

	if (FirstElementBillboard)
		FirstElementBillboard->SetSprite(*ElementTextures.Find(CurrentElements[0]));
	if (SecondElementBillboard)
		SecondElementBillboard->SetSprite(*ElementTextures.Find(CurrentElements[1]));

	SetupSpells();
}

void UElementManipulationComponent::AddElement(WizardElement Element)
{
	CurrentElements.Insert(Element, 0);
	WizardElement OldElement = CurrentElements.Pop();

	if (FirstElementBillboard)
		FirstElementBillboard->SetSprite(*ElementTextures.Find(CurrentElements[0]));
	if (SecondElementBillboard)
		SecondElementBillboard->SetSprite(*ElementTextures.Find(CurrentElements[1]));

	OnElementAdded.Broadcast(OldElement, Element);
}

void UElementManipulationComponent::TryCastSpell()
{
	const int SpellID = UElementLibrary::GetSpellIDFromElements(CurrentElements);
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	auto SpellClass = Spells.Find(SpellID);

	if (!SpellClass || TimerManager.IsTimerActive(*CooldownTimers.Find(*SpellClass)))
		return;
	
	TimerManager.SetTimer(*CooldownTimers.Find(*SpellClass), *Cooldowns.Find(*SpellClass), false);

	FVector MousePosAtActorHeight;
	FHitResult RaycastHit;
	RaycastMouseOnLevel(MousePosAtActorHeight, RaycastHit);
	const FVector ProjectileDirection = (MousePosAtActorHeight - GetOwner()->GetActorLocation()).GetSafeNormal(); // direction for projectile spells

	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto Spell = GetWorld()->SpawnActor<ABaseSpell>(*SpellClass, spawnParams);
	if (Spell != nullptr)
		Spell->InitSpell(MousePosAtActorHeight, GetOwner<APawn>()); //virtual init overriden in derived spells

	OnSpellCasted.Broadcast(GetOwner(), Spell);
}

// Called when the game starts
void UElementManipulationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UElementManipulationComponent::RaycastMouseOnLevel(FVector& mouseAtWizardHeight, FHitResult& raycastHit)
{
	FVector worldPos, worldDir;
	Controller->DeprojectMousePositionToWorld(worldPos, worldDir);
	FCollisionObjectQueryParams collisionParams{};
	collisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	GetWorld()->LineTraceSingleByObjectType(raycastHit, worldPos, worldPos + worldDir * 100000, collisionParams); //get mouse position on static level
	mouseAtWizardHeight = FMath::RayPlaneIntersection(worldPos, worldDir, FPlane(GetOwner()->GetActorLocation(), FVector(0, 0, 1)));
}

void UElementManipulationComponent::SetupSpells()
{
	for (FSpellConfig SpellConfig : SpellConfiguration)
	{
		Spells.Add(UElementLibrary::GetSpellIDFromElements(SpellConfig.ElementCombination), SpellConfig.Spell);

		if (!Cooldowns.Find(SpellConfig.Spell))
		{
			Cooldowns.Add(SpellConfig.Spell, 1);
		}

		CooldownTimers.Add(SpellConfig.Spell);
	}

	for (size_t i = 0; i < CurrentElements.Num(); i++)
	{
		OnElementAdded.Broadcast(WizardElement::None, CurrentElements[i]);
	}
}
