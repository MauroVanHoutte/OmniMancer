// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemConfig.generated.h"

UCLASS(BlueprintType)
class UNREALPROJECT_API UItemConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Instanced)
	class UCharacterUpgrade* Upgrade;
	UPROPERTY(EditDefaultsOnly)
	int Cost;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* Mesh;
};
