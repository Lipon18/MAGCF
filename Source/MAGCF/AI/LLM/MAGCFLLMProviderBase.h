// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Dom/JsonObject.h"

class IMAGCFLLMProvider
{
public:
    virtual ~IMAGCFLLMProvider() = default;
    virtual FString GetRequestURL(const FString& ModelName, const FString& CustomEndpoint) const = 0;
    virtual void ConfigureHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, const FString& ApiKey) const = 0;
    virtual TSharedPtr<FJsonObject> BuildPayload(const FString& Model, const FString& Prompt) const = 0;
    virtual bool ExtractResponseText(const FString& ResponseBody, FString& OutText) const = 0;
};
