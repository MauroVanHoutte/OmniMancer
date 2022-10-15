// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WizardCharacter.h"
#include "OmnimancerGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSkillData 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	FString Description;
	UPROPERTY(BlueprintReadWrite)
	int Cost;
};


class UOmnimancerSaveGame;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UOmnimancerGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void Init() override;
	void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	int GetCurrency();
	UFUNCTION(BlueprintCallable)
	void SetCurrency(int newCurrency);
	UFUNCTION(BlueprintCallable)
	WizardElement GetSelectedType();
	UFUNCTION(BlueprintCallable)
	void SetSelectedType(WizardElement type);

	UFUNCTION(BlueprintCallable)
	void AddUnlockedUpgrade(const FString& tag);
	UFUNCTION(BlueprintCallable)
	void RemoveUnlockedUpgrade(const FString& tag);
	UFUNCTION(BlueprintCallable)
	TArray<FString>& GetUnlockedUpgrades();

private:
	int Currency = 500;
	TArray<FString> UnlockedUpgrades{};
	WizardElement Selected = WizardElement::Fire;

	//TArray<FSkillData> FireSkills;
	//TArray<FSkillData> FrostSkills;
	//TArray<FSkillData> WindSkills;
};
