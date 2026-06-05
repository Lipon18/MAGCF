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
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

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

    UpdateCurrentMoodState();

    if (CurrentGoal == EMAGCFGoal::E_NONE && NeedComponent)
    {
        FName CriticalNeed;
        if (NeedComponent->CheckIfAnyNeedIsCritical(CriticalNeed))
        {
            EvaluateNeed();
        }
    }
    ExecuteGoal();
}

void AMAGCFCharacter::UpdateCurrentMoodState()
{
    if (!NeedComponent) return;

    auto* Hunger = NeedComponent->GetNeed(TEXT("Hunger"));
    auto* Energy = NeedComponent->GetNeed(TEXT("Energy"));
    auto* Fun = NeedComponent->GetNeed(TEXT("Fun"));

    CurrentMood = EMAGCFMood::E_NEUTRAL;

    if (Energy && Energy->CurrentValue > 65.0f)
    {
        CurrentMood = EMAGCFMood::E_TIRED;
    }
    else if (Fun && Fun->CurrentValue > 60.0f)
    {
        CurrentMood = EMAGCFMood::E_BORED;
    }
    else if (Hunger && Hunger->CurrentValue < 30.0f && Fun && Fun->CurrentValue < 30.0f)
    {
        CurrentMood = EMAGCFMood::E_HAPPY;
    }
}

void AMAGCFCharacter::EvaluateNeed()
{
    switch (CurrentMood)
    {
        case EMAGCFMood::E_HAPPY:
            CurrentGoal = EMAGCFGoal::E_DANCE;
            MAGCF_AI(TEXT("Mood Profile is HAPPY: Selecting Dance action routine."));
            break;

        case EMAGCFMood::E_TIRED:
            CurrentGoal = EMAGCFGoal::E_SIT_ON_BENCH;
            MAGCF_AI(TEXT("Mood Profile is TIRED: Selecting Sit On Bench action routine."));
            break;

        case EMAGCFMood::E_BORED:
            CurrentGoal = EMAGCFGoal::E_TALK_TO_PHONE;
            MAGCF_AI(TEXT("Mood Profile is BORED: Selecting Phone call interaction."));
            break;

        case EMAGCFMood::E_NEUTRAL:
        default:
            CurrentGoal = EMAGCFGoal::E_EAT;
            MAGCF_AI(TEXT("Mood Profile is NEUTRAL/HUNGRY: Standard routing to food source."));
            break;
    }
}

void AMAGCFCharacter::ExecuteGoal()
{
    switch (CurrentGoal)
    {
        case EMAGCFGoal::E_EAT: HandleEatGoal(); break;
        case EMAGCFGoal::E_DANCE: HandleDanceGoal(); break;
        case EMAGCFGoal::E_SIT_ON_BENCH: HandleSitOnBenchGoal(); break;
        case EMAGCFGoal::E_TALK_TO_PHONE: HandleTalkToPhoneGoal(); break;
        default: break;
    }
}

void AMAGCFCharacter::HandleDanceGoal()
{
    if (bIsPlayingActionMontage) return;

    auto* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
    if (AnimInstance && DanceMontage)
    {
        bIsPlayingActionMontage = true;
        float Length = AnimInstance->Montage_Play(DanceMontage);
        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &AMAGCFCharacter::OnActionMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, DanceMontage);
        MAGCF_AI(TEXT("Began dancing routine out of absolute happiness."));
    }
    else
    {
        bIsPlayingActionMontage = false;
        CurrentGoal = EMAGCFGoal::E_NONE;
    }
}

