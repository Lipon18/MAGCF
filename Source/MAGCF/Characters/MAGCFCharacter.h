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
#include "MAGCF/AI/Profiles/MAGCFPersonalityDataAsset.h"
#include "MAGCFCharacter.generated.h"

class AMAGCFBakery;
class UMAGCFNeedComponent;

UCLASS()
class MAGCF_API AMAGCFCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMAGCFCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF|AI|Configuration")
    UMAGCFPersonalityDataAsset* PersonalityConfig = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Components")
    UMAGCFNeedComponent* NeedComponent = nullptr;

    UPROPERTY()
    EMAGCFGoal CurrentGoal = EMAGCFGoal::E_NONE;

    UPROPERTY(EditAnywhere, Category = "MAGCF|World")
    AMAGCFBakery* TargetBakery = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|Economy")
    float Money = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|AI")
    bool bHasBread = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|AI")
    bool bIsAtBakery = false;

    void EvaluateNeed();
    void ExecuteGoal();
    void HandleEatGoal();

    FString CompileLLMContextPayload() const;
};
