// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Rooms/RoomTypes.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrencyChanged, int, Currency);

UCLASS()
class UNREALPROJECT_API UOmnimancerGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void Init() override;
	void Shutdown() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCompletedRoomCount();
	UFUNCTION(BlueprintCallable)
	void SetCompletedRoomCount(int NewRoomCount);
	UFUNCTION(BlueprintCallable)
	int IncrementCompletedRoomCount();
	UFUNCTION(BlueprintCallable)
	void SetPortalTypes(TArray<class ARoomPortal*> InOutPortals);

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
	TSet<FString>& GetUnlockedUpgrades();

	UPROPERTY(BlueprintAssignable)
	FCurrencyChanged OnCurrencyChanged;

private:
	int CompletedRooms = 0;
	int Currency = 500;
	TSet<FString> UnlockedUpgrades{};
	WizardElement Selected = WizardElement::Fire;

	//TArray<FSkillData> FireSkills;
	//TArray<FSkillData> FrostSkills;
	//TArray<FSkillData> WindSkills;
};
