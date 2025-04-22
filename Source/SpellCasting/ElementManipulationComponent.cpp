// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementManipulationComponent.h"
#include "ActorPool/ActorPoolingSubsystem.h"
#include "NiagaraComponent.h"
#include "SpellCasting/PlayerCasts/BasePlayerCast.h"
#include "SpellCasting/Spells/BaseProjectile.h"
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

	PoolingSystem = GetWorld()->GetGameInstance()->GetSubsystem<UActorPoolingSubsystem>();
}

void UElementManipulationComponent::AddElement(WizardElement Element)
{
	if (LearnedElements.Contains(Element) && !bCastingSpell)
	{
		CurrentElements.Insert(Element, 0);
		WizardElement OldElement = CurrentElements.Pop();

		if (bAddingElementLeft)
			LeftHandElementParticle->SetVariableLinearColor(ColorVariableName, *ElementColors.Find(Element));
		else
			RightHandElementParticle->SetVariableLinearColor(ColorVariableName, *ElementColors.Find(Element));

		bAddingElementLeft = !bAddingElementLeft;

		CurrentSpellId = UElementLibrary::GetSpellIDFromElements(CurrentElements);
		OnElementAddedDelegate.Broadcast(this, OldElement, Element);
	}
}

void UElementManipulationComponent::TryCastSpell()
{
	const int SpellID = UElementLibrary::GetSpellIDFromElements(CurrentElements);
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TSubclassOf<ABaseSpell>* SpellClass = Spells.Find(SpellID);

	if (!SpellClass || TimerManager.IsTimerActive(*CooldownTimers.Find(*SpellClass)))
		return;
	
	//TimerManager.SetTimer(*CooldownTimers.Find(*SpellClass), CalculateSpellCooldown(*SpellClass), false);

	FVector MousePosAtActorHeight;
	FHitResult RaycastHit;
	RaycastMouseOnLevel(MousePosAtActorHeight, RaycastHit);

	FVector SpawnLocation = IsValid(SpellOriginPoint) ? SpellOriginPoint->GetComponentLocation() : GetOwner()->GetActorLocation();
	ABaseSpell* Spell = Cast<ABaseSpell>(PoolingSystem->GetActorFromPool(*SpellClass));
	Spell->SetOwner(GetOwner());
	Spell->SetInstigator(GetOwner<APawn>());
	Spell->SetActorLocationAndRotation(SpawnLocation, FRotator(0, 0, 0));
	Spell->OnSpellHitDelegate.Clear();
	//Spell->OnSpellHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnSpellHit);
	Spell->InitSpell(MousePosAtActorHeight, GetOwner<APawn>());

	OnSpellCastedDelegate.Broadcast(this, nullptr, Spell);
}

void UElementManipulationComponent::HandleCastInputPressed()
{
	if (SpellCastObjects.Contains(CurrentSpellId)
		&& IsValid(SpellCastObjects[CurrentSpellId])
		&& !bCastingBasicAttack)
	{
		SpellCastObjects[CurrentSpellId]->HandleCastInputPressed();
	}
}

void UElementManipulationComponent::HandleCastInputDown()
{
	if (SpellCastObjects.Contains(CurrentSpellId)
		&& IsValid(SpellCastObjects[CurrentSpellId])
		&& !bCastingBasicAttack)
	{
		SpellCastObjects[CurrentSpellId]->HandleCastInputDown();
	}
}

void UElementManipulationComponent::HandleCastInputReleased()
{
	if (SpellCastObjects.Contains(CurrentSpellId)
		&& IsValid(SpellCastObjects[CurrentSpellId])
		&& !bCastingBasicAttack)
	{
		SpellCastObjects[CurrentSpellId]->HandleCastInputReleased();
	}
}

void UElementManipulationComponent::TryCastBasicAttack()
{
	if (IsValid(BasicAttackCast))
	{
		BasicAttackCast->HandleCastInputPressed();
	}
	/*FTimerManager& TimerManager = GetWorld()->GetTimerManager();
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

	FVector SpawnLocation = IsValid(BasicAttackOriginPoint) ? BasicAttackOriginPoint->GetComponentLocation() : GetOwner()->GetActorLocation();
	ABaseSpell* Spell = Cast<ABaseSpell>(PoolingSystem->GetActorFromPool(BasicAttack));
	Spell->SetOwner(GetOwner());
	Spell->SetInstigator(GetOwner<APawn>());
	Spell->SetActorLocationAndRotation(SpawnLocation, FRotator(0, 0, 0));
	Spell->OnSpellHitDelegate.Clear();
	Spell->OnSpellHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnBasicAttackHit);
	Spell->InitSpell(MousePosAtActorHeight, GetOwner<APawn>());

	OnBasicAttackCastedDelegate.Broadcast(GetOwner(), Spell);*/
}

