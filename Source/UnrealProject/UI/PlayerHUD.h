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
	void SetCooldowns(const TMap<int, float>& Cooldowns, const TMap<int, FTimerHandle>& Timers);

	void SetCurrency(int currency);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Cooldown0;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Cooldown1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Cooldown2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Cooldown3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Cooldown4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Cooldown6;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Currency;
};