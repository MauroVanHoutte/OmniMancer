// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StatusEffectHandlingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UStatusEffectHandlingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusEffectHandlingComponent();

	UFUNCTION(BlueprintCallable)
	void BindStatusEffectsBar(class UActiveStatusEffectsBar* Widget);
	UFUNCTION(BlueprintCallable)
	void ClearStatusEffects();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnUnregister();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyStatusEffect(class UBaseStatusEffect* StatusEffects);
	const TArray<class UBaseStatusEffect*>& GetActiveStatusEffects() { return ActiveStatusEffects; };
private:
	UPROPERTY(Transient)
	TArray<class UBaseStatusEffect*> ActiveStatusEffects;
	UPROPERTY(Transient)
	class UStatusEffectPoolingSubsystem* StatusEffectPooling;
	UPROPERTY(Transient)
	class UActiveStatusEffectsBar* StatusEffectsBar;
};
