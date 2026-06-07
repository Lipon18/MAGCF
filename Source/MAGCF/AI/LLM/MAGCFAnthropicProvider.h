// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "MAGCF/AI/LLM/MAGCFLLMProviderBase.h"
#include "Serialization/JsonSerializer.h"
#include "Http.h"

class MAGCF_API FMAGCFAnthropicProvider : public IMAGCFLLMProvider
{
public:
    virtual ~FMAGCFAnthropicProvider() override = default;

    virtual FString GetRequestURL(const FString& ModelName, const FString& CustomEndpoint) const override;
    virtual void ConfigureHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, const FString& ApiKey) const override;
    virtual TSharedPtr<FJsonObject> BuildPayload(const FString& Model, const FString& Prompt) const override;
    virtual bool ExtractResponseText(const FString& ResponseBody, FString& OutText) const override;
};