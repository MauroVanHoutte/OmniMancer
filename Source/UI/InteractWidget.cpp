// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractWidget.h"
#include <Components/TextBlock.h>

void UInteractWidget::SetKeyText(const FText& NewText)
{
	KeyText->SetText(NewText);
}
