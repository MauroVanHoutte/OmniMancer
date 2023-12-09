#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Elements.generated.h"

UENUM(BlueprintType)
enum class WizardElement : uint8
{
	None,
	Fire,
	Frost,
	Wind
};

UCLASS(meta = (ScriptName = "ElementLibrary"))
class UNREALPROJECT_API UElementLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		static int GetSpellIDFromElements(TArray<WizardElement> Elements)
	{
		int ID = 0;
		Elements.Sort();

		for (size_t i = 0; i < Elements.Num(); i++)
		{
			ID += FMath::RoundToInt(FMath::Pow(10, static_cast<float>(i))) * static_cast<int>(Elements[i]);
		}

		return ID;
	}
};
