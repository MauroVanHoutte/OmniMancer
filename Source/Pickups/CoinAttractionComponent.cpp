// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinAttractionComponent.h"
#include "Coin.h"

void UCoinAttractionComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UCoinAttractionComponent::OnBeginOverlap);
}

void UCoinAttractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACoin* Coin = Cast<ACoin>(OtherActor);
	if (IsValid(Coin))
	{
		Coin->HomeInToTarget(GetOwner());
	}
}
