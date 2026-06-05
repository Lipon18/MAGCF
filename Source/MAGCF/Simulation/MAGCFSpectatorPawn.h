// /*===============================================================================
//
//
// MAGCF - Multi-Agent Generative Character Framework
//
// Copyright (c) 2026 Lipon / Psycho Games.
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
#include "GameFramework/SpectatorPawn.h"
#include "MAGCFSpectatorPawn.generated.h"

class UCameraComponent;

UCLASS()
class MAGCF_API AMAGCFSpectatorPawn : public ASpectatorPawn
{
    GENERATED_BODY()

public:
    AMAGCFSpectatorPawn();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Spectator")
    TObjectPtr<UCameraComponent> CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MAGCF|Spectator")
    float FastMoveSpeedMultiplier = 2.5f;
};
