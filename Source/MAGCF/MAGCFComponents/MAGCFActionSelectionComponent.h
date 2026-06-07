// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAGCF/Enums/EGoals.h"
#include "MAGCFActionSelectionComponent.generated.h"

class AMAGCFCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGCF_API UMAGCFActionSelectionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMAGCFActionSelectionComponent();

    UFUNCTION(BlueprintCallable, Category = "MAGCF|SelectionComponent")
    TArray<FMAGCFCandidateAction> GenerateCandidateActions(int32 MaxCandidates = 3);

private:
    float CalculateBaseUtility(EMAGCFGoal Goal, const AMAGCFCharacter* OwnerChar) const;

    UPROPERTY()
    AMAGCFCharacter* CachedCharacter = nullptr;
};