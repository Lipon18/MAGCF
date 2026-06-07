// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MAGCFSettings.generated.h"

UENUM(BlueprintType)
enum class EMAGCFLLMProvider : uint8
{
    EMP_Gemini UMETA(DisplayName = "Google Gemini"),
    EMP_OpenAI UMETA(DisplayName = "OpenAI (GPT-4o/Omni)"),
    EMP_Anthropic UMETA(DisplayName = "Anthropic Claude"),
    EMP_CustomLocal UMETA(DisplayName = "Local / Custom Endpoint (Ollama/LM Studio)")
};

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "MAGCF Framework Settings"))
class MAGCF_API UMAGCFSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UMAGCFSettings();

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "LLM Provider Setup")
    EMAGCFLLMProvider Provider = EMAGCFLLMProvider::EMP_Gemini;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "LLM Provider Setup", meta = (PasswordField = true))
    FString ApiKey;

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "LLM Provider Setup")
    FString ModelName = TEXT("gemini-2.5-flash");

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "LLM Provider Setup")
    FString CustomEndpointOverride;

    /*UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "API Credentials")
    FString ModelEndpoint = TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent");*/
};
