// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "Blizzard.generated.h"

class AWizardCharacter;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ABlizzard : public ABaseSpell
{
	GENERATED_BODY()
public:
	ABlizzard();

	void TickBlizzard();

	void SetWizard(AWizardCharacter* wizard);

	void Activate();
	void Deactivate();

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CylinderMesh;
	UPROPERTY(EditAnywhere)
	float TickInterval = 0.2f;
	UPROPERTY(EditAnywhere)
	float SlowAmount = 0.2f;
	UPROPERTY(EditAnywhere)
	float DamagePerTick = 0.4f;
	FTimerHandle ApplicationTimer;
};
