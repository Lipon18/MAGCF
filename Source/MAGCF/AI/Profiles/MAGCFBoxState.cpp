// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.
#include "MAGCF/AI/Profiles/MAGCFBoxState.h"
#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"

void UMAGCFBoxState::OnEnter()
{
    if (!ActionComp || !OwnerCharacter) return;

    bPlayingJab1 = true;
    ComboCount = 0;
    ActionComp->HaltCharacterVelocity();

    // Start with jab1
    ActionComp->InternalPlayActionMontage(ActionComp->GetBoxingJab1Montage());
}

void UMAGCFBoxState::OnTick(float DeltaTime)
{
    if (!ActionComp || !OwnerCharacter) return;

    auto* NeedComp = OwnerCharacter->GetNeedComponent();
    if (!NeedComp) return;

    // Boxing drains energy fairly fast
    auto* EnergyNeed = NeedComp->GetNeed(TEXT("Energy"));
    if (EnergyNeed)
    {
        EnergyNeed->CurrentValue = FMath::Clamp(EnergyNeed->CurrentValue - (4.0f * DeltaTime), 0.0f, EnergyNeed->MaxValue);

        // Stop boxing if energy drops below the prerequisite threshold
        if (EnergyNeed->CurrentValue < 20.0f)
        {
            ActionComp->ClearCurrentAction();
        }
    }

    // Boxing also reduces Fun (it's engaging)
    NeedComp->SatisfyNeed(TEXT("Fun"), 2.0f * DeltaTime);
}

void UMAGCFBoxState::OnExit()
{
    if (!ActionComp || !OwnerCharacter) return;

    // Stop whichever montage is currently playing
    if (auto* AnimInstance = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr)
    {
        AnimInstance->Montage_Stop(0.25f, bPlayingJab1
            ? ActionComp->GetBoxingJab1Montage()
            : ActionComp->GetBoxingJab2Montage());
    }
    ActionComp->SetIsPlayingActionMontage(false);
    ActionComp->RestoreCharacterMovement();
}

void UMAGCFBoxState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted || !ActionComp) return;

    ComboCount++;

    // After MaxCombos rounds of alternating jabs, finish the boxing session
    if (ComboCount >= MaxCombos)
    {
        ActionComp->ClearCurrentAction();
        return;
    }

    // Alternate between jab1 and jab2 for combo feel
    bPlayingJab1 = !bPlayingJab1;
    UAnimMontage* NextMontage = bPlayingJab1
        ? ActionComp->GetBoxingJab1Montage()
        : ActionComp->GetBoxingJab2Montage();

    ActionComp->InternalPlayActionMontage(NextMontage);
}
