// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include <Components/CapsuleComponent.h>
#include "../Spells/BaseSpell.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <AIController.h>
#include "../Powerups/PowerUp.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../FloatingTextActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Coin.h"



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
		CharacterMovement = characterMovementComps[0];
	}

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleSystem");
	NiagaraComponent->SetupAttachment(RootComponent);
}

bool ABaseCharacter::GetStunned()
{
	return Stunned;
}

float ABaseCharacter::GetHealth()
{
	return Health;
}

float ABaseCharacter::GetStartHealth()
{
	return StartHealth;
}

void ABaseCharacter::Heal(float hp)
{
	Health += hp;
	if (Health > StartHealth)
		Health = StartHealth;
	if (FloatingTextClass == nullptr)
		return;
	auto text = GetWorld()->SpawnActor(FloatingTextClass);
	Cast<AFloatingTextActor>(text)->Initialize(FText::FromString(FString::SanitizeFloat(hp)), FColor::Green);
	text->SetActorLocation(GetActorLocation());
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	StartHealth = Health;
}

void ABaseCharacter::CheckDeath()
{
	if (Health <= 0)
	{
		OnDeath();
		Destroy();
	}
}

void ABaseCharacter::OnDeath()
{
	OnDeathEvent();

	//Spawn Powerup
	if (SpawnPowerupOnDeath)
	{
		SpawnPowerup();
	}

	//Spawn Coins
	SpawnCoins();
}

void ABaseCharacter::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetInstigatorController<APlayerController>() != GetController<APlayerController>())
	{
		auto spell = Cast<ABaseSpell>(OtherActor);
		if (spell != nullptr && !spell->WasActorHit(this))
		{
			spell->AddHitActor(this);
			TakeSpellDamage(spell);
			AddStatusEffects(spell->GetStatusEffects());
			spell->OnHit(this); //activates caster on hit trigger
		}
	}
}

void ABaseCharacter::OnTakeHit(AActor* cause)
{
}

void ABaseCharacter::TakeSpellDamage(ABaseSpell* spell)
{
	Health -= spell->GetDamage();
	OnTakeHit(spell->GetInstigator());
	OnTakeHitBP();
	SpawnDamageText(spell->GetDamage());
	CheckDeath();
	PerceiveDamage(spell->GetInstigator(), spell->GetActorLocation());
}

void ABaseCharacter::TakeSpellDamageFloat(float damage, AActor* cause)
{
	Health -= damage;
	SpawnDamageText(damage);
	OnTakeHitBP();
	CheckDeath();
	PerceiveDamage(cause, cause->GetActorLocation());
}

void ABaseCharacter::TakeTickDamage(float damage)
{
	Health -= damage;
	OnTakeHitBP();
	SpawnDamageText(damage);
	CheckDeath();
}

void ABaseCharacter::AddStatusEffects(const TArray<FStatusEffect>& statusEffects)
{
	for (const auto& effect : statusEffects)
	{
		AddStatusEffect(effect);
	}
}

void ABaseCharacter::SpawnDamageText(float damage)
{
	if (FloatingTextClass == nullptr || FMath::IsNearlyEqual(damage, 0.f))
		return;
	auto text = GetWorld()->SpawnActor(FloatingTextClass);
	Cast<AFloatingTextActor>(text)->Initialize(FText::FromString(FString::SanitizeFloat(FMath::RoundHalfToZero(damage * 100)/100.f)), DamageTextColor);
	text->SetActorLocation(GetActorLocation());
}

void ABaseCharacter::SpawnPowerup()
{
	if (FMath::RandRange(0.f, 100.f) > PowerupSpawnChance)
		return;
	
	auto location = GetActorLocation();
	auto actor = GetWorld()->SpawnActor(PowerupBP.Get(), &location);
	if (actor != nullptr)
	{
		float angle = FMath::FRandRange(0.f, 360.f);
		FVector direction{ FMath::Sin(FMath::DegreesToRadians(angle)), FMath::Cos(FMath::DegreesToRadians(angle)), 1 };
		auto powerup = Cast<APowerUp>(actor);
		powerup->SetRandomEffect();
		powerup->LaunchInDirection(direction.GetSafeNormal(), 300);
	}
}

void ABaseCharacter::SpawnCoins()
{
	int nrCoins = FMath::RandRange(MinCoinsDropped, MaxCoinsDropped);
	for (size_t i = 0; i < nrCoins; i++)
	{
		auto location = GetActorLocation();
		auto actor = GetWorld()->SpawnActor(CoinBP.Get(), &location);
		float angle = FMath::FRandRange(0.f, 360.f);
		FVector direction{ FMath::Sin(FMath::DegreesToRadians(angle)), FMath::Cos(FMath::DegreesToRadians(angle)), 1 };
		auto coin = Cast<ACoin>(actor);
		coin->LaunchInDirection(direction.GetSafeNormal(), 300);
	}
}