void AMAGCFCharacter::HandleSitOnBenchGoal()
{
    if (!TargetBench)
    {
        MAGCF_WARNING(TEXT("No physical bench assigned in map workspace. Simulating structural rest."));
        if (NeedComponent)
        {
            NeedComponent->SatisfyNeed(TEXT("Energy"), 40.0f);
        }
        CurrentGoal = EMAGCFGoal::E_NONE;
        return;
    }

    if (!bIsAtBench)
    {
        auto* AI_Controller = Cast<AAIController>(GetController());
        if (AI_Controller)
        {
            if (AI_Controller->GetMoveStatus() == EPathFollowingStatus::Idle)
            {
                FAIMoveRequest MoveRequest;
                MoveRequest.SetGoalActor(TargetBench);
                MoveRequest.SetAcceptanceRadius(120.0f);
                MoveRequest.SetUsePathfinding(true);
                MoveRequest.SetAllowPartialPath(true);
                FPathFollowingRequestResult Result = AI_Controller->MoveTo(MoveRequest);
                if (Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
                {
                    bIsAtBench = true;
                }
                else if (Result.Code == EPathFollowingRequestResult::Failed)
                {
                    MAGCF_WARNING(TEXT("Bench pathing failed instantly. Aborting goal gracefully."));
                    CurrentGoal = EMAGCFGoal::E_NONE;  // Reset state
                }
            }
        }
        return;
    }

    if (bIsPlayingActionMontage) return;

    auto* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
    if (AnimInstance && BenchSitMontage)
    {
        bIsPlayingActionMontage = true;
        SetActorEnableCollision(false);
        AnimInstance->Montage_Play(BenchSitMontage);
        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &AMAGCFCharacter::OnActionMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, BenchSitMontage);
    }
}

void AMAGCFCharacter::HandleTalkToPhoneGoal()
{
    if (bIsPlayingActionMontage) return;

    auto* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
    if (AnimInstance && PhoneMontage)
    {
        bIsPlayingActionMontage = true;
        float Length = AnimInstance->Montage_Play(PhoneMontage);
        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &AMAGCFCharacter::OnActionMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, PhoneMontage);
        MAGCF_AI(TEXT("Pulling out phone to dial a direct social companion."));
    }
    else
    {
        bIsPlayingActionMontage = false;
        CurrentGoal = EMAGCFGoal::E_NONE;
    }
}

void AMAGCFCharacter::HandleEatGoal()
{
    if (!bIsAtBakery)
    {
        auto* AI_Controller = Cast<AAIController>(GetController());
        if (AI_Controller)
        {
            if (AI_Controller->GetMoveStatus() == EPathFollowingStatus::Idle)
            {
                MAGCF_AI(TEXT("Requesting movement path to bakery."));
                FAIMoveRequest MoveRequest;
                MoveRequest.SetGoalActor(TargetBakery);
                MoveRequest.SetAcceptanceRadius(80.0f);
                MoveRequest.SetUsePathfinding(true);
                MoveRequest.SetAllowPartialPath(false);
                FPathFollowingRequestResult Result = AI_Controller->MoveTo(MoveRequest);

                if (Result.Code == EPathFollowingRequestResult::AlreadyAtGoal)
                {
                    bIsAtBakery = true;
                    MAGCF_AI(TEXT("NPC is already at the bakery. Skipping walk."));
                }
            }
        }
        return;
    }

    float TargetPrice = TargetBakery ? TargetBakery->BreadPrice : 5.0f;

    if (!bHasBread)
    {
        FString CurrentContextJSON = CompileLLMContextPayload();
        MAGCF_LLM(*FString::Printf(TEXT("Current Snapshot Context Vector:\n%s"), *CurrentContextJSON));
        if (Money >= TargetPrice)
        {
            Money -= TargetPrice;
            bHasBread = true;
            StartEatingSequence();
        }
        return;
    }
}

void AMAGCFCharacter::StartEatingSequence()
{
    if (!EatMontage)
    {
        if (NeedComponent)
        {
            NeedComponent->SatisfyNeed(TEXT("Hunger"), 50.0f);
        }
        bHasBread = false;
        CurrentGoal = EMAGCFGoal::E_NONE;
        bIsAtBakery = false;
        return;
    }
    if (BreadClass && GetMesh())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        SpawnedBreadActor = GetWorld()->SpawnActor<AActor>(BreadClass, GetActorLocation(), GetActorRotation(), SpawnParams);
        if (SpawnedBreadActor)
        {
            SpawnedBreadActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);
        }
    }
    auto* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(EatMontage);
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.BindUObject(this, &AMAGCFCharacter::OnEatMontageEnded);
        AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, EatMontage);
    }
}

