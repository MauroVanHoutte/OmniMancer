// Fill out your copyright notice in the Description page of Project Settings.


#include "Healthbar.h"
#include <Components/ProgressBar.h>

void UHealthbar::SetHealthPercentage(float HealthPercentage)
{
	Healthbar->SetPercent(HealthPercentage);
}
