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

#include "MAGCF/AI/MAGCFAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MAGCF/Characters/MAGCFCharacter.h"
#include "Navigation/PathFollowingComponent.h"

AMAGCFAIController::AMAGCFAIController() 
{
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    if (PerceptionComp && SightConfig)
    {
        SightConfig->SightRadius = 3000.0f;
        SightConfig->LoseSightRadius = 3500.0f;
        SightConfig->PeripheralVisionAngleDegrees = 90.0f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        PerceptionComp->ConfigureSense(*SightConfig);
        PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
        PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AMAGCFAIController::OnPerceptionUpdated);
    }
}

void AMAGCFAIController::BeginPlay() 
{
    Super::BeginPlay();
}

void AMAGCFAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) 
{
    Super::OnMoveCompleted(RequestID, Result);

    auto* ControlledChar = Cast<AMAGCFCharacter>(GetPawn());
    if (!ControlledChar) return;

    if (Result.IsSuccess())
    {
        if (ControlledChar->GetCurrentGoal() == EMAGCFGoal::E_EAT)
        {
            ControlledChar->SetIsAtBakery(true);
        }
        else if (ControlledChar->GetCurrentGoal() == EMAGCFGoal::E_SIT_ON_BENCH)
        {
            ControlledChar->SetIsAtBench(true);
        }

        ControlledChar->ForceExecuteGoal();
    }
    else if (Result.IsFailure())
    {
        UE_LOG(LogTemp, Warning, TEXT("AI framework navigation path tracking failed. Resetting goal context."));
        ControlledChar->SetCurrentGoal(EMAGCFGoal::E_NONE);
        ControlledChar->SetIsAtBench(false);
        ControlledChar->SetIsAtBakery(false);
    }
}

void AMAGCFAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) 
{
    for (auto* Actor : UpdatedActors)
    {
        if (Actor)
        {
            // cache the bakery when it is seen
        }
    }
}
