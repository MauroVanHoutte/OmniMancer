#include "StatBoosts.h"
#include "StatComponent.h"

void UBaseStatBoost::Apply(AActor* character)
{
	if (!IsValid(StatComponent))
		StatComponent = character->GetComponentByClass<class UStatComponent>();
}

void USpellCooldownBoost::Apply(AActor* character)
{
	Super::Apply(character);
	if (IsValid(StatComponent))
		StatComponent->SetSpellCooldownMultiplier(StatComponent->GetSpellCooldownMultiplier() * CooldownMultiplier);
}

void USpellCooldownBoost::Remove(AActor* character)
{
	if (IsValid(StatComponent))
		StatComponent->SetSpellCooldownMultiplier(StatComponent->GetSpellCooldownMultiplier() / CooldownMultiplier);
}

FFormatNamedArguments USpellCooldownBoost::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("SpellCooldownMultiplier"), CooldownMultiplier * 100);
	return Args;
}

void UElementSpellCooldownBoost::Apply(AActor* character)
{
	Super::Apply(character);
	if (IsValid(StatComponent))
		StatComponent->SetCooldownMultiplierForElement(Element, StatComponent->GetCooldownMultiplierForElement(Element) * CooldownMultiplier);
}

void UElementSpellCooldownBoost::Remove(AActor* character)
{
	if (IsValid(StatComponent))
		StatComponent->SetCooldownMultiplierForElement(Element, StatComponent->GetCooldownMultiplierForElement(Element) / CooldownMultiplier);
}

FFormatNamedArguments UElementSpellCooldownBoost::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("ElementCooldownMultiplier"), CooldownMultiplier * 100);
	const UEnum* WizardElementEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("WizardElement"));
	Args.Add(TEXT("SpellCooldownElement"), WizardElementEnum->GetDisplayNameTextByValue((int64)Element));	
	return Args;
}

void USpellDamageBoost::Apply(AActor* character)
{
	Super::Apply(character);
	if (IsValid(StatComponent))
		StatComponent->SetSpellDamageMultiplier(StatComponent->GetSpellDamageMultiplier() * DamageMultiplier);
}

void USpellDamageBoost::Remove(AActor* character)
{
	if (IsValid(StatComponent))
		StatComponent->SetSpellDamageMultiplier(StatComponent->GetSpellDamageMultiplier() / DamageMultiplier);
}

FFormatNamedArguments USpellDamageBoost::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("SpellDamageMultiplier"), DamageMultiplier * 100);
	return Args;
}

void UElementSpellDamageBoost::Apply(AActor* character)
{
	Super::Apply(character);
	if (IsValid(StatComponent))
		StatComponent->SetDamageMultiplierForElement(Element, StatComponent->GetDamageMultiplierForElement(Element) * DamageMultiplier);
}

void UElementSpellDamageBoost::Remove(AActor* character)
{
	if (IsValid(StatComponent))
		StatComponent->SetDamageMultiplierForElement(Element, StatComponent->GetDamageMultiplierForElement(Element) / DamageMultiplier);
}

FFormatNamedArguments UElementSpellDamageBoost::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("ElementDamageMultiplier"), DamageMultiplier * 100);
	const UEnum* WizardElementEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("WizardElement"));
	Args.Add(TEXT("SpellDamageElement"), WizardElementEnum->GetDisplayNameTextByValue((int64)Element));
	return Args;
}

void UBasicAttackDamageBoost::Apply(AActor* character)
{
	Super::Apply(character);
	if (IsValid(StatComponent))
		StatComponent->SetBaseAttackDamageMultiplier(StatComponent->GetBaseAttackDamageMultiplier() * DamageMultiplier);
}

void UBasicAttackDamageBoost::Remove(AActor* character)
{
	if (IsValid(StatComponent))
		StatComponent->SetBaseAttackDamageMultiplier(StatComponent->GetBaseAttackDamageMultiplier() / DamageMultiplier);
}

FFormatNamedArguments UBasicAttackDamageBoost::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("BasicDamageMultiplier"), DamageMultiplier * 100);
	return Args;
}

void UBasicAttackCooldownBoost::Apply(AActor* character)
{
	Super::Apply(character);
	if (IsValid(StatComponent))
		StatComponent->SetBaseAttackCooldownMultiplier(StatComponent->GetBaseAttackCooldownMultiplier() * CooldownMultiplier);
}

void UBasicAttackCooldownBoost::Remove(AActor* character)
{
	if (IsValid(StatComponent))
		StatComponent->SetBaseAttackCooldownMultiplier(StatComponent->GetBaseAttackCooldownMultiplier() / CooldownMultiplier);
}

FFormatNamedArguments UBasicAttackCooldownBoost::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("BasicCooldownMultiplier"), CooldownMultiplier * 100);
	return Args;
}

void UExtraBouncesBoost::Apply(AActor* character)
{
	Super::Apply(character);
	if (IsValid(StatComponent))
		StatComponent->SetExtraBounces(StatComponent->GetExtraBounces() + ExtraBounces);
}

void UExtraBouncesBoost::Remove(AActor* character)
{
	if (IsValid(StatComponent))
		StatComponent->SetExtraBounces(StatComponent->GetExtraBounces() - ExtraBounces);
}

FFormatNamedArguments UExtraBouncesBoost::GetDescriptionArguments()
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("ExtraBounces"), ExtraBounces);
	return Args;
}
