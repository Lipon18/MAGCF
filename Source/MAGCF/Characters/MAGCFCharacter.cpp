// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCFCharacter.h"
#include "MAGCF/MAGCF.h"
#include "MAGCF/World/Buildings/MAGCFBakery.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFAIComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFActionSelectionComponent.h"
#include "MAGCF/AI/Profiles/MAGCFPersonalityDataAsset.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

AMAGCFCharacter::AMAGCFCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    NeedComponent = CreateDefaultSubobject<UMAGCFNeedComponent>(TEXT("NeedComponent"));
    LLMComponent = CreateDefaultSubobject<UMAGCFAIComponent>(TEXT("LLMComponent"));
    ActionSelectionComponent = CreateDefaultSubobject<UMAGCFActionSelectionComponent>(TEXT("ActionSelectionComponent"));
    ActionComponent = CreateDefaultSubobject<UMAGCFActionComponent>(TEXT("ActionComponent"));

    InitializeLookups();
}

void AMAGCFCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(NeedComponent);

    if (LLMComponent)
    {
        LLMComponent->OnActionSelected.AddDynamic(this, &AMAGCFCharacter::HandleActionDecisionReceived);
        LLMComponent->OnLLMActionReceived.AddDynamic(this, &AMAGCFCharacter::HandleLLMActionCallback);
    }
}

void AMAGCFCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateDerivedMoodState();
    UpdateCooldowns(DeltaTime);
}

void AMAGCFCharacter::InitializeLookups()
{
    StringToGoalMap.Empty();
    StringToGoalMap.Add(TEXT("E_EAT"), EMAGCFGoal::E_EAT);
    StringToGoalMap.Add(TEXT("E_DANCE"), EMAGCFGoal::E_DANCE);
    StringToGoalMap.Add(TEXT("E_TALK_TO_PHONE"), EMAGCFGoal::E_TALK_TO_PHONE);
    StringToGoalMap.Add(TEXT("E_SIT_ON_GROUND"), EMAGCFGoal::E_SIT_ON_GROUND);
    StringToGoalMap.Add(TEXT("E_BOX"), EMAGCFGoal::E_BOX);
    StringToGoalMap.Add(TEXT("E_PUSH_UP"), EMAGCFGoal::E_PUSH_UP);

    GoalToStringMap.Empty();
    for (const auto& Pair : StringToGoalMap)
    {
        GoalToStringMap.Add(Pair.Value, Pair.Key);
    }
}

void AMAGCFCharacter::HandleActionDecisionReceived(EMAGCFGoal ChosenGoal, const FString& Reasoning)
{
    if (ChosenGoal == EMAGCFGoal::E_NONE) return;

    CurrentGoal = ChosenGoal;
    ProcessHistoryBuffer(ChosenGoal);
    CooldownMap.Add(ChosenGoal, 8.0f);

    if (ChosenGoal == EMAGCFGoal::E_EAT && !bIsAtBakery)
    {
        auto* const AIC = Cast<AAIController>(GetController());
        if (AIC)
        {
            if (TargetBakery)
            {
                FAIMoveRequest MoveRequest;
                MoveRequest.SetGoalActor(TargetBakery);
                MoveRequest.SetAcceptanceRadius(150.0f);
                AIC->MoveTo(MoveRequest);
                return;
            }
        }
    }
    if (ActionComponent)
    {
        ActionComponent->ExecuteGoal(CurrentGoal);
    }
}

void AMAGCFCharacter::HandleLLMActionCallback(const FString& Action, const FString& TargetDetails)
{
    bIsAtBakery = false;

    if (const auto* const FoundGoal = StringToGoalMap.Find(Action))
    {
        CurrentGoal = *FoundGoal;
    }
    else
    {
        CurrentGoal = EMAGCFGoal::E_NONE;
    }
}

