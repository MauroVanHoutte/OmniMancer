// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Decorator_BlackboardInRange.h"
#include "BehaviorTree/BlackboardComponent.h"


void UBT_Decorator_BlackboardInRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		RangeKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize %s due to missing blackboard data."), *GetName());
		RangeKey.InvalidateResolvedKey();
	}
}

EBlackboardNotificationResult UBT_Decorator_BlackboardInRange::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorComp = (UBehaviorTreeComponent*)Blackboard.GetBrainComponent();
	if (BehaviorComp == nullptr)
		return EBlackboardNotificationResult::RemoveObserver;

	if (BlackboardKey.GetSelectedKeyID() == ChangedKeyID)
		BehaviorComp->RequestExecution(this);

	if (RangeKey.GetSelectedKeyID() == ChangedKeyID)
		BehaviorComp->RequestExecution(this);

	return EBlackboardNotificationResult::ContinueObserving;
}

UBT_Decorator_BlackboardInRange::UBT_Decorator_BlackboardInRange(const FObjectInitializer& ObjectInitializer)
{
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBT_Decorator_BlackboardInRange, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBT_Decorator_BlackboardInRange, BlackboardKey));

	RangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBT_Decorator_BlackboardInRange, RangeKey));
}

void UBT_Decorator_BlackboardInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		auto KeyID = BlackboardKey.GetSelectedKeyID();
		BlackboardComp->RegisterObserver(KeyID, this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_BlackboardBase::OnBlackboardKeyValueChange));

		KeyID = RangeKey.GetSelectedKeyID();
		BlackboardComp->RegisterObserver(KeyID, this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTDecorator_BlackboardBase::OnBlackboardKeyValueChange));
	}
}

void UBT_Decorator_BlackboardInRange::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
		BlackboardComp->UnregisterObserversFrom(this);
}

bool UBT_Decorator_BlackboardInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (!BlackboardKey.IsSet() || !RangeKey.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("In range key not set"));
		return false;
	}

	auto blackboard = OwnerComp.GetBlackboardComponent();

	FVector selfLocation;
	blackboard->GetLocationFromEntry(FName("SelfActor"), selfLocation);
	FVector targetLocation;
	blackboard->GetLocationFromEntry(BlackboardKey.GetSelectedKeyID(), targetLocation);

	float range = blackboard->GetValueAsFloat(RangeKey.SelectedKeyName);

	if (FVector::DistSquared(selfLocation, targetLocation) < range * range)
		return true;
	return false;
}
