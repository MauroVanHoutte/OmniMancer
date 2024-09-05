// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "BaseHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FDamageTakenSignature, UBaseHealthComponent*, DamagedComponent, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FFatalDamageTakenSignature, UBaseHealthComponent*, DamagedComponent, float, Damage, float, OverKillDamage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(Blueprintable, EditInlineNew, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UBaseHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UHealthManager;

public:
	// Sets default values for this component's properties
	UBaseHealthComponent();

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
	virtual void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float NewCurrentHealth, bool ClampToMaxHealth = true);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage() const;
	UFUNCTION(BlueprintCallable)
	void SetCurrentHealthPercentage(float NewPercentage);

	void Kill(const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool CanBeHealed() const;
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
	bool bCanBeHealed = true;
	UPROPERTY(EditDefaultsOnly)
	bool bSpawnDamageText = true;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFloatingTextActor> DamageFloatingTextClass = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFloatingTextActor> HealFloatingTextClass = nullptr;
	UPROPERTY(BlueprintReadWrite)
	class UHealthbar* BoundHealthbar = nullptr;

	//Recovery
	void StartRegenerating();
	UPROPERTY(EditDefaultsOnly)
	bool bRegenerates = false;
	UPROPERTY(EditDefaultsOnly)
	bool bIsRegenerating = false;
	FTimerHandle RegenerationTimer;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bRegenerates"))
	float RegenerationCooldown = 5.f;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bRegenerates"))
	float RegenerationRate = 5.f;
	//

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