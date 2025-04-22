#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

UCLASS(Abstract)
class UNREALPROJECT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void Setup();

	void SetCurrency(int currency);

	UFUNCTION(BlueprintCallable)
	void SetExperiencePercentage(float expPercentage);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Currency;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Experience;

protected:
	AActor* Player;
	UPROPERTY(BlueprintReadOnly)
	class UElementManipulationComponent* SpellCastingComponent;
	class UStatComponent* StatComponent;
};