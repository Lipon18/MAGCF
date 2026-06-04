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
#include "Animation/AnimInstance.h"
#include "MAGCFAnimInstance.generated.h"

UCLASS()
class MAGCF_API UMAGCFAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float GroundSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsMoving;
};
