// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpelUpgrade.h"

FFormatNamedArguments UBPBaseSpellUpgrade::GetDescriptionArguments()
{
    FFormatNamedArguments Args = FFormatNamedArguments();
    TMap<FString, FText> StringArgs = BP_GetDescriptionArguments();
    for (auto kvp : StringArgs)
    {
        Args.Add(kvp.Key, kvp.Value);
    }
    return  Args;
}
