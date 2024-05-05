#pragma once

#include "PlayerTriggerInterace.generated.h"

UINTERFACE(Blueprintable)
class UPlayerTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlayerTriggerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTriggeredByPlayer(class AActor* Player);
};