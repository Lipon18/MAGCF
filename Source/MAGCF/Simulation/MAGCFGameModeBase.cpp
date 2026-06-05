// /*===============================================================================
//
//
// MAGCF - Multi-Agent Generative Character Framework
//
// Copyright (c) 2026 Lipon / Psycho Games.
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

#include "MAGCFGameModeBase.h"
#include "MAGCFSpectatorPawn.h"
#include "MAGCF/MAGCF.h"

AMAGCFGameModeBase::AMAGCFGameModeBase() 
{
    DefaultPawnClass = AMAGCFSpectatorPawn::StaticClass();
}

void AMAGCFGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    MAGCF_LOG(TEXT("MAGCF framework initialized."));
}
