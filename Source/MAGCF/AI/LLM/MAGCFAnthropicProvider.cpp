// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/LLM/MAGCFAnthropicProvider.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

FString FMAGCFAnthropicProvider::GetRequestURL(const FString& ModelName, const FString& CustomEndpoint) const
{
    // TODO: implement full Anthropic endpoint
    return CustomEndpoint.IsEmpty() ? TEXT("https://api.anthropic.com/v1/messages") : CustomEndpoint;
}

void FMAGCFAnthropicProvider::ConfigureHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, const FString& ApiKey) const
{
    // TODO: implement full Anthropic header setup
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("x-api-key"), ApiKey);
    Request->SetHeader(TEXT("anthropic-version"), TEXT("2023-06-01"));
}

TSharedPtr<FJsonObject> FMAGCFAnthropicProvider::BuildPayload(const FString& Model, const FString& Prompt) const
{
    // TODO: implement full Anthropic messages payload
    TSharedPtr<FJsonObject> RootJson = MakeShared<FJsonObject>();
    RootJson->SetStringField(TEXT("model"), Model.IsEmpty() ? TEXT("claude-sonnet-4-20250514") : Model);
    RootJson->SetNumberField(TEXT("max_tokens"), 1024);

    TArray<TSharedPtr<FJsonValue>> MessagesArray;
    TSharedPtr<FJsonObject> MessageObject = MakeShared<FJsonObject>();
    MessageObject->SetStringField(TEXT("role"), TEXT("user"));
    MessageObject->SetStringField(TEXT("content"), Prompt);
    MessagesArray.Add(MakeShared<FJsonValueObject>(MessageObject));
    RootJson->SetArrayField(TEXT("messages"), MessagesArray);

    return RootJson;
}

bool FMAGCFAnthropicProvider::ExtractResponseText(const FString& ResponseBody, FString& OutText) const
{
    // TODO: implement full Anthropic response extraction
    TSharedPtr<FJsonObject> RootJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
    if (FJsonSerializer::Deserialize(Reader, RootJson) && RootJson.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* ContentArray = nullptr;
        if (RootJson->TryGetArrayField(TEXT("content"), ContentArray) && ContentArray && ContentArray->Num() > 0)
        {
            TSharedPtr<FJsonObject> ContentObj = (*ContentArray)[0]->AsObject();
            if (ContentObj.IsValid())
            {
                if (ContentObj->TryGetStringField(TEXT("text"), OutText))
                {
                    return true;
                }
            }
        }
    }
    return false;
}