void ABaseCharacter::UpdateStatusEffects(float deltaTime) //todo move logic into statuseffect
{
	for (int32 i = 0; i < CurrentStatusEffects.Num(); i++)
	{
		CurrentStatusEffects[i].Timer += deltaTime;
		if (CurrentStatusEffects[i].EffectType == Type::Damage)
		{
			if (CurrentStatusEffects[i].Timer > CurrentStatusEffects[i].Interval)
			{
				CurrentStatusEffects[i].Timer -= CurrentStatusEffects[i].Interval;
				TakeTickDamage(CurrentStatusEffects[i].Value);
				CurrentStatusEffects[i].Duration -= CurrentStatusEffects[i].Interval;
				if (CurrentStatusEffects[i].Duration < CurrentStatusEffects[i].Interval)
				{
					CurrentStatusEffects.RemoveAt(i);
					i--;

					auto otherBurnEffect = CurrentStatusEffects.FindByPredicate([](const FStatusEffect& effect) {return effect.EffectType == Type::Damage; });
					if (otherBurnEffect == nullptr && NiagaraComponent != nullptr)
						NiagaraComponent->SetVariableFloat(TEXT("Burning"), 0.f);

					continue;
				}
			}
		}
		else
		{
			if (CurrentStatusEffects[i].Timer > CurrentStatusEffects[i].Duration)
			{
				if (CurrentStatusEffects[i].EffectType == Type::Slow)
				{
					SlowAmount -= CurrentStatusEffects[i].Value;
					CharacterMovement->MaxWalkSpeed /= (1 - CurrentStatusEffects[i].Value);
				}
				if (CurrentStatusEffects[i].EffectType == Type::Stun)
				{
					auto otherStunEffect = CurrentStatusEffects.FilterByPredicate([](const FStatusEffect& effect) {return effect.EffectType == Type::Stun; });
					if (otherStunEffect.Num() < 2)
					{
						Stunned = false;
						OnStunnedEnd();
						if (NiagaraComponent != nullptr)
							NiagaraComponent->SetVariableFloat(TEXT("Stunned"), 0.f);
					}
				}
				if (CurrentStatusEffects[i].EffectType == Type::Curse)
				{
					TakeTickDamage(CurrentStatusEffects[i].Value);
					if (IsPendingKillPending())
					{
						SpreadCurse(CurrentStatusEffects[i]);
					}

					if (NiagaraComponent != nullptr)
						NiagaraComponent->SetVariableFloat(TEXT("Cursed"), 0.f);
				}
				CurrentStatusEffects.RemoveAt(i);
				i--;
				continue;
			}
		}
	}
}

void ABaseCharacter::SpreadCurse(const FStatusEffect& curseEffect)
{
	TArray<AActor*>ignoreActors{this};
	TArray<AActor*>outActors{};
	//get nearby enemies
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), curseEffect.Interval, TArray<TEnumAsByte<EObjectTypeQuery>>(), StaticClass(), ignoreActors, outActors); //range is stored in interval

	for (AActor* actor : outActors)
	{
		if (!actor->IsA<AWizardCharacter>())
		{
			FStatusEffect curse = curseEffect;
			curse.Timer = 0;
			TArray<FStatusEffect> effects{curse};
			Cast<ABaseCharacter>(actor)->AddStatusEffects(effects);
			return;
		}
	}
}

void ABaseCharacter::Knockup()
{
	Push(FVector(0.f, 0.f, 500.f));
}

void ABaseCharacter::Push(const FVector& force)
{
	AAIController* controller = GetController<AAIController>();
	if (controller != nullptr)
		controller->StopMovement();
	TArray<UCharacterMovementComponent*> comps;
	GetComponents(comps);
	if (comps.Num() > 0)
	{
		comps[0]->Velocity = FVector(0.f, 0.f, 0.f);
		comps[0]->AddImpulse(force*KnockbackMultiplier, true);
	}
}

void ABaseCharacter::ReapplyStatusEffects(const TArray<FStatusEffect>& statusEffects)
{
	for (const auto& effect : statusEffects)
	{
		bool doContinue{ false };
		for (auto& currEffect : CurrentStatusEffects)
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
			Stunned = true;
			OnStunned();
			if (NiagaraComponent != nullptr)
				NiagaraComponent->SetVariableFloat(TEXT("Stunned"), 1.f);
		}
		if (effect.EffectType == Type::Damage)
		{
			if (NiagaraComponent != nullptr)
				NiagaraComponent->SetVariableFloat(TEXT("Burning"), 1.f);
		}
		if (effect.EffectType == Type::Slow)
		{
			SlowAmount += effect.Value;
			CharacterMovement->MaxWalkSpeed *= (1 - effect.Value);
		}
		CurrentStatusEffects.Add(effect);
	}
}

void ABaseCharacter::AddStatusEffect(const FStatusEffect& effect)
{
	if (effect.EffectType == Type::Stun)
	{
		Stunned = true;
		OnStunned();
		if (NiagaraComponent != nullptr)
			NiagaraComponent->SetVariableFloat(TEXT("Stunned"), 1.f);
	}
	if (effect.EffectType == Type::Damage)
	{
		if (NiagaraComponent != nullptr)
			NiagaraComponent->SetVariableFloat(TEXT("Burning"), 1.f);
	}
	if (effect.EffectType == Type::Slow)
	{
		SlowAmount += effect.Value;
		CharacterMovement->MaxWalkSpeed *= (1 - effect.Value);
	}
	if (effect.EffectType == Type::Curse)
	{
		if (NiagaraComponent != nullptr)
			NiagaraComponent->SetVariableFloat(TEXT("Cursed"), 1.f);
	}
	CurrentStatusEffects.Add(effect);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStatusEffects(DeltaTime);
	
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

