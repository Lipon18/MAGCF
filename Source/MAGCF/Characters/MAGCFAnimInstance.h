// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

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
