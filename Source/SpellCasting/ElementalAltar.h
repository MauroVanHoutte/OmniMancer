// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalAltar.generated.h"

UCLASS()
class UNREALPROJECT_API AElementalAltar : public AActor
{
	GENERATED_BODY()
	
public:	
	AElementalAltar();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInteractableComponent* InteractableComponent;

private:
	UFUNCTION()
	void OnInteractAction(AActor* Player);
	UFUNCTION()
	void OnInteractRangeExited(AActor* Player);
	UFUNCTION()
	void CloseUiMenu(class UButton* ExitButton);


	UPROPERTY(EditInstanceOnly)
	class ARoomPortal* Portal;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UElementalAltarUi> AltarUiClass;
	UPROPERTY(Transient)
	class UElementalAltarUi* AltarUi;
};
