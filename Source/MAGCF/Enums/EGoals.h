// /*===============================================================================
//
//
// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games.
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
#include "EGoals.generated.h"

UENUM(BlueprintType)
enum class EMAGCFGoal : uint8
{
	E_NONE,
    E_EAT,
    E_DANCE,
    E_SIT_ON_BENCH,
    E_TALK_TO_PHONE
};

UENUM(BlueprintType)
enum class EMAGCFMood : uint8
{
    E_NEUTRAL,
    E_HAPPY,
    E_TIRED,
    E_BORED
};
