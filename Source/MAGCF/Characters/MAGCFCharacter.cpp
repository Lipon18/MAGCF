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

AMAGCFCharacter::AMAGCFCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMAGCFCharacter::BeginPlay()
{
    Super::BeginPlay();

    Hunger = 20.0f;

    MAGCF_NEED(TEXT("John spawned."));
}

void AMAGCFCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Hunger += DeltaTime * 2.0f;
    Hunger = FMath::Clamp(Hunger, 0.0f, MaxHunger);

    if (Hunger >= 80.0f && CurrentGoal == EMAGCFGoal::E_NONE)
    {
        EvaluateNeed();
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
    if (!bHasBread)
    {
        if (Money >= 5.0f)
        {
            Money -= 5.0f;
            bHasBread = true;
            MAGCF_ECONOMY(TEXT("Bought Bread"));
        }
        return;
    }
    if (bHasBread)
    {
        Hunger -= 50.0f;
        bHasBread = false;
        MAGCF_NEED(TEXT("Ate Bread"));
        CurrentGoal = EMAGCFGoal::E_NONE;
        bIsAtBakery = false;
    }
}
