// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementManipulationComponent.h"
#include "NiagaraComponent.h" 
#include "Upgrades/StatUpgrades/StatComponent.h"


// Sets default values for this component's properties
UElementManipulationComponent::UElementManipulationComponent()
	: CurrentElements({WizardElement::Fire, WizardElement::Fire})
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UElementManipulationComponent::Initialize(APlayerController* PlayerController, UNiagaraComponent* LeftParticle, UNiagaraComponent* RightParticle, UStatComponent* StatComponent)
{
	Controller = PlayerController;
	Stats = StatComponent;
	LeftHandElementParticle = LeftParticle;
	RightHandElementParticle = RightParticle;

	if (LeftHandElementParticle)
		LeftHandElementParticle->SetVariableLinearColor(ColorVariableName, *ElementColors.Find(CurrentElements[0]));
	if (RightHandElementParticle)
		RightHandElementParticle->SetVariableLinearColor(ColorVariableName, *ElementColors.Find(CurrentElements[1]));

	SetupSpells();
}

void UElementManipulationComponent::AddElement(WizardElement Element)
{
	if (LearnedElements.Contains(Element))
	{
		CurrentElements.Insert(Element, 0);
		WizardElement OldElement = CurrentElements.Pop();

		if (bAddingElementLeft)
			LeftHandElementParticle->SetVariableLinearColor(ColorVariableName, *ElementColors.Find(Element));
		else
			RightHandElementParticle->SetVariableLinearColor(ColorVariableName, *ElementColors.Find(Element));

		bAddingElementLeft = !bAddingElementLeft;

		OnElementAddedDelegate.Broadcast(OldElement, Element);
	}
}

void UElementManipulationComponent::TryCastSpell()
{
	const int SpellID = UElementLibrary::GetSpellIDFromElements(CurrentElements);
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TSubclassOf<ABaseSpell>* SpellClass = Spells.Find(SpellID);

	if (!SpellClass || TimerManager.IsTimerActive(*CooldownTimers.Find(*SpellClass)))
		return;
	
	TimerManager.SetTimer(*CooldownTimers.Find(*SpellClass), CalculateSpellCooldown(*SpellClass), false);

	FVector MousePosAtActorHeight;
	FHitResult RaycastHit;
	RaycastMouseOnLevel(MousePosAtActorHeight, RaycastHit);

	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = IsValid(SpellOriginPoint) ? SpellOriginPoint->GetComponentLocation() : GetOwner()->GetActorLocation();
	ABaseSpell* Spell = GetWorld()->SpawnActor<ABaseSpell>(*SpellClass, SpawnLocation, FRotator(), spawnParams);
	
	if (IsValid(Spell))
	{
		Spell->OnSpellHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnSpellHit);
		Spell->InitSpell(MousePosAtActorHeight, GetOwner<APawn>());
	}

	OnSpellCastedDelegate.Broadcast(GetOwner(), Spell);
}

void UElementManipulationComponent::TryCastBasicAttack()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	FTimerHandle* CooldownTimer = CooldownTimers.Find(BasicAttack);

	if (!BasicAttack || !CooldownTimer || TimerManager.IsTimerActive(*CooldownTimer))
	{
		GEngine->AddOnScreenDebugMessage(191991, 2, FColor::Red, FString::SanitizeFloat(TimerManager.GetTimerRemaining(*CooldownTimer)));
		return;
	}

	TimerManager.SetTimer(*CooldownTimer, CalculateSpellCooldown(BasicAttack), false);

	FVector MousePosAtActorHeight;
	FHitResult RaycastHit;
	RaycastMouseOnLevel(MousePosAtActorHeight, RaycastHit);

	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = IsValid(BasicAttackOriginPoint) ? BasicAttackOriginPoint->GetComponentLocation() : GetOwner()->GetActorLocation();
	auto Spell = GetWorld()->SpawnActor<ABaseSpell>(BasicAttack, SpawnLocation, FRotator(), spawnParams);

	if (Spell != nullptr)
	{
		Spell->OnSpellHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnBasicAttackHit);
		Spell->InitSpell(MousePosAtActorHeight, GetOwner<APawn>());
	}

	OnBasicAttackCastedDelegate.Broadcast(GetOwner(), Spell);
}

