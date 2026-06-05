// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#include "MAGCFCharacter.h"
#include "MAGCF/MAGCF.h"
#include "MAGCF/World/Buildings/MAGCFBakery.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFAIComponent.h"

AMAGCFCharacter::AMAGCFCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    NeedComponent = CreateDefaultSubobject<UMAGCFNeedComponent>(TEXT("NeedComponent"));
    LLMComponent = CreateDefaultSubobject<UMAGCFAIComponent>(TEXT("LLMComponent"));
}

void AMAGCFCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(NeedComponent);
    if (LLMComponent)
    {
        LLMComponent->OnLLMActionReceived.AddDynamic(this, &AMAGCFCharacter::HandleLLMActionCallback);
    }

    InitializeExecutionRegistries();

    if (PersonalityConfig)
    {
        MAGCF_NEED(*FString::Printf(TEXT("Character spawned using Archetype profile: %s"), *PersonalityConfig->ArchetypeName));
    }
}

void AMAGCFCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateCurrentMoodState();
    ExecuteGoal();

    /* if (CurrentGoal == EMAGCFGoal::E_NONE && NeedComponent)
     {
         FName CriticalNeed;
         if (NeedComponent->CheckIfAnyNeedIsCritical(CriticalNeed))
         {
             EvaluateNeed();
         }
     }*/
}

void AMAGCFCharacter::InitializeExecutionRegistries()
{
    StringToGoalMap.Add(TEXT("E_EAT"), EMAGCFGoal::E_EAT);
    StringToGoalMap.Add(TEXT("E_DANCE"), EMAGCFGoal::E_DANCE);
    StringToGoalMap.Add(TEXT("E_TALK_TO_PHONE"), EMAGCFGoal::E_TALK_TO_PHONE);
    StringToGoalMap.Add(TEXT("E_SIT_ON_BENCH"), EMAGCFGoal::E_SIT_ON_BENCH);  // Still catch if model chooses old name

    // Map Enum States directly to executable C++ lambda functions (Replacing the Switch statement)
    GoalExecutionMap.Add(EMAGCFGoal::E_EAT, [this]() { HandleEatGoal(); });
    GoalExecutionMap.Add(EMAGCFGoal::E_DANCE, [this]() { HandleDanceGoal(); });
    GoalExecutionMap.Add(EMAGCFGoal::E_SIT_ON_BENCH, [this]() { HandleSitOnGroundGoal(); });
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
    if (TFunction<void()>* GoalRoutine = GoalExecutionMap.Find(CurrentGoal))
    {
        (*GoalRoutine)();
    }
}

void AMAGCFCharacter::HandleDanceGoal()
{
    if (bIsPlayingActionMontage) return;
    if (InternalPlayActionMontage(DanceMontage, TEXT("OnActionMontageEnded")))
    {
        MAGCF_AI(TEXT("Began dancing routine out of absolute happiness."));
    }
    else
    {
        CurrentGoal = EMAGCFGoal::E_NONE;
    }
}

void AMAGCFCharacter::HandleSitOnGroundGoal()
{
    if (bIsPlayingActionMontage) return;

    MAGCF_AI(TEXT("Initiating sitting sequence directly on the floor terrain baseline..."));
    HaltCharacterVelocity();
    SetActorEnableCollision(true);
    if (!InternalPlayActionMontage(GroundSitMontage, TEXT("OnActionMontageEnded")))
    {
        if (GetCharacterMovement()) GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        CurrentGoal = EMAGCFGoal::E_NONE;
    }
}

void AMAGCFCharacter::HandleTalkToPhoneGoal()
{
    if (bIsPlayingActionMontage) return;
    if (InternalPlayActionMontage(PhoneMontage, TEXT("OnActionMontageEnded")))
    {
        MAGCF_AI(TEXT("Pulling out phone to dial a direct social companion."));
    }
    else
    {
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
    if (auto* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
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
    if (NeedComponent) NeedComponent->SatisfyNeed(TEXT("Hunger"), 50.0f);

    bHasBread = false;
    bIsAtBakery = false;
    CurrentGoal = EMAGCFGoal::E_NONE;
    MAGCF_NEED(TEXT("Finished eating bread animation. Framework state reset."));
}

bool AMAGCFCharacter::InternalPlayActionMontage(UAnimMontage* TargetMontage, FName EndWorkerFunctionName)
{
    if (bIsPlayingActionMontage || !TargetMontage) return false;

    if (auto* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
    {
        bIsPlayingActionMontage = true;
        AnimInstance->Montage_Play(TargetMontage);
        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &AMAGCFCharacter::OnActionMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, TargetMontage);
        return true;
    }
    return false;
}

void AMAGCFCharacter::HaltCharacterVelocity()
{
    if (auto* AI_Controller = Cast<AAIController>(GetController()))
    {
        AI_Controller->StopMovement();
    }
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void AMAGCFCharacter::OnActionMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsPlayingActionMontage = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }

    if (NeedComponent)
    {
        if (Montage == DanceMontage)
            NeedComponent->SatisfyNeed(TEXT("Fun"), 50.0f);
        else if (Montage == PhoneMontage)
            NeedComponent->SatisfyNeed(TEXT("Fun"), 35.0f);
        else if (Montage == GroundSitMontage)
            NeedComponent->SatisfyNeed(TEXT("Energy"), 60.0f);
    }
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
                                "    {\"Action\": \"E_SIT_ON_BENCH\", \"Description\": \"Triggered when systemic Energy drops low to drop and rest sitting on the ground directly.\", \"Prerequisite\": \"None\"},\n"
                                "    {\"Action\": \"E_TALK_TO_PHONE\", \"Description\": \"Dial up a direct social companion to reduce accumulated Boredom values.\", \"Prerequisite\": \"None\"}\n"
                                "  ]\n"
                                "}"),
        *Archetype, *FormattedTags, Social, Strength, Spend, PatienceVal, Calmness, *Flavor, ThresholdPrice, *ExtractedNeedsMapData, Money,
        *MoodString);
}

void AMAGCFCharacter::HandleLLMActionCallback(FString Action, FString TargetDetails)
{
    MAGCF_AI(*FString::Printf(TEXT("LLM Brain completed evaluation. Parsed Action: %s"), *Action));
    bIsAtBakery = false;

    if (EMAGCFGoal* FoundGoal = StringToGoalMap.Find(Action))
    {
        CurrentGoal = *FoundGoal;
    }
    else
    {
        CurrentGoal = EMAGCFGoal::E_NONE;
    }
    CurrentGoal = EMAGCFGoal::E_NONE;
}
