// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/Characters/MAGCFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMAGCFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    AActor* OwningActor = GetOwningActor();
    if (auto* Character = Cast<ACharacter>(OwningActor))
    {
        FVector Velocity = Character->GetVelocity();
        GroundSpeed = Velocity.Size2D();
        bIsMoving = (GroundSpeed > 3.0f);
    }
}
