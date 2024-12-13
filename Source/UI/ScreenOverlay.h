// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScreenOverlay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeComponentCompletedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UScreenOverlay : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScreenOverlay();

	void FadeInOverlay(float Duration);
	void FadeOutOverlay(float Duration);

	UPROPERTY(BlueprintAssignable)
	FFadeComponentCompletedSignature OnFadeComponentCompletedSignature;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnWidgetFadeCompleted();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UOverlayWidget> WidgetClass;
	class UOverlayWidget* Widget;
};