void UElementManipulationComponent::HandleBasicAttackInputPressed()
{
	if (IsValid(BasicAttackCast) && !bCastingSpell)
	{
		BasicAttackCast->HandleCastInputPressed();
	}
}

void UElementManipulationComponent::HandleBasicAttackInputDown()
{
	if (IsValid(BasicAttackCast) && !bCastingSpell)
	{
		BasicAttackCast->HandleCastInputDown();
	}
}

void UElementManipulationComponent::HandleBasicAttackInputReleased()
{
	if (IsValid(BasicAttackCast) && !bCastingSpell)
	{
		BasicAttackCast->HandleCastInputReleased();
	}
}

ABaseSpell* UElementManipulationComponent::TriggeredCast(TSubclassOf<ABaseSpell> SpellClass, const FVector& TargetLocation, const FVector& SpawnLocation, bool SendSpellEvents)
{
	ABaseSpell* Spell = Cast<ABaseSpell>(PoolingSystem->GetActorFromPool(SpellClass));
	Spell->OnSpellHitDelegate.Clear();
	Spell->SetOwner(GetOwner());
	Spell->SetInstigator(GetOwner<APawn>());
	Spell->SetActorLocationAndRotation(SpawnLocation, FRotator(0, 0, 0));
	Spell->InitSpell(TargetLocation, GetOwner<APawn>());
	ABaseProjectile* Projectile = Cast<ABaseProjectile>(Spell);
	if (IsValid(Stats) && IsValid(Projectile))
		Projectile->SetBounces(Projectile->GetBounces() + Stats->GetExtraBounces());

	if (SendSpellEvents)
	{
		Spell->OnSpellHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnTriggeredSpellHit);
		OnSpellCastedDelegate.Broadcast(this, nullptr, Spell);
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

void UElementManipulationComponent::UnlearnElement(WizardElement Element)
{
	LearnedElements.Remove(Element);
}

const TArray<FSpellConfig>& UElementManipulationComponent::GetSpellConfigs()
{
	return SpellConfiguration;
}

TArray<FSpellCastConfig>& UElementManipulationComponent::GetSpellCastConfigs()
{
	return SpellCastConfiguration;
}

void UElementManipulationComponent::AddCooldownMultiplier(TSubclassOf<ABaseSpell> ApplicableSpell, float CooldownMultiplier)
{
	SpellCooldownMultipliers.FindOrAdd(ApplicableSpell, 1) *= CooldownMultiplier;
}

void UElementManipulationComponent::AddCooldownMultiplierForElement(WizardElement Element, float CooldownMultiplier)
{
	ElementCooldownMultipliers.FindOrAdd(Element, 1) *= CooldownMultiplier;
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
	BasicAttackCast->SetCastOrigin(NewBasicAttackOrigin);
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
	for (const FSpellConfig& SpellConfig : SpellConfiguration)
	{
		Spells.Add(UElementLibrary::GetSpellIDFromElements(SpellConfig.ElementCombination), SpellConfig.Spell);

		if (!Cooldowns.Find(SpellConfig.Spell))
		{
			Cooldowns.Add(SpellConfig.Spell, 5);
		}
	}

	for (const FSpellCastConfig& SpellCastConfig : SpellCastConfiguration)
	{
		SpellCastObjects.Add(UElementLibrary::GetSpellIDFromElements(SpellCastConfig.ElementCombination), SpellCastConfig.CastObject);
		SpellCastConfig.CastObject->Initialize(this, Stats, GetOwner(), Controller, SpellOriginPoint);
		SpellCastConfig.CastObject->OnCastStartedDelegate.AddDynamic(this, &UElementManipulationComponent::OnSpellCastStarted);
		SpellCastConfig.CastObject->OnCastDelegate.AddDynamic(this, &UElementManipulationComponent::OnSpellCasted);
		SpellCastConfig.CastObject->OnCastHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnSpellHit);
	}

	for (TTuple<TSubclassOf<ABaseSpell>, float>& kvp : Cooldowns)
	{
		CooldownTimers.Add(kvp.Key);
		SpellCooldownMultipliers.Add(kvp.Key, 1);
	}

	for (size_t i = 0; i < CurrentElements.Num(); i++)
	{
		OnElementAddedDelegate.Broadcast(this, WizardElement::None, CurrentElements[i]);
	}

	BasicAttackCast->Initialize(this, Stats, GetOwner(), Controller, BasicAttackOriginPoint);
	BasicAttackCast->OnCastStartedDelegate.AddDynamic(this, &UElementManipulationComponent::OnBasicAttackCastStarted);
	BasicAttackCast->OnCastDelegate.AddDynamic(this, &UElementManipulationComponent::OnBasicAttackCasted);
	BasicAttackCast->OnCastHitDelegate.AddDynamic(this, &UElementManipulationComponent::OnBasicAttackHit);

	CurrentSpellId = UElementLibrary::GetSpellIDFromElements(CurrentElements);
}

