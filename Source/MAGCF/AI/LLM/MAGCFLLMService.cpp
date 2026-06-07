// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/LLM/MAGCFLLMService.h"
#include "MAGCF/AI/LLM/MAGCFLLMProviderBase.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

void UMAGCFLLMService::Initialize(
    TSharedPtr<IMAGCFLLMProvider> InProvider, const FString& InApiKey, const FString& InModel, const FString& InCustomEndpoint)
{
    Provider = InProvider;
    ApiKey = InApiKey;
    Model = InModel;
    CustomEndpointOverride = InCustomEndpoint;
}

void UMAGCFLLMService::RequestCompletion(const FString& Prompt, FMAGCFLLMSuccess Success, FMAGCFLLMFailure Failure) 
{
    if (!Provider.IsValid())
    {
        Failure.ExecuteIfBound(TEXT("Provider is null"));
        return;
    }

    auto Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Provider->GetRequestURL(Model, CustomEndpointOverride));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Provider->ConfigureHeaders(Request, ApiKey);

    TSharedPtr<FJsonObject> Payload = Provider->BuildPayload(Model, Prompt);
    FString Body;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Body);

    if (FJsonSerializer::Serialize(Payload.ToSharedRef(), Writer))
    {
        Request->SetContentAsString(Body);
        Request->OnProcessRequestComplete().BindUObject(this, &UMAGCFLLMService::OnResponse, Success, Failure);
        Request->ProcessRequest();
    }
    else
    {
        Failure.ExecuteIfBound(TEXT("Serialization Failed"));
    }
}

void UMAGCFLLMService::OnResponse(
    FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FMAGCFLLMSuccess Success, FMAGCFLLMFailure Failure)
{
    if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
    {
        FString ServerErrorReason = Response.IsValid() ? Response->GetContentAsString() : TEXT("No response body");
        Failure.ExecuteIfBound(
            FString::Printf(TEXT("HTTP Request Failed. Code: %d | Server Message: %s"), Response.IsValid() ? Response->GetResponseCode() : 0, *ServerErrorReason));
        return;
    }
    FString ExtractedText;
    if (Provider->ExtractResponseText(Response->GetContentAsString(), ExtractedText))
    {
        Success.ExecuteIfBound(ExtractedText);
    }
    else
    {
        Failure.ExecuteIfBound(TEXT("Failed to extract text from provider response structure."));
    }
}
