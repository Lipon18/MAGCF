// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MAGCF/Enums/EGoals.h"
#include "MAGCFCharacter.generated.h"

class AMAGCFBakery;
class UMAGCFNeedComponent;
class UMAGCFActionComponent;
class UMAGCFAIComponent;
class UMAGCFActionSelectionComponent;
class UMAGCFPersonalityDataAsset;

UCLASS()
class MAGCF_API AMAGCFCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMAGCFCharacter();

    void ExecuteGoal();

    UFUNCTION(BlueprintCallable, Category = "MAGCF|State")
    bool IsBusy() const;

    FORCEINLINE bool HasBread() const { return bHasBread; }
    FORCEINLINE bool IsAtBakery() const { return bIsAtBakery; }
    FORCEINLINE float GetMoney() const { return Money; }
    FORCEINLINE void SetHasBread(const bool bState) { bHasBread = bState; }
    FORCEINLINE void SetMoney(const float NewAmount) { Money = NewAmount; }
    FORCEINLINE void SetIsAtBakery(const bool bAtShop) { bIsAtBakery = bAtShop; }
    FORCEINLINE void SetIsAtBench(const bool bAtBench) { bIsAtBench = bAtBench; }
    FORCEINLINE void SetCurrentGoal(EMAGCFGoal NewGoal) { CurrentGoal = NewGoal; }
    FORCEINLINE AMAGCFBakery* GetTargetBakery() const { return TargetBakery; }
    FORCEINLINE EMAGCFGoal GetCurrentGoal() const { return CurrentGoal; }
    FORCEINLINE EMAGCFMood GetCurrentMood() const { return CurrentMood; }

    FORCEINLINE UMAGCFNeedComponent* GetNeedComponent() const { return NeedComponent; }
    FORCEINLINE UMAGCFPersonalityDataAsset* GetPersonalityConfig() const { return PersonalityConfig; }
    FORCEINLINE UMAGCFAIComponent* GetAIComponent() const { return LLMComponent; }
    FORCEINLINE UMAGCFActionSelectionComponent* GetActionSelectionComponent() const { return ActionSelectionComponent; }

    int32 GetRecentActionOccurrenceCount(const EMAGCFGoal TargetGoal) const;
    FString GetCharacterArchetypeName() const;
    FString GetGoalEnumToString(const EMAGCFGoal Goal) const;
    EMAGCFGoal GetGoalStringToEnum(const FString& GoalStr) const;
    const TArray<EMAGCFGoal>& GetRecentActionsArray() const { return RecentActions; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void HandleActionDecisionReceived(EMAGCFGoal ChosenGoal, const FString& Reasoning);

    UFUNCTION()
    void HandleLLMActionCallback(const FString& Action, const FString& TargetDetails);

    void UpdateDerivedMoodState();
    void ProcessHistoryBuffer(const EMAGCFGoal NewAction);
    void UpdateCooldowns(const float DeltaTime);
    void EvaluateNeed();
    void HandleLLMFailure();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF|Configuration")
    UMAGCFPersonalityDataAsset* PersonalityConfig = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    UMAGCFNeedComponent* NeedComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    UMAGCFAIComponent* LLMComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    UMAGCFActionComponent* ActionComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    UMAGCFActionSelectionComponent* ActionSelectionComponent = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "MAGCF")
    EMAGCFGoal CurrentGoal = EMAGCFGoal::E_NONE;

    UPROPERTY(BlueprintReadOnly, Category = "MAGCF")
    EMAGCFMood CurrentMood = EMAGCFMood::E_NEUTRAL;

    UPROPERTY(EditAnywhere, Category = "MAGCF")
    AMAGCFBakery* TargetBakery = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF")
    float Money = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF")
    bool bHasBread = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF")
    bool bIsAtBakery = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF")
    bool bIsAtBench = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF", meta = (ClampMin = "1"))
    int32 MaxHistorySize = 10;

private:
    TMap<FString, EMAGCFGoal> StringToGoalMap;
    TMap<EMAGCFGoal, FString> GoalToStringMap;
    TMap<EMAGCFGoal, float> CooldownMap;
    TArray<EMAGCFGoal> RecentActions;

    void InitializeLookups();
};