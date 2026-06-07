// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

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
