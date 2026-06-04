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
#include "AIController.h"
#include "MAGCFAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class AMAGCFBakery;

UCLASS()
class MAGCF_API AMAGCFAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMAGCFAIController();

    virtual void BeginPlay() override;

    AMAGCFBakery* GetKnownBakeryLocation() const { return CachedBakeryTarget; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Perception")
    TObjectPtr<UAIPerceptionComponent> PerceptionComp;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
    UPROPERTY()
    AMAGCFBakery* CachedBakeryTarget = nullptr;
};
