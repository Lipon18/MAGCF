// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/MAGCFAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MAGCF/Characters/MAGCFCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "MAGCF/World/Buildings/MAGCFBakery.h"

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

void AMAGCFAIController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);

    if (PerceptionComp)
    {
        PerceptionComp->OnPerceptionUpdated.AddUniqueDynamic(this, &AMAGCFAIController::OnPerceptionUpdated);
    }
}

void AMAGCFAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    auto* const ControlledChar = Cast<AMAGCFCharacter>(GetPawn());
    if (!ControlledChar) return;

    if (Result.IsSuccess())
    {
        if (ControlledChar->GetCurrentGoal() == EMAGCFGoal::E_EAT)
        {
            ControlledChar->SetIsAtBakery(true);
            ControlledChar->ExecuteGoal();
        }
    }
    else if (Result.IsFailure())
    {
        UE_LOG(LogTemp, Warning, TEXT("AI framework navigation path tracking failed. Resetting goal context."));
        ControlledChar->SetCurrentGoal(EMAGCFGoal::E_NONE);
        ControlledChar->SetIsAtBakery(false);
    }
}

void AMAGCFAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (const auto* const Actor : UpdatedActors)
    {
        if (Actor && Actor->IsA<AMAGCFBakery>())
        {
            CachedBakeryTarget = Cast<AMAGCFBakery>(const_cast<AActor*>(Actor));
            break;
        }
    }
}
