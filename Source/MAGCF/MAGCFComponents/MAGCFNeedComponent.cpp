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
    HungerNeed.CurrentValue = 20.0f;
    HungerNeed.MaxValue = 100.0f;
    HungerNeed.DrainRate = 2.0f;
    HungerNeed.CriticalThreshold = 80.0f;
    TrackedNeeds.Add(TEXT("Hunger"), HungerNeed);

    FNPCNeedState EnergyNeed;
    EnergyNeed.NeedName = TEXT("Energy");
    EnergyNeed.CurrentValue = 10.0f;
    EnergyNeed.MaxValue = 100.0f;
    EnergyNeed.DrainRate = 1.0f;
    EnergyNeed.CriticalThreshold = 75.0f;
    TrackedNeeds.Add(TEXT("Energy"), EnergyNeed);

    FNPCNeedState FunNeed;
    FunNeed.NeedName = TEXT("Fun");
    FunNeed.CurrentValue = 30.0f;
    FunNeed.MaxValue = 100.0f;
    FunNeed.DrainRate = 2.5f;
    FunNeed.CriticalThreshold = 70.0f;
    TrackedNeeds.Add(TEXT("Fun"), FunNeed);
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
    TArray<FName> PriorityChecklist = {TEXT("Hungry"), TEXT("Energy"), TEXT("Fun")};

    for (const FName& NeedName : PriorityChecklist)
    {
        if (auto* Need = TrackedNeeds.Find(NeedName))
        {
            if (Need->CurrentValue >= Need->CriticalThreshold)
            {
                OutCriticalNeedName = NeedName;
                return true;
            }
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
