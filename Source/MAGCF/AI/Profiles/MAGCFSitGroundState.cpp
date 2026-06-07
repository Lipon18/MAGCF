// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/Profiles/MAGCFSitGroundState.h"
#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"

void UMAGCFSitGroundState::OnEnter()
{
    if (!ActionComp || !OwnerCharacter) return;

    ActionComp->HaltCharacterVelocity();
    ActionComp->InternalPlayActionMontage(ActionComp->GetGroundSitMontage());
}

void UMAGCFSitGroundState::OnTick(float DeltaTime)
{
    if (!ActionComp || !OwnerCharacter) return;

    auto* NeedComp = OwnerCharacter->GetNeedComponent();
    if (!NeedComp) return;
    NeedComp->RestoreNeed(TEXT("Energy"), ActionComp->GetGroundRestEnergyRecoveryRate() * DeltaTime);
    auto* EnergyNeed = NeedComp->GetNeed(TEXT("Energy"));
    if (EnergyNeed && EnergyNeed->CurrentValue >= 60.0f)
    {
        ActionComp->ClearCurrentAction();
    }
}

void UMAGCFSitGroundState::OnExit()
{
    if (!ActionComp || !OwnerCharacter) return;
    if (auto* AnimInstance = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr)
    {
        AnimInstance->Montage_Stop(0.25f, ActionComp->GetGroundSitMontage());
    }
    ActionComp->SetIsPlayingActionMontage(false);
    ActionComp->RestoreCharacterMovement();
}

void UMAGCFSitGroundState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // Sit montage ending early (interrupted) is fine, OnTick handles the exit condition
}