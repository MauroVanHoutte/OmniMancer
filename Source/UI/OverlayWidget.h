// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeCompletedSignature);

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void FadeInOverlay(float Duration);
	void FadeOutOverlay(float Duration);

	virtual bool Initialize() override;

	UPROPERTY(BlueprintAssignable)
	FFadeCompletedSignature OnFadeCompletedDelegate;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	class UWidgetAnimation* GetFadeAnimation();

	UFUNCTION()
	void OnAnimationCompleted();

	UPROPERTY(Transient)
	class UUMGSequencePlayer* SequencePlayer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* OverlayImage;
};
