// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UI/InteractWidget.h"
#include <Components/TextBlock.h>
#include <Animation/UMGSequencePlayer.h>
#include <Animation/WidgetAnimation.h>
#include "OverlayWidget.h"

void UOverlayWidget::FadeInOverlay(float Duration)
{
	UWidgetAnimation* WidgetAnimation = GetFadeAnimation();
	
	if (IsValid(WidgetAnimation))
	{
		float StartTime = 0.f;
		if (IsValid(SequencePlayer))
		{
			if (SequencePlayer->GetPlaybackStatus() == EMovieScenePlayerStatus::Playing)
			{
				FQualifiedFrameTime Time = SequencePlayer->GetCurrentTime();
				StartTime = Time.AsSeconds();
				SequencePlayer->Stop();
			}
		}
		SequencePlayer = PlayAnimation(WidgetAnimation, StartTime, 1, EUMGSequencePlayMode::Forward, 1.f / Duration);
	}
}

void UOverlayWidget::FadeOutOverlay(float Duration)
{
	UWidgetAnimation* WidgetAnimation = GetFadeAnimation();
	
	if (IsValid(WidgetAnimation))
	{
		float StartTime = 0.f;
		if (IsValid(SequencePlayer))
		{
			if (SequencePlayer->GetPlaybackStatus() == EMovieScenePlayerStatus::Playing)
			{
				FQualifiedFrameTime Time = SequencePlayer->GetCurrentTime();
				StartTime = Time.AsSeconds();
				SequencePlayer->Stop();
			}
		}
		SequencePlayer = PlayAnimation(WidgetAnimation, StartTime, 1, EUMGSequencePlayMode::Reverse, 1.f / Duration);
	}
}

bool UOverlayWidget::Initialize()
{
	bool ReturnVal = UUserWidget::Initialize();
	
	UWidgetAnimation* WidgetAnimation = GetFadeAnimation();
	if (IsValid(WidgetAnimation))
	{
		FWidgetAnimationDynamicEvent Delegate;
		Delegate.BindDynamic(this, &UOverlayWidget::OnAnimationCompleted);
		BindToAnimationFinished(WidgetAnimation, Delegate);
	}

	return ReturnVal;
}

void UOverlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(SequencePlayer))
	{
		FQualifiedFrameTime Time = SequencePlayer->GetCurrentTime();
		GEngine->AddOnScreenDebugMessage(744747, 1, FColor::Black, FString::Printf(TEXT("Time: %f"), Time.AsSeconds()), true);
	}
}

void UOverlayWidget::OnAnimationCompleted()
{
	OnFadeCompletedDelegate.Broadcast();
}
