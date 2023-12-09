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
	void TryExecuteDash();

protected:
	virtual void ExecuteDash() {};
	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteDashBP();

	UPROPERTY(Transient, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UCharacterMovementComponent* CharacterMovementComponent = nullptr;

private:
	UPROPERTY(EditDefaultsOnly)
	float Cooldown;
	FTimerHandle CooldownTimer;
};
