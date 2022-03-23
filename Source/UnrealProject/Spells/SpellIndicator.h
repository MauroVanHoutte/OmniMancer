// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellIndicator.generated.h"

UCLASS()
class UNREALPROJECT_API ASpellIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_CylinderMesh;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_CylinderProgressMesh;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_BoxMesh;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_BoxProgressMesh;

	USceneComponent* m_Root;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartCircleIndicator(const FVector& scale, float lifetime);

private:
	
	float m_Duration;
	float m_Timer;
	bool m_Active = false;
	FVector m_Scale;

};