void AMAGCFCharacter::UpdateDerivedMoodState()
{
    if (!NeedComponent) return;

    const auto* const Hunger = NeedComponent->GetNeed(TEXT("Hunger"));
    const auto* const Energy = NeedComponent->GetNeed(TEXT("Energy"));
    const auto* const Fun = NeedComponent->GetNeed(TEXT("Fun"));

    if (Hunger && Hunger->CurrentValue > Hunger->CriticalThreshold)
    {
        CurrentMood = EMAGCFMood::E_IRRITATED;
    }
    else if (Energy && Energy->CurrentValue < 30.0f)
    {
        CurrentMood = EMAGCFMood::E_TIRED;
    }
    else if (Fun && Fun->CurrentValue > Fun->CriticalThreshold)
    {
        CurrentMood = EMAGCFMood::E_BORED;
    }
    else if (Fun && Fun->CurrentValue < 30.0f)
    {
        CurrentMood = EMAGCFMood::E_HAPPY;
    }
    else
    {
        CurrentMood = EMAGCFMood::E_NEUTRAL;
    }
}

void AMAGCFCharacter::ProcessHistoryBuffer(const EMAGCFGoal NewAction)
{
    RecentActions.Insert(NewAction, 0);
    if (RecentActions.Num() > MaxHistorySize)
    {
        RecentActions.Pop(EAllowShrinking::No);
    }
}

int32 AMAGCFCharacter::GetRecentActionOccurrenceCount(const EMAGCFGoal TargetGoal) const
{
    int32 Count = 0;
    for (const auto G : RecentActions)
    {
        if (G == TargetGoal)
        {
            Count++;
        }
    }
    return Count;
}

FString AMAGCFCharacter::GetCharacterArchetypeName() const
{
    return PersonalityConfig ? PersonalityConfig->ArchetypeName : TEXT("Unconfigured");
}

void AMAGCFCharacter::UpdateCooldowns(const float DeltaTime)
{
    for (auto& Element : CooldownMap)
    {
        if (Element.Value > 0.0f)
        {
            Element.Value = FMath::Max(0.0f, Element.Value - DeltaTime);
        }
    }
}

FString AMAGCFCharacter::GetGoalEnumToString(const EMAGCFGoal Goal) const
{
    if (const auto* const FoundStr = GoalToStringMap.Find(Goal))
    {
        return *FoundStr;
    }
    return TEXT("E_NONE");
}

EMAGCFGoal AMAGCFCharacter::GetGoalStringToEnum(const FString& GoalStr) const
{
    if (const auto* const Found = StringToGoalMap.Find(GoalStr))
    {
        return *Found;
    }
    return EMAGCFGoal::E_NONE;
}

bool AMAGCFCharacter::IsBusy() const
{
    if (ActionComponent)
    {
        if (ActionComponent->IsPlayingActionMontage() || ActionComponent->HasSpawnedProp())
        {
            return true;
        }
    }

    const auto* const AI_Controller = Cast<AAIController>(GetController());
    if (AI_Controller)
    {
        const auto Status = AI_Controller->GetMoveStatus();
        if (Status == EPathFollowingStatus::Moving || Status == EPathFollowingStatus::Waiting)
        {
            return true;
        }
    }
    return false;
}

void AMAGCFCharacter::HandleLLMFailure()
{
    EvaluateNeed();
}

void AMAGCFCharacter::EvaluateNeed()
{
    switch (CurrentMood)
    {
        case EMAGCFMood::E_HAPPY: CurrentGoal = EMAGCFGoal::E_DANCE; break;
        case EMAGCFMood::E_TIRED: CurrentGoal = EMAGCFGoal::E_SIT_ON_GROUND; break;
        case EMAGCFMood::E_BORED: CurrentGoal = EMAGCFGoal::E_TALK_TO_PHONE; break;
        case EMAGCFMood::E_NEUTRAL:
        default: CurrentGoal = EMAGCFGoal::E_EAT; break;
    }
}

void AMAGCFCharacter::ExecuteGoal()
{
    if (ActionComponent)
    {
        ActionComponent->ExecuteGoal(CurrentGoal);
    }
}