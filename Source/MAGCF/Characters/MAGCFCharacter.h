// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MAGCF/Enums/EGoals.h"
#include "MAGCF/AI/Profiles/MAGCFPersonalityDataAsset.h"
#include "MAGCFCharacter.generated.h"

class AMAGCFBakery;
class UMAGCFNeedComponent;
class UAnimMontage;
class UMAGCFAIComponent;

UCLASS()
class MAGCF_API AMAGCFCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMAGCFCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE void SetIsAtBakery(bool bAtShop) { bIsAtBakery = bAtShop; }
    void ForceExecuteGoal() { ExecuteGoal(); }
    void StartEatingSequence();

    FString CompileLLMContextPayload() const;

    FORCEINLINE EMAGCFGoal GetCurrentGoal() const { return CurrentGoal; }
    FORCEINLINE void SetCurrentGoal(EMAGCFGoal NewGoal) { CurrentGoal = NewGoal; }
    FORCEINLINE void SetIsAtBench(bool bAtBench) { bIsAtBench = bAtBench; }
    FORCEINLINE UMAGCFNeedComponent* GetNeedComponent() const { return NeedComponent; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF|AI|Configuration")
    UMAGCFPersonalityDataAsset* PersonalityConfig = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|AI|Components")
    UMAGCFNeedComponent* NeedComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|AI|Components")
    UMAGCFAIComponent* LLMComponent = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|AI|Animations")
    TObjectPtr<UAnimMontage> EatMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|AI|Animations")
    TObjectPtr<UAnimMontage> DanceMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|AI|Animations")
    TObjectPtr<UAnimMontage> PhoneMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|AI|Animations")
    TObjectPtr<UAnimMontage> GroundSitMontage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF|AI|Configuration")
    FName HandSocketName = TEXT("Hand_R_Socket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF|AI|Configuration")
    TSubclassOf<AActor> BreadClass;

    UPROPERTY(BlueprintReadOnly, Category = "MAGCF|AI")
    EMAGCFGoal CurrentGoal = EMAGCFGoal::E_NONE;

    UPROPERTY(BlueprintReadOnly, Category = "MAGCF|AI")
    EMAGCFMood CurrentMood = EMAGCFMood::E_NEUTRAL;

    UPROPERTY(EditAnywhere, Category = "MAGCF|World")
    AMAGCFBakery* TargetBakery = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|Economy")
    float Money = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|AI")
    bool bHasBread = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|AI")
    bool bIsAtBakery = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|AI")
    bool bIsAtBench = false;

    void EvaluateNeed();
    void UpdateCurrentMoodState();
    void ExecuteGoal();

    void HandleEatGoal();
    void HandleDanceGoal();
    void HandleSitOnGroundGoal();
    void HandleTalkToPhoneGoal();

    UFUNCTION()
    void HandleLLMActionCallback(FString Action, FString TargetDetails);

private:
    UPROPERTY()
    AActor* SpawnedBreadActor = nullptr;

    bool bIsPlayingActionMontage = false;

    TMap<EMAGCFGoal, TFunction<void()>> GoalExecutionMap;
    TMap<FString, EMAGCFGoal> StringToGoalMap;

    void InitializeExecutionRegistries();
    bool InternalPlayActionMontage(UAnimMontage* TargetMontage, FName EndWorkerFunctionName);
    void HaltCharacterVelocity();

   UFUNCTION()
   void OnEatMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION()
    void OnActionMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
