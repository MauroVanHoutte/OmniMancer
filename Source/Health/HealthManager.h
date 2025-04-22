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
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	UFUNCTION()
	void OnHealthComponentFatalDamage(class UBaseHealthComponent* HealthComponent, float Damage, float OverkillDamage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddHealthComponent(class UBaseHealthComponent* Component);
	void RemoveHealthComponent(class UBaseHealthComponent* Component);

	UFUNCTION(BlueprintCallable)
	void SetInvulnerability(bool NewInvulnerability);

	UFUNCTION(BlueprintCallable)
	int GetLiveHealthComponentCount();
	UFUNCTION(BlueprintCallable)
	TArray<class UBaseHealthComponent*>& GetHealthComponents();
	UFUNCTION(BlueprintCallable)
	float GetTotalRemainingHealth();
	UFUNCTION(BlueprintCallable)
	float GetTotalRemainingHealthPercentage();
	UFUNCTION(BlueprintCallable)
	void ResetHealthComponents();

	UFUNCTION(BlueprintCallable)
	void Kill(const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void Heal(float HealAmount);

	UPROPERTY(BlueprintAssignable)
	FManagerDamageTakenSignature OnDamageTakenDelegate;
	UPROPERTY(BlueprintAssignable)
	FManagerFatalDamageTakenSignature OnFatalDamageTakenDelegate;

protected:
	UPROPERTY(Transient)
	TArray<class UBaseHealthComponent*> HealthComponents;

	UPROPERTY(Transient)
	class UTriggerHandlingComponent* PlayerTriggerHandling;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bInvulnerable = false;
};
