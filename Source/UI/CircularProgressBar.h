// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CircularProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UCircularProgressBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetFillPercentage(float FillPercentage);

	UFUNCTION(BlueprintCallable)
	void SetImage(UTexture2D* ImageTexture);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* Image;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UImage* Progress;
	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* ProgressMaterial;
};
