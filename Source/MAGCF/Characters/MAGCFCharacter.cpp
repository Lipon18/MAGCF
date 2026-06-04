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

#include "MAGCFCharacter.h"
#include "MAGCF/MAGCF.h"
#include "MAGCF/World/Buildings/MAGCFBakery.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"

AMAGCFCharacter::AMAGCFCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    NeedComponent = CreateDefaultSubobject<UMAGCFNeedComponent>(TEXT("NeedComponent"));
}

void AMAGCFCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(NeedComponent);

    if (PersonalityConfig)
    {
        MAGCF_NEED(*FString::Printf(TEXT("Character spawned using Archetype profile: %s"), *PersonalityConfig->ArchetypeName));
    }
    else
    {
        MAGCF_WARNING(TEXT("Character spawned without an assigned Personality Config Data Asset!"));
    }
}

void AMAGCFCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentGoal == EMAGCFGoal::E_NONE && NeedComponent)
    {
        FName CriticalNeed;
        if (NeedComponent->CheckIfAnyNeedIsCritical(CriticalNeed))
        {
            if (CriticalNeed == TEXT("Hunger"))
            {
                EvaluateNeed();
            }
        }
    }
    ExecuteGoal();
}

void AMAGCFCharacter::EvaluateNeed()
{
    CurrentGoal = EMAGCFGoal::E_EAT;
    MAGCF_AI(TEXT("Selected Eat Goal"));
}

void AMAGCFCharacter::ExecuteGoal()
{
    switch (CurrentGoal)
    {
        case EMAGCFGoal::E_EAT: HandleEatGoal(); break;
        default: break;
    }
}

void AMAGCFCharacter::HandleEatGoal()
{
    if (!bIsAtBakery)
    {
        MAGCF_AI(TEXT("Going to bakery."));
        bIsAtBakery = true;
        return;
    }
    float TargetPrice = TargetBakery ? TargetBakery->BreadPrice : 5.0f;

    if (!bHasBread)
    {
        FString CurrentContextJSON = CompileLLMContextPayload();
        MAGCF_LLM(*FString::Printf(TEXT("Current Snapshot Context Vector:\n%s"), *CurrentContextJSON));
        if (PersonalityConfig)
        {
            if (TargetPrice > PersonalityConfig->PriceThreshold)
            {
                FString ReactionType = (PersonalityConfig->Stability < 0.4f) ? TEXT("panics nervously") : TEXT("grumpily evaluates code");
                FString StrengthType = (PersonalityConfig->Fortitude < 0.4f) ? TEXT("weakly surrenders") : TEXT("stoutly persists");
                MAGCF_AI(
                    *FString::Printf(TEXT("Price Check Simulation: John notices item costs %.2f. Profile limits says %.2f. He %s and %s."),
                        TargetPrice, PersonalityConfig->PriceThreshold, *ReactionType, *StrengthType));
            }
        }
        if (Money >= TargetPrice)
        {
            Money -= TargetPrice;
            bHasBread = true;
            MAGCF_ECONOMY(TEXT("Bought Bread"));
        }
        return;
    }
    if (bHasBread)
    {
        if (NeedComponent)
        {
            NeedComponent->SatisfyNeed(TEXT("Hunger"), 50.0f);
        }
        bHasBread = false;
        MAGCF_NEED(TEXT("Ate Bread"));
        CurrentGoal = EMAGCFGoal::E_NONE;
        bIsAtBakery = false;
    }
}

FString AMAGCFCharacter::CompileLLMContextPayload() const
{
    FString Archetype = TEXT("Unconfigured");
    float Social = 0.5f, Strength = 0.5f, Spend = 0.5f, PatienceVal = 0.5f, Calmness = 0.5f;
    FString Flavor = TEXT("Savory");
    float ThresholdPrice = 5.0f;
    FString FormattedTags = TEXT("");

    if (PersonalityConfig)
    {
        Archetype = PersonalityConfig->ArchetypeName;
        Social = PersonalityConfig->SocialTendency;
        Strength = PersonalityConfig->Fortitude;
        Spend = PersonalityConfig->SpendingTendency;
        PatienceVal = PersonalityConfig->Patience;
        Calmness = PersonalityConfig->Stability;
        Flavor = PersonalityConfig->PreferredFlavor;
        ThresholdPrice = PersonalityConfig->PriceThreshold;

        FString Joined = FString::Join(PersonalityConfig->BehavioralFlavors, TEXT("\", \""));
        if (!Joined.IsEmpty())
        {
            FormattedTags = TEXT("\"") + Joined + TEXT("\"");
        }
    }

    FString ExtractedNeedsMapData = NeedComponent ? NeedComponent->GetNeedsAsJSONString() : TEXT("\"Hunger\": 0.00");

    return FString::Printf(TEXT("{\n"
                                "  \"Identity\": {\"Archetype\": \"%s\", \"Tags\": [%s]},\n"
                                "  \"Psychology\": {\"IntroversionExtroversion\": %.2f, \"StrengthFortitude\": %.2f, "
                                "\"FrugalityExtravagance\": %.2f, \"Patience\": %.2f, \"EmotionalStability\": %.2f},\n"
                                "  \"Taste\": {\"PreferredFlavor\": \"%s\", \"ComfortPriceLimit\": %.2f},\n"
                                "  \"LiveState\": {\"Hunger\": %.2f, \"WalletBalance\": %.2f, \"CarryingFood\": %s}\n"
                                "}"),
        *Archetype, *FormattedTags, Social, Strength, Spend, PatienceVal, Calmness, *Flavor, ThresholdPrice, *ExtractedNeedsMapData, Money,
        bHasBread ? TEXT("true") : TEXT("false"));
}
