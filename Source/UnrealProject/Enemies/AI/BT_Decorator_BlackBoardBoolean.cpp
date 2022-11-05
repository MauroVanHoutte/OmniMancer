// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Decorator_BlackBoardBoolean.h"
#include "BehaviorTree/BlackboardComponent.h"

UBT_Decorator_BlackBoardBoolean::UBT_Decorator_BlackBoardBoolean(const FObjectInitializer& ObjectInitializer)
	: UBTDecorator_BlackboardBase(ObjectInitializer)
{
	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBT_Decorator_BlackBoardBoolean, BlackboardKey));
}


bool UBT_Decorator_BlackBoardBoolean::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(BlackboardKey.SelectedKeyName);
}
