// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.
#include "MAGCF/AI/Profiles/MAGCFPushUpState.h"
#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"

void UMAGCFPushUpState::OnEnter()
{
    if (!ActionComp || !OwnerCharacter) return;

    RepCount = 0;
    ActionComp->HaltCharacterVelocity();
    ActionComp->InternalPlayActionMontage(ActionComp->GetPushUpMontage());
}

void UMAGCFPushUpState::OnTick(float DeltaTime)
{
    if (!ActionComp || !OwnerCharacter) return;

    auto* NeedComp = OwnerCharacter->GetNeedComponent();
    if (!NeedComp) return;

    // Push-ups drain energy heavily
    auto* EnergyNeed = NeedComp->GetNeed(TEXT("Energy"));
    if (EnergyNeed)
    {
        EnergyNeed->CurrentValue = FMath::Clamp(EnergyNeed->CurrentValue - (5.0f * DeltaTime), 0.0f, EnergyNeed->MaxValue);

        // Bail out if energy drops below safe threshold
        if (EnergyNeed->CurrentValue < 15.0f)
        {
            ActionComp->ClearCurrentAction();
        }
    }
}

void UMAGCFPushUpState::OnExit()
{
    if (!ActionComp || !OwnerCharacter) return;

    if (auto* AnimInstance = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr)
    {
        AnimInstance->Montage_Stop(0.25f, ActionComp->GetPushUpMontage());
    }
    ActionComp->SetIsPlayingActionMontage(false);
    ActionComp->RestoreCharacterMovement();
}

void UMAGCFPushUpState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted || !ActionComp) return;

    RepCount++;

    // Do MaxReps sets then finish
    if (RepCount >= MaxReps)
    {
        ActionComp->ClearCurrentAction();
        return;
    }

    // Loop the montage for next rep
    ActionComp->InternalPlayActionMontage(ActionComp->GetPushUpMontage());
}
