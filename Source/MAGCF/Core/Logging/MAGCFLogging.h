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

#include "MAGCFLog.h"

// clang-format off

//
// Core
//

#define MAGCF_LOG(Message) \
FMAGCFLogger::Log( \
    LogMAGCF, \
    ELogVerbosity::Log, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

#define MAGCF_WARNING(Message) \
FMAGCFLogger::Log( \
    LogMAGCF, \
    ELogVerbosity::Warning, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

#define MAGCF_ERROR(Message) \
FMAGCFLogger::Log( \
    LogMAGCF, \
    ELogVerbosity::Error, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

//
// AI
//

#define MAGCF_AI(Message) \
FMAGCFLogger::Log( \
    LogMAGCFAI, \
    ELogVerbosity::Log, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

//
// Needs
//

#define MAGCF_NEED(Message) \
FMAGCFLogger::Log( \
    LogMAGCFNeed, \
    ELogVerbosity::Log, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

//
// Economy
//

#define MAGCF_ECONOMY(Message) \
FMAGCFLogger::Log( \
    LogMAGCFEconomy, \
    ELogVerbosity::Log, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

//
// Memory
//

#define MAGCF_MEMORY(Message) \
FMAGCFLogger::Log( \
    LogMAGCFMemory, \
    ELogVerbosity::Log, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

//
// World
//

#define MAGCF_WORLD(Message) \
FMAGCFLogger::Log( \
    LogMAGCFWorld, \
    ELogVerbosity::Log, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

//
// LLM
//

#define MAGCF_LLM(Message) \
FMAGCFLogger::Log( \
    LogMAGCFLLM, \
    ELogVerbosity::Log, \
    Message, \
    TEXT(__FUNCTION__), \
    __LINE__)

//
// Screen
//

#define MAGCF_SCREEN(Message) \
FMAGCFLogger::LogScreen(Message)

#define MAGCF_SCREEN_WARNING(Message) \
FMAGCFLogger::LogScreen(Message, FColor::Yellow)

#define MAGCF_SCREEN_ERROR(Message) \
FMAGCFLogger::LogScreen(Message, FColor::Red)

// clang-format on