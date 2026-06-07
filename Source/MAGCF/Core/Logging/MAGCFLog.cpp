// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

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
