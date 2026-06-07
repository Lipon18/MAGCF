// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/LLM/MAGCFOpenAIProvider.h"

FString FMAGCFOpenAIProvider::GetRequestURL(const FString& ModelName, const FString& CustomEndpoint) const
{
    if (!CustomEndpoint.IsEmpty()) return CustomEndpoint;
    return TEXT("https://api.openai.com/v1/chat/completions");
}

void FMAGCFOpenAIProvider::ConfigureHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, const FString& ApiKey) const
{
    Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *ApiKey));
}

TSharedPtr<FJsonObject> FMAGCFOpenAIProvider::BuildPayload(const FString& Model, const FString& Prompt) const
{
    TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
    Root->SetStringField(TEXT("model"), Model);
    TSharedPtr<FJsonObject> Msg = MakeShared<FJsonObject>();
    Msg->SetStringField(TEXT("role"), TEXT("user"));
    Msg->SetStringField(TEXT("content"), Prompt);
    TArray<TSharedPtr<FJsonValue>> Messages;
    Messages.Add(MakeShared<FJsonValueObject>(Msg));
    Root->SetArrayField(TEXT("messages"), Messages);
    return Root;
}

bool FMAGCFOpenAIProvider::ExtractResponseText(const FString& ResponseBody, FString& OutText) const
{
    TSharedPtr<FJsonObject> Root;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

    if (!FJsonSerializer::Deserialize(Reader, Root))
    {
        return false;
    }

    const TArray<TSharedPtr<FJsonValue>>* Choices;

    if (!Root->TryGetArrayField(TEXT("choices"), Choices))
    {
        return false;
    }

    const TSharedPtr<FJsonObject>* ChoiceObj;

    if (!(*Choices)[0]->TryGetObject(ChoiceObj))
    {
        return false;
    }

    const TSharedPtr<FJsonObject>* MessageObj;

    if (!(*ChoiceObj)->TryGetObjectField(TEXT("message"), MessageObj))
    {
        return false;
    }
    return (*MessageObj)->TryGetStringField(TEXT("content"), OutText);
}
