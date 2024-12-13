// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WizardController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractActionTriggeredSignature, AActor*, Player);

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AWizardController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FInteractActionTriggeredSignature OnInteractActionDelegate;
protected:

	UFUNCTION(BlueprintCallable)
	void BroadcastInteractDelegate();
};
