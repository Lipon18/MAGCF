// /*===============================================================================
//
//
// MAGCF - Multi-Agent Generative Character Framework
//
// Copyright (c) 2026 Your Lipon / Psycho Games.
//
// All Rights Reserved.
//
// MAGCF is an experimental research framework for autonomous AI-driven characters and multi-agent simulation within Unreal Engine
// environments.
//
// Unauthorized copying, modification, distribution, or use of this software
//
// without explicit permission is prohibited.
//
//
// ===============================================================================*/

#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"

UMAGCFNeedComponent::UMAGCFNeedComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMAGCFNeedComponent::BeginPlay()
{
    Super::BeginPlay();

    FNPCNeedState HungerNeed;
    HungerNeed.NeedName = TEXT("Hunger");
    HungerNeed.CurrentValue = 85.0f;
    HungerNeed.MaxValue = 100.0f;
    HungerNeed.DrainRate = 2.0f;
    HungerNeed.CriticalThreshold = 80.0f;

    TrackedNeeds.Add(TEXT("Hunger"), HungerNeed);
}

void UMAGCFNeedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (auto& Element : TrackedNeeds)
    {
        FNPCNeedState& Need = Element.Value;
        Need.CurrentValue += DeltaTime * Need.DrainRate;
        Need.CurrentValue = FMath::Clamp(Need.CurrentValue, 0.0f, Need.MaxValue);
    }
}

FNPCNeedState* UMAGCFNeedComponent::GetNeed(FName NeedName)
{
    return TrackedNeeds.Find(NeedName);
}

void UMAGCFNeedComponent::SatisfyNeed(FName NeedName, float Amount) 
{
    if (FNPCNeedState* Need = GetNeed(NeedName))
    {
        Need->CurrentValue = FMath::Clamp(Need->CurrentValue - Amount, 0.0f, Need->MaxValue);
    }
}

bool UMAGCFNeedComponent::CheckIfAnyNeedIsCritical(FName& OutCriticalNeedName)
{
    for (const auto& Element : TrackedNeeds)
    {
        if (Element.Value.CurrentValue >= Element.Value.CriticalThreshold)
        {
            OutCriticalNeedName = Element.Key;
            return true;
        }
    }
    return false;
}

FString UMAGCFNeedComponent::GetNeedsAsJSONString() const
{
    TArray<FString> JSONRows;
    for (const auto& Element : TrackedNeeds)
    {
        JSONRows.Add(FString::Printf(TEXT("\"%s\": %.2f"), *Element.Key.ToString(), Element.Value.CurrentValue));
    }
    return FString::Join(JSONRows, TEXT(", "));
}
