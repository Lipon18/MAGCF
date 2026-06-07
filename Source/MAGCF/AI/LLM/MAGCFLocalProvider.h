// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MAGCF/AI/LLM/MAGCFLLMProviderBase.h"
#include "Serialization/JsonSerializer.h"
#include "Http.h"

class FMAGCFLocalProvider : public IMAGCFLLMProvider
{
public:
    virtual ~FMAGCFLocalProvider() override = default;

    virtual FString GetRequestURL(const FString& ModelName, const FString& CustomEndpoint) const override
    {
        return CustomEndpoint.IsEmpty() ? TEXT("http://localhost:11434/v1/chat/completions") : CustomEndpoint;
    }

    virtual void ConfigureHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, const FString& ApiKey) const override
    {
        UE_LOG(LogTemp, Display, TEXT("FMAGCFLocalProvider::ConfigureHeaders called"));
        Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

        // Local Ollama endpoints will crash or reject requests if passed an empty Bearer token array header.
        // We override this to leave headers completely pristine and clean for local loopback.
    }

    virtual TSharedPtr<FJsonObject> BuildPayload(const FString& Model, const FString& Prompt) const override
    {
        TSharedPtr<FJsonObject> RootJson = MakeShared<FJsonObject>();
        RootJson->SetStringField(TEXT("model"), Model.IsEmpty() ? TEXT("llama3.2") : Model);
        TArray<TSharedPtr<FJsonValue>> MessagesArray;
        TSharedPtr<FJsonObject> MessageObject = MakeShared<FJsonObject>();
        MessageObject->SetStringField(TEXT("role"), TEXT("user"));
        MessageObject->SetStringField(TEXT("content"), Prompt);
        MessagesArray.Add(MakeShared<FJsonValueObject>(MessageObject));
        RootJson->SetArrayField(TEXT("messages"), MessagesArray);
        TSharedPtr<FJsonObject> ResponseFormatObject = MakeShared<FJsonObject>();
        ResponseFormatObject->SetStringField(TEXT("type"), TEXT("json_object"));
        RootJson->SetObjectField(TEXT("response_format"), ResponseFormatObject);
        RootJson->SetNumberField(TEXT("temperature"), 0.2);

        return RootJson;
    }

    virtual bool ExtractResponseText(const FString& ResponseBody, FString& OutText) const override
    {
        TSharedPtr<FJsonObject> RootJson;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

        if (FJsonSerializer::Deserialize(Reader, RootJson) && RootJson.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>>* ChoicesArray = nullptr;
            if (RootJson->TryGetArrayField(TEXT("choices"), ChoicesArray) && ChoicesArray && ChoicesArray->Num() > 0)
            {
                TSharedPtr<FJsonObject> ChoiceObj = (*ChoicesArray)[0]->AsObject();
                if (ChoiceObj.IsValid())
                {
                    const TSharedPtr<FJsonObject>* MessageObj = nullptr;
                    if (ChoiceObj->TryGetObjectField(TEXT("message"), MessageObj) && MessageObj)
                    {
                        if ((*MessageObj)->TryGetStringField(TEXT("content"), OutText))
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
};

