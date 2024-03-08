// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "BaseHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FDamageTakenSignature, UBaseHealthComponent*, DamagedActor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FFatalDamageTakenSignature, UBaseHealthComponent*, DamagedActor, float, Damage, float, OverKillDamage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UBaseHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void BindHealthbar(class UWidgetComponent* Healthbar);

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void SpawnDamageText(float damage);

	float GetMaxHealth() const;
	void SetMaxHealth(float NewMaxHealth);

	float GetCurrentHealth() const;
	void SetCurrentHealth(float NewCurrentHealth, bool ClampToMaxHealth);

	float GetHealthPercentage() const;
	void SetCurrentHealthPercentage(float NewPercentage);

	void Kill(const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool IsDepleted() const;
	//Only important when a HealthManager is present
	//Higher priority health components takes damage first
	UFUNCTION(BlueprintPure)
	float GetPriority() const;
	//Only important when a HealthManager is present
	//If false damage will also be applied to the next highest priority health component
	UFUNCTION(BlueprintPure)
	bool GetBlocksDamage() const;
	//Only important when a HealthManager is present
	//If true overkill damage spills over to next highest priority health component
	UFUNCTION(BlueprintPure)
	bool GetOverflows() const;

	UPROPERTY(BlueprintAssignable)
	FDamageTakenSignature OnDamageTakenDelegate;
	UPROPERTY(BlueprintAssignable)
	FFatalDamageTakenSignature OnFatalDamageTakenDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bStartFullHealth = true;
	UPROPERTY(VisibleAnywhere, meta = (EditCondition = "!StartFullHealth"))
	float CurrentHealth = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bStartDepleted = false;
	UPROPERTY(EditDefaultsOnly)
	bool bSpawnDamageText = true;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFloatingTextActor> FloatingTextClass = nullptr;
	UPROPERTY(BlueprintReadWrite)
	class UHealthbar* BoundHealthbar = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDepleted = false;

	//Only important when a HealthManager is present
	//Higher priority health components takes damage first
	UPROPERTY(EditDefaultsOnly)
	int Priority = 0;

	//Only important when a HealthManager is present
	//If false damage will also be applied to the next highest priority health component
	UPROPERTY(EditDefaultsOnly)
	bool bBlockDamage = true;

	//Only important when a HealthManager is present
	//If true overkill damage spills over to next highest priority health component
	UPROPERTY(EditDefaultsOnly)
	bool bOverflow = true;
};