// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractRangeEnteredSignature, AActor*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractRangeExitedSignature, AActor*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractActionSignature, AActor*, Player);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALPROJECT_API UInteractableComponent : public USphereComponent
{
	GENERATED_BODY()
public:
	UInteractableComponent();

	virtual void OnRegister() override;
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

	UPROPERTY(BlueprintAssignable)
	FInteractRangeEnteredSignature OnInteractRangeEnteredDelegate;
	UPROPERTY(BlueprintAssignable)
	FInteractRangeExitedSignature OnInteractRangeExitedDelegate;
	UPROPERTY(BlueprintAssignable)
	FInteractActionSignature OnInteractActionDelegate;

protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* Widget;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* InteractAction;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnInteractAction(AActor* Player);

	void HandleBeginOverlap(class AWizardController* Controller, AActor* Actor);
	void HandleEndOverlap(class AWizardController* Controller, AActor* Actor);

	void SetMappedKeyOnWidget(class AWizardController* Controller);

	UPROPERTY(EditDefaultsOnly)
	FName PlayerTag = TEXT("Player");
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInteractWidget> WidgetClass;
};
