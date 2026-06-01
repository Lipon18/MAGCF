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
#include "GameFramework/Character.h"
#include "MAGCF/Enums/EGoals.h"
#include "MAGCFCharacter.generated.h"

class AMAGCFBakery;

UCLASS()
class MAGCF_API AMAGCFCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMAGCFCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY()
    EMAGCFGoal CurrentGoal = EMAGCFGoal::E_NONE;

    UPROPERTY(EditAnywhere, Category = "World")
    AMAGCFBakery* TargetBakery = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Needs")
    float Hunger = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Needs")
    float MaxHunger = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
    float Money = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool bHasBread = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool bIsAtBakery = false;

    void EvaluateNeed();
    void ExecuteGoal();
    void HandleEatGoal();
};
