// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "MAGCFAIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLLMActionReceived, FString, Action, FString, TargetDetails);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGCF_API UMAGCFAIComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMAGCFAIComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "MAGCF | AI | LLM")
    void RequestNextAction();

    UPROPERTY(BlueprintAssignable, Category = "MAGCF | AI | LLM")
    FOnLLMActionReceived OnLLMActionReceived;

protected:
    virtual void BeginPlay() override;

private:
    float TimeSinceLastDecision = 0.0f;
    float DecisionInterval = 5.0f;
    bool bIsThinking = false;

    FString GatherCharacterContext();
    void SendLLMRequest(const FString& PromptPayload);
    void OnLLMResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void ProcessStructuredOutput(const FString& JsonResponseString);

    FString ApiKey = TEXT("GOOGLE_AI_STUDIO_API_KEY");
    FString GeminiModelEndpoint = TEXT("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent");
};