void UElementManipulationComponent::OnSpellCastStarted(UBasePlayerCast* CastObject)
{
	bCastingSpell = true;
}

void UElementManipulationComponent::OnSpellCasted(UBasePlayerCast* CastObject, ABaseSpell* Spell)
{
	bCastingSpell = false;
	OnSpellCastedDelegate.Broadcast(this, CastObject, Spell);
}

void UElementManipulationComponent::OnSpellHit(UBasePlayerCast* CastObject, ABaseSpell* Spell, AActor* HitActor)
{
	OnSpellHitDelegate.Broadcast(this, CastObject, Spell, HitActor);
}

void UElementManipulationComponent::OnTriggeredSpellHit(ABaseSpell* Spell, AActor* HitActor)
{
	OnSpellHitDelegate.Broadcast(this, nullptr, Spell, HitActor);
}

void UElementManipulationComponent::OnBasicAttackCastStarted(UBasePlayerCast* CastObject)
{
	bCastingBasicAttack = true;
}

void UElementManipulationComponent::OnBasicAttackCasted(UBasePlayerCast* CastObject, ABaseSpell* Spell)
{
	bCastingBasicAttack = false;
	OnBasicAttackCastedDelegate.Broadcast(this, CastObject, Spell);
}

void UElementManipulationComponent::OnBasicAttackHit(UBasePlayerCast* CastObject, ABaseSpell* Spell, AActor* HitActor)
{
	OnBasicAttackHitDelegate.Broadcast(this, CastObject, Spell, HitActor);
	OnSpellHitDelegate.Broadcast(this, CastObject, Spell, HitActor);
}

float UElementManipulationComponent::GetSpellCastCooldownMultiplier(UBasePlayerCast* SpellCast)
{
	if (SpellCast == BasicAttackCast)
		return Stats->GetBaseAttackCooldownMultiplier();

	//Calculate element cooldown multiplier for spell
	FSpellCastConfig* SpellConfig = SpellCastConfiguration.FindByPredicate([SpellCast](const FSpellCastConfig& SpellConfig) {return SpellConfig.CastObject == SpellCast; });
	float ElementCooldownMultiplier = 1;
	if (SpellConfig)
	{
		//Spells consisting of only 1 element do not get its cooldown multiplier applied twice
		WizardElement LastAddedElement{ WizardElement::None };
		for (const WizardElement& ComponentElement : SpellConfig->ElementCombination)
		{
			if (LastAddedElement != ComponentElement)
				ElementCooldownMultiplier *= Stats->GetCooldownMultiplierForElement(ComponentElement);

			LastAddedElement = ComponentElement;
		}
	}

	return ElementCooldownMultiplier * Stats->GetSpellCooldownMultiplier();
}

float UElementManipulationComponent::GetSpellCastDamageMultiplier(UBasePlayerCast* SpellCast)
{
	if (SpellCast == BasicAttackCast)
		return Stats->GetBaseAttackDamageMultiplier();

	//Calculate element damage multiplier for spell
	FSpellCastConfig* SpellConfig = SpellCastConfiguration.FindByPredicate([SpellCast](const FSpellCastConfig& SpellConfig) {return SpellConfig.CastObject == SpellCast; });
	float ElementDamageMultiplier = 1;
	if (SpellConfig)
	{
		//Spells consisting of only 1 element do not get its cooldown multiplier applied twice
		WizardElement LastAddedElement{ WizardElement::None };
		for (const WizardElement& ComponentElement : SpellConfig->ElementCombination)
		{
			if (LastAddedElement != ComponentElement)
				ElementDamageMultiplier *= Stats->GetDamageMultiplierForElement(ComponentElement);

			LastAddedElement = ComponentElement;
		}
	}

	return 1 + (ElementDamageMultiplier - 1) + (Stats->GetSpellDamageMultiplier() - 1);
}
