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

#include "MAGCF/Characters/MAGCFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMAGCFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    AActor* OwningActor = GetOwningActor();
    if (ACharacter* Character = Cast<ACharacter>(OwningActor))
    {
        FVector Velocity = Character->GetVelocity();
        GroundSpeed = Velocity.Size2D();
        bIsMoving = (GroundSpeed > 3.0f);
    }
}
