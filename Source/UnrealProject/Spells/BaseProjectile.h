// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameplayTagContainer.h>

#include "BaseProjectile.generated.h"

UCLASS()
class UNREALPROJECT_API ABaseProjectile : public ABaseSpell
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& direction);

	void OnHit(AActor* hitActor) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	USphereComponent* m_CollisionComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_Mesh;
	UPROPERTY(EditAnywhere)
	float m_LifeTime = 5.f;

};
