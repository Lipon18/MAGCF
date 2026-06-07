// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAGCF/Enums/EGoals.h"
#include "MAGCFNeedComponent.generated.h"

class UMAGCFPersonalityDataAsset;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGCF_API UMAGCFNeedComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMAGCFNeedComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void InitializeNeedsFromPersonality(const UMAGCFPersonalityDataAsset* Personality);
    void SatisfyNeed(FName NeedName, float Amount);
    void RestoreNeed(FName NeedName, float Amount);

    FNPCNeedState* GetNeed(FName NeedName);
    const FNPCNeedState* GetNeed(FName NeedName) const;

    bool CheckIfAnyNeedIsCritical(FName& OutCriticalNeedName);
    FString GetNeedsAsJSONString() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MAGCF|Needs")
    TMap<FName, FNPCNeedState> TrackedNeeds;

private:
    FORCEINLINE bool IsInverseNeed(const FName NeedName) const { return NeedName == TEXT("Energy"); }
    FORCEINLINE bool IsNeedCritical(const FName NeedName, const FNPCNeedState& Need) const
    {
        return IsInverseNeed(NeedName) ? (Need.CurrentValue <= Need.CriticalThreshold) : (Need.CurrentValue >= Need.CriticalThreshold);
    }
};