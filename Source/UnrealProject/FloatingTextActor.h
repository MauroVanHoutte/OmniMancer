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
	AFloatingTextActor();

	UFUNCTION(BlueprintImplementableEvent)
	void Initialize(const FText& text, FColor color);

	inline const FVector& GetAnchorLocation() const { return AnchorLocation; };

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	FVector AnchorLocation;
};
