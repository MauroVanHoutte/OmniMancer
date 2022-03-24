// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "../WizardCharacter.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnPickup);
}

void APowerUp::OnPickup(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<AWizardCharacter>(OtherActor);

	if (player != nullptr)
	{
		player->AddPowerUpEffect(Effect); 
		Destroy();
	}
}
