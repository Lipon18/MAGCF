// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/AI/Profiles/MAGCFPersonalityDataAsset.h"

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
    HungerNeed.DrainRate = 0.08f;
    HungerNeed.CriticalThreshold = 80.0f;
    TrackedNeeds.Add(TEXT("Hunger"), HungerNeed);

    FNPCNeedState EnergyNeed;
    EnergyNeed.NeedName = TEXT("Energy");
    EnergyNeed.CurrentValue = 85.0f;
    EnergyNeed.MaxValue = 100.0f;
    EnergyNeed.DrainRate = 0.05f;
    EnergyNeed.CriticalThreshold = 75.0f;   
    TrackedNeeds.Add(TEXT("Energy"), EnergyNeed);

    FNPCNeedState FunNeed;
    FunNeed.NeedName = TEXT("Fun");
    FunNeed.CurrentValue = 30.0f;
    FunNeed.MaxValue = 100.0f;
    FunNeed.DrainRate = 0.12f;
    FunNeed.CriticalThreshold = 70.0f;
    TrackedNeeds.Add(TEXT("Fun"), FunNeed);
}

void UMAGCFNeedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (auto& Element : TrackedNeeds)
    {
        auto& Need = Element.Value;
        const float DirectionMultiplier = IsInverseNeed(Need.NeedName) ? -1.0f : 1.0f;
        Need.CurrentValue += (DeltaTime * Need.DrainRate * DirectionMultiplier);
        Need.CurrentValue = FMath::Clamp(Need.CurrentValue, 0.0f, Need.MaxValue);
    }
}

void UMAGCFNeedComponent::InitializeNeedsFromPersonality(const UMAGCFPersonalityDataAsset* Personality)
{
    if (!Personality) return;

    if (auto* const Hunger = TrackedNeeds.Find(TEXT("Hunger")))
    {
        Hunger->DrainRate *= (1.35f - (Personality->Fortitude * 0.35f));
    }
    if (auto* const Energy = TrackedNeeds.Find(TEXT("Energy")))
    {
        Energy->DrainRate *= (1.25f - (Personality->Stability * 0.25f));
    }
    if (auto* const Fun = TrackedNeeds.Find(TEXT("Fun")))
    {
        Fun->DrainRate *= (0.5f + (Personality->SocialTendency * 1.0f));
    }
}

FNPCNeedState* UMAGCFNeedComponent::GetNeed(FName NeedName)
{
    return TrackedNeeds.Find(NeedName);
}

const FNPCNeedState* UMAGCFNeedComponent::GetNeed(FName NeedName) const
{
    return TrackedNeeds.Find(NeedName);
}

void UMAGCFNeedComponent::SatisfyNeed(FName NeedName, float Amount)
{
    if (auto* const Need = GetNeed(NeedName))
    {
        Need->CurrentValue = FMath::Clamp(Need->CurrentValue - Amount, 0.0f, Need->MaxValue);
    }
}

void UMAGCFNeedComponent::RestoreNeed(FName NeedName, float Amount)
{
    if (auto* const Need = GetNeed(NeedName))
    {
        Need->CurrentValue = FMath::Clamp(Need->CurrentValue + Amount, 0.0f, Need->MaxValue);
    }
}

bool UMAGCFNeedComponent::CheckIfAnyNeedIsCritical(FName& OutCriticalNeedName)
{
    static const TArray<FName> PriorityChecklist = {TEXT("Hunger"), TEXT("Energy"), TEXT("Fun")};

    for (const auto& NeedName : PriorityChecklist)
    {
        if (const auto* const Need = TrackedNeeds.Find(NeedName))
        {
            if (IsNeedCritical(NeedName, *Need))
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