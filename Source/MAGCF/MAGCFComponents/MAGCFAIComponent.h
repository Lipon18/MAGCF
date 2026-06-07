// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAGCF/Enums/EGoals.h"
#include "MAGCF/AI/LLM/MAGCFLLMService.h"
#include "MAGCFAIComponent.generated.h"

class UMAGCFLLMService;
struct FMAGCFCandidateAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLLMActionChoiceSignature, EMAGCFGoal, ChosenGoal, const FString&, Reasoning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLLMActionStringSignature, const FString&, ActionName, const FString&, Reasoning);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGCF_API UMAGCFAIComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMAGCFAIComponent();

    FString CompileLLMContextPayload() const;

    UPROPERTY(BlueprintAssignable, Category = "MAGCF|AI")
    FOnLLMActionChoiceSignature OnActionSelected;

    UPROPERTY(BlueprintAssignable, Category = "MAGCF|AI")
    FOnLLMActionStringSignature OnLLMActionReceived;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MAGCF|AI", meta = (ClampMin = "5.0"))
    float DecisionInterval = 20.0f;

    FORCEINLINE float GetTimeSinceLastDecision() const { return TimeSinceLastDecision; }
    FORCEINLINE bool GetIsThinking() const { return bIsThinking; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void RequestCandidateSelection();
    void RequestNextAction();

    FString FormatPayload(const TArray<FMAGCFCandidateAction>& Candidates) const;
    FString GatherCharacterContext() const;

    void OnLLMSuccess(const FString& CleanResponseText);
    void OnLLMFailure(const FString& ErrorMessage);
    void ProcessStructuredOutput(const FString& JsonResponseString);

    FORCEINLINE bool IsActionAllowed(const FString& ActionField) const { return !ActionField.IsEmpty(); }

private:
    UPROPERTY()
    UMAGCFLLMService* LLMService = nullptr;

    UPROPERTY(EditAnywhere, Category = "MAGCF|AI")
    FString InstanceApiKeyOverride;

    FString SystemInstructions;
    FString LastAction;

    bool bIsThinking = false;
    float TimeSinceLastDecision = 0.0f;
    float LLMRetryBlockUntil = 0.0f;
    float LastActionTimestamp = 0.0f;
};