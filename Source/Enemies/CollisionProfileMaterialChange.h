// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionProfileMaterialChange.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UCollisionProfileMaterialChange : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
	void Initialize(TMap<class UPrimitiveComponent*, class UMeshComponent*> Components);

private:
	UFUNCTION()
	void OnCollisionSettingsChanged(UPrimitiveComponent* ChangedComponent);

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, class UMaterialInterface*> Materials;
	UPROPERTY(Transient)
	TMap<class UPrimitiveComponent*, class UMeshComponent*> AffectedComponents;
};
