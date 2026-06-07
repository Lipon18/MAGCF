// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.
#include "MAGCF/AI/Profiles/MAGCFTalkToPhoneState.h"
#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"

void UMAGCFTalkToPhoneState::OnEnter()
{
    if (!ActionComp || !OwnerCharacter) return;

    CallElapsedTime = 0.0f;
    ActionComp->HaltCharacterVelocity();
    ActionComp->InternalPlayActionMontage(ActionComp->GetPhoneMontage());
}

void UMAGCFTalkToPhoneState::OnTick(float DeltaTime)
{
    if (!ActionComp || !OwnerCharacter) return;

    CallElapsedTime += DeltaTime;

    auto* NeedComp = OwnerCharacter->GetNeedComponent();
    if (NeedComp)
    {
        // Talking to a friend reduces boredom (Fun drains up so we satisfy it down)
        NeedComp->SatisfyNeed(TEXT("Fun"), 5.0f * DeltaTime);
    }

    // End the call after CallDuration seconds
    if (CallElapsedTime >= CallDuration)
    {
        ActionComp->ClearCurrentAction();
    }
}

void UMAGCFTalkToPhoneState::OnExit()
{
    if (!ActionComp || !OwnerCharacter) return;

    if (auto* AnimInstance = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr)
    {
        AnimInstance->Montage_Stop(0.25f, ActionComp->GetPhoneMontage());
    }
    ActionComp->SetIsPlayingActionMontage(false);
    ActionComp->RestoreCharacterMovement();
}

void UMAGCFTalkToPhoneState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // Phone montage may loop — OnTick timer handles the actual exit
    // If montage ends without interrupt and timer hasn't fired yet, just clear
    if (!bInterrupted && ActionComp)
    {
        ActionComp->ClearCurrentAction();
    }
}
