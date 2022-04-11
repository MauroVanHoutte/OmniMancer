// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../StatusEffect.h"
#include "BaseSpell.generated.h"

UCLASS(Abstract)
class UNREALPROJECT_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSpell();

	void Destroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float m_Damage = 0;

	UPROPERTY(EditAnywhere)
	TArray<FStatusEffect> m_StatusEffects{};

	UPROPERTY(EditDefaultsOnly)
	float m_LifeSpan = 5;

	TArray<AActor*> m_HitActors{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float GetDamage() const;
	virtual const TArray<FStatusEffect>& GetStatusEffects() const;
	virtual void SetDamageMultiplier(int damageMultiplier);

	void AddHitActor( AActor* actor);
	bool WasActorHit( AActor* actor) const;

	virtual void OnHit(AActor* hitActor);

private:
	UPROPERTY(VisibleAnywhere)
	int m_DamageMultiplier = 0;

	FTimerHandle m_LifeTimer{};
};
