// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingTextActor.generated.h"

UCLASS()
class UNREALPROJECT_API AFloatingTextActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingTextActor();

	UFUNCTION(BlueprintImplementableEvent)
	void Initialize(const FText& text, FColor color);

	inline const FVector& GetAnchorLocation() const { return m_AnchorLocation; };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector m_AnchorLocation;
};
