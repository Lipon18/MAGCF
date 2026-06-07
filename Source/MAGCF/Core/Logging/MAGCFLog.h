// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

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
