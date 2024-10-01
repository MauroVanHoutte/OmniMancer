// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FExperienceAddedSignature, UExperienceComponent*, ExperienceComponent, int, AddedExperience, int, NewTotalExperience, int, ExperienceToNextLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLevelledUpSignature, UExperienceComponent*, ExperienceComponent, int, NewLevel);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExperienceComponent();

	UFUNCTION(BlueprintCallable)
	void AddExperience(int AddedExperience);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetExperience();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetExperienceToNextLevel();
	UFUNCTION(BlueprintCallable)
	void ConvertStoredExperience();
	UFUNCTION(BlueprintCallable)
	void SetStoringExperience(bool newStoringExperience);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetLevel();

	UFUNCTION(BlueprintNativeEvent)
	float CalculateExperienceForNextLevel();

	UPROPERTY(BlueprintAssignable)
	FExperienceAddedSignature OnExperienceAddedDelegate;
	UPROPERTY(BlueprintAssignable)
	FLevelledUpSignature OnLevelledUpDelegate;

private:
	UPROPERTY(EditDefaultsOnly)
	int ExperienceToNextLevel = 10;
	UPROPERTY(EditDefaultsOnly)
	int Level = 1;

	bool bStoringExperience = true;

	int Experience = 0;
	int StoredExperience = 0;
};
