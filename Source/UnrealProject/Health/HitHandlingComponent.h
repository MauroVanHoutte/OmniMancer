// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "HitHandlingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJECT_API UHitHandlingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitHandlingComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(const TArray<class UShapeComponent*>& Colliders, UAffiliationComponent* inAffiliationComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnColliderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<UShapeComponent*> Colliders;
	class UAffiliationComponent* AffiliationComponent;
};
