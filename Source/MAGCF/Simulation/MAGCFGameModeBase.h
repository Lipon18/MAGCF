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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MAGCFGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class MAGCF_API AMAGCFGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMAGCFGameModeBase();

    virtual void BeginPlay() override;
};
