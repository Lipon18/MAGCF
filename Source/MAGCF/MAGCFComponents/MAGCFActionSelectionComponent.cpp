// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/MAGCFComponents/MAGCFActionSelectionComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"

UMAGCFActionSelectionComponent::UMAGCFActionSelectionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

TArray<FMAGCFCandidateAction> UMAGCFActionSelectionComponent::GenerateCandidateActions(int32 MaxCandidates)
{
    TArray<FMAGCFCandidateAction> Candidates;

    if (!CachedCharacter)
    {
        CachedCharacter = Cast<AMAGCFCharacter>(GetOwner());
        if (!CachedCharacter) return Candidates;
    }

    const uint8 StartGoal = static_cast<uint8>(EMAGCFGoal::E_EAT);
    const uint8 EndGoal = static_cast<uint8>(EMAGCFGoal::E_PUSH_UP);

    for (uint8 i = StartGoal; i <= EndGoal; ++i)
    {
        const EMAGCFGoal TestGoal = static_cast<EMAGCFGoal>(i);
        if (CachedCharacter->GetCurrentGoal() == TestGoal && CachedCharacter->IsBusy())
        {
            continue;
        }
        float Score = CalculateBaseUtility(TestGoal, CachedCharacter);
        const int32 Occurrences = CachedCharacter->GetRecentActionOccurrenceCount(TestGoal);
        if (Occurrences > 0)
        {
            const float PenaltyMultiplier = FMath::Pow(0.25f, static_cast<float>(Occurrences));
            Score *= PenaltyMultiplier;
        }
        if (Score > 0.05f)
        {
            FMAGCFCandidateAction NewCandidate;
            NewCandidate.Goal = TestGoal;
            NewCandidate.FinalScore = Score;
            Candidates.Add(NewCandidate);
        }
    }
    Candidates.Sort([](const FMAGCFCandidateAction& A, const FMAGCFCandidateAction& B) { return A.FinalScore > B.FinalScore; });
    if (Candidates.Num() > MaxCandidates)
    {
        Candidates.SetNum(MaxCandidates);
    }
    return Candidates;
}

float UMAGCFActionSelectionComponent::CalculateBaseUtility(EMAGCFGoal Goal, const AMAGCFCharacter* OwnerChar) const
{
    const auto* const NeedComp = OwnerChar->GetNeedComponent();
    if (!NeedComp) return 0.0f;

    const auto* const Hunger = NeedComp->GetNeed(TEXT("Hunger"));
    const auto* const Energy = NeedComp->GetNeed(TEXT("Energy"));
    const auto* const Fun = NeedComp->GetNeed(TEXT("Fun"));

    switch (Goal)
    {
        case EMAGCFGoal::E_EAT: return Hunger ? Hunger->CurrentValue : 0.0f;

        case EMAGCFGoal::E_SIT_ON_GROUND:
            return Energy ? (100.0f - Energy->CurrentValue) : 0.0f;

        case EMAGCFGoal::E_DANCE:
            if (Energy && Energy->CurrentValue < 40.0f) return 0.0f;
            return Fun ? Fun->CurrentValue : 0.0f;

        case EMAGCFGoal::E_TALK_TO_PHONE: return Fun ? (Fun->CurrentValue * 0.8f) : 0.0f;

        case EMAGCFGoal::E_BOX:
        case EMAGCFGoal::E_PUSH_UP:
            if (Energy && Energy->CurrentValue < 50.0f) return 0.0f;
            return (Energy && Fun) ? ((Energy->CurrentValue + Fun->CurrentValue) * 0.5f) : 0.0f;

        default: break;
    }
    return 0.0f;
}