// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include <Components/MeshComponent.h>
#include <Components/BoxComponent.h>

#include "TestDummy.generated.h"

UCLASS()
class UNREALPROJECT_API ATestDummy : public ABaseCharacter
{
	GENERATED_BODY()
	
public:	
	ATestDummy();

	virtual void Tick(float DeltaTime) override;
	void TakeSpellDamage(float damage) override;
	void TakeTickDamage(float damage) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DummyMesh;
};
