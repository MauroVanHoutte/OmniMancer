// Fill out your copyright notice in the Description page of Project Settings.


#include "OmnimancerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "OmnimancerSaveGame.h"
#include "Json.h"
#include "Misc/FileHelper.h"

void UOmnimancerGameInstance::Init()
{
	Super::Init();

	//loading unlocks
	UOmnimancerSaveGame* loadedSave;
	if (UGameplayStatics::DoesSaveGameExist("SaveSlot", 0))
	{
		loadedSave = Cast<UOmnimancerSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));
	}
	else
	{
		loadedSave = Cast<UOmnimancerSaveGame>(UGameplayStatics::CreateSaveGameObject(UOmnimancerSaveGame::StaticClass()));
	}

	Currency = 500;//loadedSave->Currency;
	UnlockedUpgrades = loadedSave->UnlockedUpgrades;

	//loading skill names and prices
	/*FString rawJson;
	if (FFileHelper::LoadFileToString(rawJson, *FPaths::ProjectContentDir().Append("/SkillData/Skills.json")))
	{
		TSharedPtr<FJsonObject> parsedJson;
		TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(rawJson);
		if (FJsonSerializer::Deserialize(jsonReader, parsedJson))
		{
			auto typesArray = parsedJson->GetArrayField("Types");
			for (auto type : typesArray)
			{
				FString element;
				element = type->AsObject()->GetStringField("Type");
				auto skills = type->AsObject()->GetArrayField("Skills");
				for (auto skill : skills)
				{
					FSkillData skillData;
					skillData.Name = skill->AsObject()->GetStringField("Name");
					skillData.Description = skill->AsObject()->GetStringField("Tooltip");
					skillData.Cost = skill->AsObject()->GetIntegerField("Cost");

					if (!element.Compare("Fire"))
					{
						FireSkills.Add(skillData);
					}
					if (!element.Compare("Frost"))
					{
						FrostSkills.Add(skillData);
					}
					if (!element.Compare("Wind"))
					{
						WindSkills.Add(skillData);
					}
				}
			}
		}
	}*/
}

void UOmnimancerGameInstance::Shutdown()
{
	Super::Shutdown();
	UOmnimancerSaveGame* save;
	save = Cast<UOmnimancerSaveGame>(UGameplayStatics::CreateSaveGameObject(UOmnimancerSaveGame::StaticClass()));
	save->Currency = Currency;
	save->UnlockedUpgrades = UnlockedUpgrades;
	UGameplayStatics::SaveGameToSlot(save, "SaveSlot", 0);
}

int UOmnimancerGameInstance::GetCurrency()
{
	return Currency;
}

void UOmnimancerGameInstance::SetCurrency(int newCurrency)
{
	Currency = newCurrency;
	OnCurrencyChanged.Broadcast(Currency);
}

WizardElement UOmnimancerGameInstance::GetSelectedType()
{
	return Selected;
}

void UOmnimancerGameInstance::SetSelectedType(WizardElement type)
{
	Selected = type;
}

void UOmnimancerGameInstance::AddUnlockedUpgrade(const FString& tag)
{
	UnlockedUpgrades.Add(tag);
}

void UOmnimancerGameInstance::RemoveUnlockedUpgrade(const FString& tag)
{
	UnlockedUpgrades.Remove(tag);
}

TSet<FString>& UOmnimancerGameInstance::GetUnlockedUpgrades()
{
	return UnlockedUpgrades;
}