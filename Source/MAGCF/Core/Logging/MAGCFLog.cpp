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

#include "MAGCFLog.h"

DEFINE_LOG_CATEGORY(LogMAGCF);
DEFINE_LOG_CATEGORY(LogMAGCFAI);
DEFINE_LOG_CATEGORY(LogMAGCFNeed);
DEFINE_LOG_CATEGORY(LogMAGCFEconomy);
DEFINE_LOG_CATEGORY(LogMAGCFMemory);
DEFINE_LOG_CATEGORY(LogMAGCFWorld);
DEFINE_LOG_CATEGORY(LogMAGCFLLM);

void FMAGCFLogger::Log(
    const FLogCategoryBase& Category, ELogVerbosity::Type Verbosity, const FString& Message, const FString& Function, int32 Line)
{
    FString FinalMessage = FString::Printf(TEXT("[%s:%d] %s"), *Function, Line, *Message);
    FMsg::Logf(__FILE__, Line, Category.GetCategoryName(), Verbosity, TEXT("%s"), *FinalMessage);
}

void FMAGCFLogger::LogScreen(const FString& Message, const FColor& Color, float Duration)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
    }
}
