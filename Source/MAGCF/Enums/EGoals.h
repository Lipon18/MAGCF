// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EGoals.generated.h"

UENUM(BlueprintType)
enum class EMAGCFGoal : uint8
{
	E_NONE,
    E_EAT,
    E_DANCE,
    E_SIT_ON_GROUND,
    E_TALK_TO_PHONE,
    E_BOX,
    E_PUSH_UP
};

UENUM(BlueprintType)
enum class EMAGCFMood : uint8
{
    E_NEUTRAL,
    E_HAPPY,
    E_TIRED,
    E_BORED,
    E_IRRITATED
};

USTRUCT(BlueprintType)
struct FNPCNeedState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName NeedName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxValue = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DrainRate = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalThreshold = 75.0f;
};

USTRUCT(BlueprintType)
struct FMAGCFCandidateAction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMAGCFGoal Goal = EMAGCFGoal::E_NONE;

    UPROPERTY(BlueprintReadOnly)
    float FinalScore = 0.0f;
};
