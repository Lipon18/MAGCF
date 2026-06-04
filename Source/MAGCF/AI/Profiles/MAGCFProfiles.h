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
#include "MAGCFProfiles.generated.h"

USTRUCT(BlueprintType)
struct FNPCNeedState
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Need")
    FName NeedName = TEXT("GenericNeed");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Need")
    float CurrentValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Need")
    float MaxValue = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Need")
    float DrainRate = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Need")
    float CriticalThreshold = 80.0f;
};
