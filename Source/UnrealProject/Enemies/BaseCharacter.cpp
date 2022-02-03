// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include <Components/CapsuleComponent.h>
#include "../Spells/BaseSpell.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <AIController.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TArray<UCapsuleComponent*> comps;
	GetComponents(comps);
	if (comps.Num() > 0)
	{
		comps[0]->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnHit);
		comps[0]->SetSimulatePhysics(true);
	}

	TArray<UCharacterMovementComponent*> characterMovementComps{};
	GetComponents(characterMovementComps);
	if (characterMovementComps.Num() > 0)
	{
		m_CharacterMovement = characterMovementComps[0];
	}

	m_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleSystem");
	m_NiagaraComponent->SetupAttachment(RootComponent);
}

bool ABaseCharacter::GetStunned()
{
	return m_Stunned;
}

float ABaseCharacter::GetHealth()
{
	return m_Health;
}

float ABaseCharacter::GetStartHealth()
{
	return m_StartHealth;
}

void ABaseCharacter::Heal(float hp)
{
	m_Health += hp;
	if (m_Health > m_StartHealth)
		m_Health = m_StartHealth;
	if (m_FloatingTextClass == nullptr)
		return;
	auto text = GetWorld()->SpawnActor(m_FloatingTextClass);
	Cast<AFloatingTextActor>(text)->Initialize(FText::FromString(FString::SanitizeFloat(hp)), FColor::Green);
	text->SetActorLocation(GetActorLocation());
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_StartHealth = m_Health;
}

void ABaseCharacter::CheckDeath()
{
	if (m_Health <= 0)
	{
		OnDeath();
		Destroy();
	}
}

void ABaseCharacter::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetInstigatorController<APlayerController>() != GetController<APlayerController>())
	{
		auto spell = Cast<ABaseSpell>(OtherActor);
		if (spell != nullptr && !spell->WasActorHit(this))
		{
			spell->AddHitActor(this);
			TakeSpellDamage(spell->GetDamage());
			AddStatusEffects(spell->GetStatusEffects());
			spell->OnHit(this);
		}
	}
}

void ABaseCharacter::TakeSpellDamage(float damage)
{
	m_Health -= damage;
	SpawnDamageText(damage);
	CheckDeath();
}

void ABaseCharacter::TakeTickDamage(float damage)
{
	m_Health -= damage;
	SpawnDamageText(damage);
	CheckDeath();
}

void ABaseCharacter::AddStatusEffects(const TArray<FStatusEffect>& statusEffects)
{
	for (const auto& effect : statusEffects)
	{
		if (effect.EffectType == Type::Stun)
		{
			m_Stunned = true;
			OnStunned();
			if (m_NiagaraComponent != nullptr)
				m_NiagaraComponent->SetVariableFloat(TEXT("Stunned"), 1.f);
		}
		if (effect.EffectType == Type::Damage)
		{
			if (m_NiagaraComponent != nullptr)
				m_NiagaraComponent->SetVariableFloat(TEXT("Burning"), 1.f);
		}
		if (effect.EffectType == Type::Slow)
		{
			m_SlowAmount += effect.Value;
			m_CharacterMovement->MaxWalkSpeed *= (1 - effect.Value/100.f);
		}
		m_CurrentStatusEffects.Add(effect);
	}
}

void ABaseCharacter::SpawnDamageText(float damage)
{
	if (m_FloatingTextClass == nullptr)
		return;
	auto text = GetWorld()->SpawnActor(m_FloatingTextClass);
	Cast<AFloatingTextActor>(text)->Initialize(FText::FromString(FString::SanitizeFloat(damage)), m_DamageTextColor);
	text->SetActorLocation(GetActorLocation());
}

void ABaseCharacter::Knockup()
{
	auto controller = GetController<AAIController>();
	
	if (controller != nullptr)
		controller->StopMovement();

	Push(FVector(0.f, 0.f, 500.f));
}

