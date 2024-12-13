// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardController.h"

void AWizardController::BroadcastInteractDelegate()
{
	OnInteractActionDelegate.Broadcast(GetPawn());
}
