
#include "ExperienceComponent.h"

// Sets default values for this component's properties
UExperienceComponent::UExperienceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UExperienceComponent::AddExperience(int AddedExperience)
{
	Experience += AddedExperience;

	if (Experience >= ExperienceToNextLevel)
	{
		Experience -= ExperienceToNextLevel;
		Level++;
		OnLevelledUpDelegate.Broadcast(this, Level);
		ExperienceToNextLevel = CalculateExperienceForNextLevel();
		if (Experience >= ExperienceToNextLevel)
		{
			FSimpleDelegate Delegate;
			Delegate.BindUObject(this, &UExperienceComponent::AddExperience, 0);
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, 0.1, false);
		}
	}

	OnExperienceAddedDelegate.Broadcast(this, AddedExperience, Experience, ExperienceToNextLevel);
}

int UExperienceComponent::GetExperience()
{
	return Experience;
}

int UExperienceComponent::GetExperienceToNextLevel()
{
	return ExperienceToNextLevel - Experience;
}

int UExperienceComponent::GetLevel()
{
	return Level;
}

float UExperienceComponent::CalculateExperienceForNextLevel_Implementation()
{
	return int(1 * FMath::Pow(1.4, Level));
}