void ABaseCharacter::Push(const FVector& force)
{
	TArray<UCharacterMovementComponent*> comps;
	GetComponents(comps);
	if (comps.Num() > 0)
	{
		comps[0]->Velocity = FVector(0.f, 0.f, 0.f);
		comps[0]->AddImpulse(force, true);
	}
}

void ABaseCharacter::ReapplyStatusEffects(const TArray<FStatusEffect>& statusEffects)
{
	for (const auto& effect : statusEffects)
	{
		bool doContinue{ false };
		for (auto& currEffect : m_CurrentStatusEffects)
		{
			if (effect.EffectType == currEffect.EffectType && effect.Cause == currEffect.Cause)
			{
				doContinue = true;
				switch (effect.EffectType)
				{
				case Type::Damage:
					currEffect.Duration = effect.Duration;
					break;
				case Type::Slow:
				case Type::Stun:
					currEffect.Timer = 0;
					break;
				default:
					break;
				}
			}
		}

		if (doContinue)
			continue;

		if (effect.EffectType == Type::Stun)
		{
			m_Stunned = true;
			OnStunned();
			if (m_NiagaraComponent != nullptr)
				m_NiagaraComponent->SetVariableFloat(TEXT("Stunned"), 1.f);
		}
		if (effect.EffectType == Type::Damage)
		{
			if (m_NiagaraComponent != nullptr)
				m_NiagaraComponent->SetVariableFloat(TEXT("Burning"), 1.f);
		}
		if (effect.EffectType == Type::Slow)
		{
			m_SlowAmount += effect.Value;
			m_CharacterMovement->MaxWalkSpeed *= (1 - effect.Value / 100.f);
		}
		m_CurrentStatusEffects.Add(effect);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (size_t i = 0; i < m_CurrentStatusEffects.Num(); i++)
	{
		if (m_CurrentStatusEffects[i].EffectType == Type::Damage)
		{
			m_CurrentStatusEffects[i].Timer += DeltaTime;
			if (m_CurrentStatusEffects[i].Timer > m_CurrentStatusEffects[i].Interval)
			{
				m_CurrentStatusEffects[i].Timer -= m_CurrentStatusEffects[i].Interval;
				TakeTickDamage(m_CurrentStatusEffects[i].Value);
				m_CurrentStatusEffects[i].Duration -= m_CurrentStatusEffects[i].Interval;
				if (m_CurrentStatusEffects[i].Duration < m_CurrentStatusEffects[i].Interval)
				{
					m_CurrentStatusEffects.RemoveAt(i);
					i--;

					auto otherBurnEffect = m_CurrentStatusEffects.FindByPredicate([](const FStatusEffect& effect) {return effect.EffectType == Type::Damage; });
					if (otherBurnEffect == nullptr && m_NiagaraComponent != nullptr)
						m_NiagaraComponent->SetVariableFloat(TEXT("Burning"), 0.f);

					continue;
				}
			}
		}
		else
		{
			m_CurrentStatusEffects[i].Timer += DeltaTime;
			if (m_CurrentStatusEffects[i].Timer > m_CurrentStatusEffects[i].Duration)
			{
				FStatusEffect effect = m_CurrentStatusEffects[i];
				m_CurrentStatusEffects.RemoveAt(i);
				if (effect.EffectType == Type::Slow)
				{
					m_SlowAmount -= effect.Value;
					m_CharacterMovement->MaxWalkSpeed /= (1 - effect.Value / 100.f);
				}
				if (effect.EffectType == Type::Stun)
				{
					auto otherStunEffect = m_CurrentStatusEffects.FindByPredicate([](const FStatusEffect& effect) {return effect.EffectType == Type::Stun; });
					if (otherStunEffect == nullptr)
					{
						m_Stunned = false;
						if (m_NiagaraComponent != nullptr)
							m_NiagaraComponent->SetVariableFloat(TEXT("Stunned"), 0.f);
					}
				}
				i--;
				continue;
			}
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

