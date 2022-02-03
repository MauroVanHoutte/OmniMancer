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
	// Sets default values for this actor's properties
	ATestDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TakeSpellDamage(float damage) override;
	void TakeTickDamage(float damage) override;
private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;
};
