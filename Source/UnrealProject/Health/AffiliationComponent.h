// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AffiliationComponent.generated.h"

UENUM(BlueprintType)
enum class EAffiliation : uint8
{
	None,
	Player,
	Enemy
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UAffiliationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAffiliationComponent();

	UFUNCTION(BlueprintCallable)
	void SetAffiliation(EAffiliation NewAffiliation);
	UFUNCTION(BlueprintPure)
	EAffiliation GetAffiliation();

private:
	UPROPERTY(EditAnywhere)
	EAffiliation Affiliation;
};
