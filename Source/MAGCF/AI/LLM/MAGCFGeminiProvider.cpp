// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/LLM/MAGCFGeminiProvider.h"
#include "Interfaces/IHttpRequest.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FString FMAGCFGeminiProvider::GetRequestURL(const FString& ModelName, const FString& CustomEndpoint) const
{
    return FString::Printf(TEXT("https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent"), *ModelName);
}

void FMAGCFGeminiProvider::ConfigureHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request, const FString& ApiKey) const
{
    FString CurrentURL = Request->GetURL();
    Request->SetURL(FString::Printf(TEXT("%s?key=%s"), *CurrentURL, *ApiKey));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
}

TSharedPtr<FJsonObject> FMAGCFGeminiProvider::BuildPayload(const FString& Model, const FString& Prompt) const
{
    TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
    TSharedPtr<FJsonObject> ContentObj = MakeShared<FJsonObject>();
    TSharedPtr<FJsonObject> PartObj = MakeShared<FJsonObject>();

    PartObj->SetStringField(TEXT("text"), Prompt);

    TArray<TSharedPtr<FJsonValue>> PartsArray;
    PartsArray.Add(MakeShared<FJsonValueObject>(PartObj));
    ContentObj->SetArrayField(TEXT("parts"), PartsArray);

    TArray<TSharedPtr<FJsonValue>> ContentsArray;
    ContentsArray.Add(MakeShared<FJsonValueObject>(ContentObj));
    Root->SetArrayField(TEXT("contents"), ContentsArray);

    return Root;
}

bool FMAGCFGeminiProvider::ExtractResponseText(const FString& ResponseBody, FString& OutText) const
{
    TSharedPtr<FJsonObject> RootJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);

    if (!FJsonSerializer::Deserialize(Reader, RootJson) || !RootJson.IsValid()) return false;

    const TArray<TSharedPtr<FJsonValue>>* Candidates = nullptr;
    if (RootJson->TryGetArrayField(TEXT("candidates"), Candidates) && Candidates && Candidates->Num() > 0)
    {
        const TSharedPtr<FJsonObject>* CandidateObj = nullptr;
        if ((*Candidates)[0]->TryGetObject(CandidateObj) && CandidateObj)
        {
            const TSharedPtr<FJsonObject>* ContentObj = nullptr;
            if ((*CandidateObj)->TryGetObjectField(TEXT("content"), ContentObj) && ContentObj)
            {
                const TArray<TSharedPtr<FJsonValue>>* Parts = nullptr;
                if ((*ContentObj)->TryGetArrayField(TEXT("parts"), Parts) && Parts && Parts->Num() > 0)
                {
                    const TSharedPtr<FJsonObject>* PartObj = nullptr;
                    if ((*Parts)[0]->TryGetObject(PartObj) && PartObj)
                    {
                        return (*PartObj)->TryGetStringField(TEXT("text"), OutText);
                    }
                }
            }
        }
    }
    return false;
}