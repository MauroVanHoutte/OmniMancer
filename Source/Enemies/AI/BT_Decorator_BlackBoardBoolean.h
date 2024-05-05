// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BT_Decorator_BlackBoardBoolean.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UBT_Decorator_BlackBoardBoolean : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBT_Decorator_BlackBoardBoolean(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