void AMAGCFCharacter::OnEatMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (SpawnedBreadActor)
    {
        SpawnedBreadActor->Destroy();
        SpawnedBreadActor = nullptr;
    }
    if (NeedComponent)
    {
        NeedComponent->SatisfyNeed(TEXT("Hunger"), 50.0f);
    }
    bHasBread = false;
    bIsAtBakery = false;
    CurrentGoal = EMAGCFGoal::E_NONE;
    MAGCF_NEED(TEXT("Finished eating bread animation. Framework state reset."));
}

void AMAGCFCharacter::OnActionMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsPlayingActionMontage = false;
    SetActorEnableCollision(true);

    if (NeedComponent)
    {
        if (Montage == DanceMontage)
        {
            NeedComponent->SatisfyNeed(TEXT("Fun"), 50.0f);
        }
        else if (Montage == PhoneMontage)
        {
            NeedComponent->SatisfyNeed(TEXT("Fun"), 35.0f);
        }
        else if (Montage == BenchSitMontage)
        {
            NeedComponent->SatisfyNeed(TEXT("Energy"), 60.0f);
        }
    }

    bIsAtBench = false;
    CurrentGoal = EMAGCFGoal::E_NONE;
    MAGCF_AI(TEXT("Completed custom physical action montage routine. State vectors cleared."));
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

    FString MoodString = TEXT("Neutral");
    switch (CurrentMood)
    {
        case EMAGCFMood::E_HAPPY: MoodString = TEXT("Happy"); break;
        case EMAGCFMood::E_TIRED: MoodString = TEXT("Tired"); break;
        case EMAGCFMood::E_BORED: MoodString = TEXT("Bored"); break;
        default: break;
    }
    return FString::Printf(TEXT("{\n"
                                "  \"Identity\": {\"Archetype\": \"%s\", \"Tags\": [%s]},\n"
                                "  \"Psychology\": {\"IntroversionExtroversion\": %.2f, \"StrengthFortitude\": %.2f, \"FrugalityExtravagance\": %.2f, \"Patience\": %.2f, \"EmotionalStability\": %.2f},\n"
                                "  \"Taste\": {\"PreferredFlavor\": \"%s\", \"ComfortPriceLimit\": %.2f},\n"
                                "  \"LiveState\": {%s, \"WalletBalance\": %.2f, \"CurrentMoodProfile\": \"%s\"},\n"
                                "  \"AllowedActionSchema\": [\n"
                                "    {\"Action\": \"E_EAT\", \"Description\": \"Triggered when Hunger is high or mood profile returns Neutral.\", \"Prerequisite\": \"Available Wallet Capital >= Price\"},\n"
                                "    {\"Action\": \"E_DANCE\", \"Description\": \"Triggered out of absolute comfort when target is structurally Happy.\", \"Prerequisite\": \"None\"},\n"
                                "    {\"Action\": \"E_SIT_ON_BENCH\", \"Description\": \"Triggered when systemic Energy drops low to initiate resting protocols.\", \"Prerequisite\": \"TargetBench reference present in scene layout\"},\n"
                                "    {\"Action\": \"E_TALK_TO_PHONE\", \"Description\": \"Dial up a direct social companion to reduce accumulated Boredom values.\", \"Prerequisite\": \"None\"}\n"
                                "  ]\n"
                                "}"),
        *Archetype, *FormattedTags, Social, Strength, Spend, PatienceVal, Calmness, *Flavor, ThresholdPrice, *ExtractedNeedsMapData, Money,
        *MoodString);
}