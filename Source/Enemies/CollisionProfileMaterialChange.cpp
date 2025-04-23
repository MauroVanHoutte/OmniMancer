// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/CollisionProfileMaterialChange.h"

void UCollisionProfileMaterialChange::Initialize(TMap<class UPrimitiveComponent*, class UMeshComponent*> Components)
{
	AffectedComponents = Components;
	for (auto kvp : AffectedComponents)
	{
		kvp.Key->OnComponentCollisionSettingsChangedEvent.AddDynamic(this, &UCollisionProfileMaterialChange::OnCollisionSettingsChanged);
	}
}

void UCollisionProfileMaterialChange::OnCollisionSettingsChanged(UPrimitiveComponent* ChangedComponent)
{
	UMeshComponent** Mesh = AffectedComponents.Find(ChangedComponent);
	UMaterialInterface** Material = Materials.Find(ChangedComponent->GetCollisionProfileName());
	if (Mesh && IsValid(*Mesh) && Material && IsValid(*Material))
	{
		(*Mesh)->SetMaterial(0, *Material);
	}
}

