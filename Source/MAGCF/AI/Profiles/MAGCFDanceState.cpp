// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.
#include "MAGCF/AI/Profiles/MAGCFDanceState.h"
#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"

void UMAGCFDanceState::OnEnter()
{
    if (!ActionComp || !OwnerCharacter) return;

    ActionComp->HaltCharacterVelocity();
    ActionComp->InternalPlayActionMontage(ActionComp->GetDanceMontage());
}

void UMAGCFDanceState::OnTick(float DeltaTime)
{
    // Dancing slowly drains energy and restores fun — driven by tick
    if (!ActionComp || !OwnerCharacter) return;

    auto* NeedComp = OwnerCharacter->GetNeedComponent();
    if (!NeedComp) return;

    // Dancing is fun so it reduces Fun need (lower = more entertained)
    NeedComp->SatisfyNeed(TEXT("Fun"), 3.0f * DeltaTime);

    // Dancing costs energy
    auto* EnergyNeed = NeedComp->GetNeed(TEXT("Energy"));
    if (EnergyNeed)
    {
        EnergyNeed->CurrentValue = FMath::Clamp(EnergyNeed->CurrentValue - (2.0f * DeltaTime), 0.0f, EnergyNeed->MaxValue);

        // Stop dancing if too tired
        if (EnergyNeed->CurrentValue < 20.0f)
        {
            ActionComp->ClearCurrentAction();
        }
    }
}

void UMAGCFDanceState::OnExit()
{
    if (!ActionComp || !OwnerCharacter) return;

    if (auto* AnimInstance = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr)
    {
        AnimInstance->Montage_Stop(0.25f, ActionComp->GetDanceMontage());
    }
    ActionComp->SetIsPlayingActionMontage(false);
    ActionComp->RestoreCharacterMovement();
}

void UMAGCFDanceState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // Dance montage ended naturally — release the state
    if (ActionComp)
    {
        ActionComp->ClearCurrentAction();
    }
}
