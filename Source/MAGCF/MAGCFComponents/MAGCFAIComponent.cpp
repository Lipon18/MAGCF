// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/MAGCFComponents/MAGCFAIComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"
#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GameFramework/Actor.h"

UMAGCFAIComponent::UMAGCFAIComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMAGCFAIComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMAGCFAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeSinceLastDecision += DeltaTime;
    if (TimeSinceLastDecision >= DecisionInterval && !bIsThinking)
    {
        TimeSinceLastDecision = 0.0f;
        RequestNextAction();
    }
}

FString UMAGCFAIComponent::GatherCharacterContext()
{
    auto* OwnerChar = Cast<AMAGCFCharacter>(GetOwner());
    if (!OwnerChar)
    {
        return TEXT("{\"error\": \"No character context found\"}");
    }
    return OwnerChar->CompileLLMContextPayload();
}

void UMAGCFAIComponent::RequestNextAction()
{
    bIsThinking = true;
    FString StateContext = GatherCharacterContext();

    FString SystemInstructions = TEXT("You are the cognitive driver of an autonomous character in Unreal Engine. ") TEXT(
        "Analyze the input JSON object, which represents your complete live state, identity profiles, psychological data, and needs map. ")
        TEXT("Select the single best next action from the provided 'AllowedActionSchema'. ")
            TEXT("You MUST respond ONLY with a raw JSON object matching this schema exactly: ")
                TEXT("{\"Action\": \"E_EAT\" or \"E_DANCE\" or \"E_TALK_TO_PHONE\", \"Reasoning\": \"Brief explanation based on "
                     "personality and live states\"}");

    FString FullPrompt = FString::Printf(TEXT("%s\n\nInput Context Payload Data:\n%s"), *SystemInstructions, *StateContext);
    SendLLMRequest(FullPrompt);
}

void UMAGCFAIComponent::SendLLMRequest(const FString& PromptPayload)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    FString TargetURL = FString::Printf(TEXT("%s?key=%s"), *GeminiModelEndpoint, *ApiKey);

    Request->SetURL(TargetURL);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonPayload = MakeShareable(new FJsonObject());
    TSharedPtr<FJsonObject> ContentsObj = MakeShareable(new FJsonObject());
    TSharedPtr<FJsonObject> PartsObj = MakeShareable(new FJsonObject());

    PartsObj->SetStringField(TEXT("text"), PromptPayload);

    TArray<TSharedPtr<FJsonValue>> PartsArray;
    PartsArray.Add(MakeShareable(new FJsonValueObject(PartsObj)));
    ContentsObj->SetArrayField(TEXT("parts"), PartsArray);

    TArray<TSharedPtr<FJsonValue>> ContentsArray;
    ContentsArray.Add(MakeShareable(new FJsonValueObject(ContentsObj)));
    JsonPayload->SetArrayField(TEXT("contents"), ContentsArray);

    FString SerializedBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&SerializedBody);
    if (FJsonSerializer::Serialize(JsonPayload.ToSharedRef(), Writer))
    {
        Request->SetContentAsString(SerializedBody);
        Request->OnProcessRequestComplete().BindUObject(this, &UMAGCFAIComponent::OnLLMResponseReceived);
        Request->ProcessRequest();
    }
    else
    {
        bIsThinking = false;
    }
}

void UMAGCFAIComponent::OnLLMResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    bIsThinking = false;

    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[LLM] Connection to Google AI Studio failed."));
        return;
    }

    TSharedPtr<FJsonObject> RootJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

    if (FJsonSerializer::Deserialize(Reader, RootJson) && RootJson.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* Candidates;
        if (RootJson->TryGetArrayField(TEXT("candidates"), Candidates) && Candidates->Num() > 0)
        {
            TSharedPtr<FJsonObject> CandidateObj = (*Candidates)[0]->AsObject();
            if (CandidateObj.IsValid())
            {
                TSharedPtr<FJsonObject> ContentObj = CandidateObj->GetObjectField(TEXT("content"));
                if (ContentObj.IsValid())
                {
                    const TArray<TSharedPtr<FJsonValue>>* Parts;
                    if (ContentObj->TryGetArrayField(TEXT("parts"), Parts) && Parts->Num() > 0)
                    {
                        FString RawLLMOutput = (*Parts)[0]->AsObject()->GetStringField(TEXT("text"));
                        ProcessStructuredOutput(RawLLMOutput);
                    }
                }
            }
        }
    }
}

void UMAGCFAIComponent::ProcessStructuredOutput(const FString& JsonResponseString)
{
    FString CleanedString = JsonResponseString;
    CleanedString.ReplaceInline(TEXT("```json"), TEXT(""));
    CleanedString.ReplaceInline(TEXT("```"), TEXT(""));
    CleanedString = CleanedString.TrimStartAndEnd();

    TSharedPtr<FJsonObject> ActionJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CleanedString);

    if (FJsonSerializer::Deserialize(Reader, ActionJson) && ActionJson.IsValid())
    {
        FString ActionField = ActionJson->GetStringField(TEXT("Action"));
        FString ReasoningField = ActionJson->GetStringField(TEXT("Reasoning"));
        UE_LOG(LogTemp, Log, TEXT("[LLM Selection] Action decided: %s. Reason: %s"), *ActionField, *ReasoningField);
        OnLLMActionReceived.Broadcast(ActionField, ReasoningField);
    }
}
