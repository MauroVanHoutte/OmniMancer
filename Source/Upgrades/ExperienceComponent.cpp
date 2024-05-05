
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
}

int UExperienceComponent::GetExperience()
{
	return Experience;
}
