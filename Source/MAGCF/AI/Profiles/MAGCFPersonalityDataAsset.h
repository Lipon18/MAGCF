// /*===============================================================================
//
//
// MAGCF - Multi-Agent Generative Character Framework
//
// Copyright (c) 2026 Your Lipon / Psycho Games.
//
// All Rights Reserved.
//
// MAGCF is an experimental research framework for autonomous AI-driven characters and multi-agent simulation within Unreal Engine
// environments.
//
// Unauthorized copying, modification, distribution, or use of this software
//
// without explicit permission is prohibited.
//
//
// ===============================================================================*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MAGCFPersonalityDataAsset.generated.h"

UCLASS()
class MAGCF_API UMAGCFPersonalityDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
    FString ArchetypeName = TEXT("Default");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality", meta = (ClampMin = 0.0f, ClampMax = "1.0"))
    float SocialTendency = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality", meta = (ClampMin = 0.0f, ClampMax = "1.0"))
    float Fortitude = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality", meta = (ClampMin = 0.0f, ClampMax = "1.0"))
    float SpendingTendency = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality", meta = (ClampMin = 0.0f, ClampMax = "1.0"))
    float Patience = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality", meta = (ClampMin = 0.0f, ClampMax = "1.0"))
    float Stability = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality | Taste")
    FString PreferredFlavor = TEXT("Savory");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality | Taste")
    float PriceThreshold = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Personality | Tags")
    TArray<FString> BehavioralFlavors;
};
