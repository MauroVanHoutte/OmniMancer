// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

void UPickupComponent::Initialize()
{
	OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnBeginOverlap);
}

void UPickupComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}