ABaseSpell* UElementManipulationComponent::TriggeredCast(TSubclassOf<ABaseSpell> SpellClass, const FVector& TargetLocation, const FVector& SpawnLocation, bool SendSpellEvents)
{
	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseSpell* Spell = GetWorld()->SpawnActor<ABaseSpell>(SpellClass, SpawnLocation, FRotator(), spawnParams);

	if (Spell != nullptr)
	{
		Spell->InitSpell(TargetLocation, GetOwner<APawn>());
	}

	if (SendSpellEvents)
	{
		Spell->OnSpellHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnSpellHit);
		OnSpellCastedDelegate.Broadcast(GetOwner(), Spell);
	}

	return Spell;
}

const TSet<WizardElement>& UElementManipulationComponent::GetLearnedElements() const
{
	return LearnedElements;
}

const TArray<WizardElement>& UElementManipulationComponent::GetActiveElements() const
{
	return CurrentElements;
}

void UElementManipulationComponent::LearnElement(WizardElement Element)
{
	LearnedElements.FindOrAdd(Element);
	OnElementLearnedDelegate.Broadcast(Element);
}

const TArray<FSpellConfig>& UElementManipulationComponent::GetSpellConfig()
{
	return SpellConfiguration;
}

void UElementManipulationComponent::AddCooldownMultiplier(TSubclassOf<ABaseSpell> ApplicableSpell, float CooldownMultiplier)
{
	if (SpellCooldownMultipliers.Contains(ApplicableSpell))
	{
		SpellCooldownMultipliers[ApplicableSpell] *= CooldownMultiplier;
	}
	else
	{
		SpellCooldownMultipliers.Add(ApplicableSpell, 1 * CooldownMultiplier);
	}
}

void UElementManipulationComponent::AddCooldownMultiplierForElement(WizardElement Element, float CooldownMultiplier)
{
	ElementCooldownMultipliers[Element] *= CooldownMultiplier;
}

TMap<TSubclassOf<class ABaseSpell>, float>& UElementManipulationComponent::GetSpellCooldowns()
{
	return Cooldowns;
}

TMap<TSubclassOf<class ABaseSpell>, FTimerHandle>& UElementManipulationComponent::GetSpellCooldownTimers()
{
	return CooldownTimers;
}

USceneComponent* UElementManipulationComponent::GetBasicAttackOrigin() const
{
	return BasicAttackOriginPoint;
}

void UElementManipulationComponent::SetBasicAttackOrigin(USceneComponent* NewBasicAttackOrigin)
{
	BasicAttackOriginPoint = NewBasicAttackOrigin;
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
			Cooldowns.Add(SpellConfig.Spell, 5);
		}
	}

	for (TTuple<TSubclassOf<ABaseSpell>, float>& kvp : Cooldowns)
	{
		CooldownTimers.Add(kvp.Key);
		SpellCooldownMultipliers.Add(kvp.Key, 1);
	}

	for (size_t i = 0; i < CurrentElements.Num(); i++)
	{
		OnElementAddedDelegate.Broadcast(WizardElement::None, CurrentElements[i]);
	}
}

void UElementManipulationComponent::OnSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	OnSpellHitDelegate.Broadcast(Spell, HitActor);
}

void UElementManipulationComponent::OnBasicAttackHit(ABaseSpell* Spell, AActor* HitActor)
{
	OnBasicAttackHitDelegate.Broadcast(Spell, HitActor);
}

float UElementManipulationComponent::CalculateSpellCooldown(TSubclassOf<class ABaseSpell> Spell)
{
	//Calculate element cooldown multiplier for spell
	FSpellConfig* SpellConfig = SpellConfiguration.FindByPredicate([Spell](const FSpellConfig& SpellConfig) {return SpellConfig.Spell == Spell; });
	float ElementCooldownMultiplier = 1;
	if (SpellConfig)
	{
		//Spells consisting of only 1 element do not get its cooldown multiplier applied twice
		WizardElement LastAddedElement{ WizardElement::None };
		for (const WizardElement& ComponentElement : SpellConfig->ElementCombination)
		{
			if (LastAddedElement != ComponentElement)
				ElementCooldownMultiplier *= ElementCooldownMultipliers[ComponentElement];

			LastAddedElement = ComponentElement;
		}
	}

	return *Cooldowns.Find(Spell) * SpellCooldownMultipliers.FindOrAdd(Spell, 1) * ElementCooldownMultiplier * (Stats ? Stats->GetSpellCooldownMultiplier() : 1.f);
}
