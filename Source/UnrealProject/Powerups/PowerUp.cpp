// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "../WizardCharacter.h"
#include <Kismet2/KismetEditorUtilities.h>

TArray<TSubclassOf<UPowerUpEffect>> APowerUp::AllEffects{};
bool APowerUp::EffectsInitialized{ false };

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(FName("Collider"));

	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));

	Mesh->SetupAttachment(Collider);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnPickup);
}

void APowerUp::OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<AWizardCharacter>(OtherActor);

	if (player != nullptr)
	{
		player->AddPowerUpEffect(NewObject<UPowerUpEffect>(this, Effect.Get()));
		Destroy();
	}
}

void APowerUp::SetRandomEffect()
{
	if (!EffectsInitialized)
	{
		for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
		{
			UClass* Class = *ClassIt;


			if (!Class->IsChildOf(UPowerUpEffect::StaticClass()) ||
				Class == UPowerUpEffect::StaticClass() ||
				!Class->IsNative())
			{
				continue;
			}

			AllEffects.Add(Class);
		}

		EffectsInitialized = true;
	}

	int idx = FMath::RandRange(0, AllEffects.Num()-1);

	if (AllEffects.Num() == 0)
		return;

	Effect = AllEffects[idx];
}

void APowerUp::ReInitializeEffects()
{
	EffectsInitialized = false;
	SetRandomEffect();
}
