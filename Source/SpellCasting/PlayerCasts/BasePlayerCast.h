// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BasePlayerCast.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastStartedSignature, class UBasePlayerCast*, CastObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCastSignature, class UBasePlayerCast*, CastObject, class ABaseSpell*, CastSpell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCastHitSignature, class UBasePlayerCast*, CastObject, class ABaseSpell*, CastSpell, class AActor*, HitActor);

UCLASS(Abstract, EditInlineNew, BlueprintType)
class UNREALPROJECT_API UBasePlayerCast : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Initialize(class UElementManipulationComponent* SpellCasting, class UStatComponent* Stats, class AActor* CastOwner, class APlayerController* CastController, class USceneComponent* CastOrigin);

	UFUNCTION(BlueprintNativeEvent)
	void HandleCastInputPressed();
	UFUNCTION(BlueprintNativeEvent)
	void HandleCastInputDown();
	UFUNCTION(BlueprintNativeEvent)
	void HandleCastInputReleased();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRemainingCooldown();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTotalCooldownDuration();
	UFUNCTION(BlueprintCallable)
	void SetRemainingCooldown(float NewRemainingCooldown);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDamageMultiplier();
	void SetCastOrigin(class USceneComponent* NewOrigin) { Origin = NewOrigin; };

	UPROPERTY(BlueprintAssignable)
	FCastStartedSignature OnCastStartedDelegate;
	UPROPERTY(BlueprintAssignable)
	FCastSignature OnCastDelegate;
	UPROPERTY(BlueprintAssignable)
	FCastHitSignature OnCastHitDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	float BaseCooldown = 5.f;
	FTimerHandle CooldownTimer;

	UPROPERTY(Transient, BlueprintReadOnly)
	class UElementManipulationComponent* SpellCastingComponent;
	UPROPERTY(Transient, BlueprintReadOnly)
	class UStatComponent* StatComponent;
	UPROPERTY(Transient, BlueprintReadOnly)
	class AActor* Owner;
	UPROPERTY(Transient, BlueprintReadOnly)
	class APlayerController* Controller;
	UPROPERTY(Transient, BlueprintReadOnly)
	class USceneComponent* Origin;
};
