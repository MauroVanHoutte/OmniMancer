// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Spells/FlameColumn.h"
#include "Spells/IceZone.h"
#include "Spells/IceWall.h"
#include "Spells/Tornado.h"
#include "Spells/ChainLightning.h"
#include "Spells/Shockwave.h"
#include <Components/BillboardComponent.h>
#include "FloatingTextActor.h"
#include "Enemies/BaseCharacter.h"

#include "WizardCharacter.generated.h"

UENUM()
enum class WizardElement
{
	Fire = 0,
	Frost = 1,
	Wind = 3
};

UCLASS()
class UNREALPROJECT_API AWizardCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWizardCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TakeSpellDamage(float damage) override;
	void TakeTickDamage(float damage) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnAddElement();

	UFUNCTION(BlueprintCallable)
	TArray<int> GetCurrentElements();

	UFUNCTION(BlueprintCallable)
	TMap<int, float>& GetCooldowns();
	UFUNCTION(BlueprintCallable)
	TMap<int, float>& GetCooldownCounters();

private:
	void MoveUp(float value);
	void MoveRight(float value);
	void ChangeTurnDirection();
	void Fire(); //Base Projectile
	void CastSpell();

	void CastFlameColumn(const FVector& worldPos);
	void CastIceZone(const FVector& worldPos);
	void CastIceWall(const FVector& worldPos);
	void CastTornado(const FVector& direction);
	void CastChainLightning(const FVector& direction);
	void CastShockwave();

	void AddElement(WizardElement element);
	DECLARE_DELEGATE_OneParam(FAddElementDelegate, WizardElement);


	UPROPERTY(EditAnywhere)
	USpringArmComponent* m_SpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* m_Camera;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere)
	TArray<WizardElement> m_CurrentElements{};
	int m_MaxElements = 2;
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	TSubclassOf<AFlameColumn> m_FlameColumnClass = nullptr;
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	TSubclassOf<AIceZone> m_IceZoneClass = nullptr;
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	TSubclassOf<AIceWall> m_IceWallClass = nullptr;
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	TSubclassOf<ATornado> m_TornadoClass = nullptr;
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	TSubclassOf<AChainLightning> m_ChainLighningClass = nullptr;
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	TSubclassOf<AShockwave> m_ShockwaveClass = nullptr;


	UPROPERTY(EditAnywhere)
	float m_TurnSpeed = 60.f;
	UPROPERTY(EditAnywhere)
	float m_Rotation = 0.f;
	UPROPERTY(EditAnywhere)
	int m_TurnDirection = 1.f;
	UPROPERTY(EditAnywhere)
	float m_FovAngle = 90.f;

	UPROPERTY(EditAnywhere)
	int m_SpeedPerWind = 10;
	UPROPERTY(EditAnywhere)
	int m_DamagePerFire = 10;
	UPROPERTY(EditAnywhere)
	int m_DamageReductionPerFrost = 10;
	UPROPERTY(EditAnywhere)
	int m_BaseDamageMultiplier = 20;
	UPROPERTY(EditAnywhere)
	int m_DamageTakenMultiplier = 0;

	UPROPERTY(EditAnywhere)
	UBillboardComponent* m_FirstElementBillboard = nullptr; 
	UPROPERTY(EditAnywhere)
	UBillboardComponent* m_SecondElementBillboard = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* m_FireElementTexture = nullptr;
	UPROPERTY(EditAnywhere)
	UTexture2D* m_FrostElementTexture = nullptr;
	UPROPERTY(EditAnywhere)
	UTexture2D* m_WindElementTexture = nullptr;

	// Cooldowns
	UPROPERTY(EditAnywhere)
	TMap<int, float> m_Cooldowns{};
	UPROPERTY(EditAnywhere)
	TMap<int, float> m_CooldownCounter{};
};
