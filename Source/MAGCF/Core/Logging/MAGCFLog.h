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
#include "Engine/Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMAGCF, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMAGCFAI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMAGCFNeed, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMAGCFEconomy, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMAGCFMemory, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMAGCFWorld, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMAGCFLLM, Log, All);

class MAGCF_API FMAGCFLogger
{
public:
    static void Log(const FLogCategoryBase& Category, ELogVerbosity::Type Verbosity, const FString& Message, const FString& Function, int32 Line);
    static void LogScreen(const FString& Message, const FColor& Color = FColor::Green, float Duration = 5.0f);
};
