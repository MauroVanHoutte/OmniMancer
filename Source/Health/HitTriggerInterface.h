#pragma once
#include "HitTriggerInterface.generated.h"

UINTERFACE(Blueprintable)
class UHitTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

class IHitTriggerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTriggered(class AActor* TriggeringActor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class UAffiliationComponent* GetAffiliation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool WasActorHitBefore(class AActor* TriggeringActor);
};