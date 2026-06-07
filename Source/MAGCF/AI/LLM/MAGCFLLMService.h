// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "MAGCFLLMService.generated.h"

class IMAGCFLLMProvider;

DECLARE_DELEGATE_OneParam(FMAGCFLLMSuccess, const FString&);
DECLARE_DELEGATE_OneParam(FMAGCFLLMFailure, const FString&);

UCLASS()
class MAGCF_API UMAGCFLLMService : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(
        TSharedPtr<IMAGCFLLMProvider> InProvider, const FString& InApiKey, const FString& InModel, const FString& InCustomEndpoint);
    void RequestCompletion(const FString& Prompt, FMAGCFLLMSuccess Success, FMAGCFLLMFailure Failure);

private:
    void OnResponse(
        FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FMAGCFLLMSuccess Success, FMAGCFLLMFailure Failure);

    TSharedPtr<IMAGCFLLMProvider> Provider;

    FString ApiKey;
    FString Model;
    FString CustomEndpointOverride;

    FMAGCFLLMSuccess PendingSuccess;
    FMAGCFLLMFailure PendingFailure;
};