// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseDashComponent.generated.h"


UCLASS( ClassGroup=(Custom), abstract )
class UNREALPROJECT_API UBaseDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseDashComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class UCharacterMovementComponent* CharacterMovement, class UStatComponent* StatComponent);

	UFUNCTION(BlueprintCallable)
	void TryExecuteDash();

protected:
	virtual void ExecuteDash() {};
	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteDashBP();

	UPROPERTY(Transient)
	class UCharacterMovementComponent* CharacterMovement = nullptr;
	UPROPERTY(Transient)
	class UStatComponent* Stats = nullptr;
private:
	float CalculateCooldown() const;

	UPROPERTY(EditDefaultsOnly)
	float Cooldown;
	FTimerHandle CooldownTimer;
};
