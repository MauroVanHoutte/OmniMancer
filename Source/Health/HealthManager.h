// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HealthManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FManagerDamageTakenSignature, UBaseHealthComponent*, DamagedComponent, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FManagerFatalDamageTakenSignature, UBaseHealthComponent*, DamagedComponent, float, Damage, float, OverkillDamage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UHealthManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthComponentFatalDamage(class UBaseHealthComponent* HealthComponent, float Damage, float OverkillDamage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable)
	FManagerDamageTakenSignature OnDamageTakenDelegate;
	UPROPERTY(BlueprintAssignable)
	FManagerFatalDamageTakenSignature OnFatalDamageTakenDelegate;

private:
	UPROPERTY(Transient)
	TArray<class UBaseHealthComponent*> HealthComponents;

	UPROPERTY(Transient)
	class UTriggerHandlingComponent* PlayerTriggerHandling;
};
