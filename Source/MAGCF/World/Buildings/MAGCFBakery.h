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
#include "GameFramework/Actor.h"
#include "MAGCFBakery.generated.h"

class UStaticMeshComponent;

UCLASS()
class MAGCF_API AMAGCFBakery : public AActor
{
    GENERATED_BODY()

public:
    AMAGCFBakery();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Economy")
    float BreadPrice = 5.0f;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    TObjectPtr<USceneComponent> SceneRootComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    TObjectPtr<UStaticMeshComponent> BakeryMeshComponent;
};
