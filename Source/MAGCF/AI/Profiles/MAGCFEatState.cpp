// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/Profiles/MAGCFEatState.h"
#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"
#include "Engine/World.h"

void UMAGCFEatState::OnEnter()
{
    if (!ActionComp || !OwnerCharacter) return;

    ActionComp->HaltCharacterVelocity();
    SpawnBreadProp();
    ActionComp->InternalPlayActionMontage(ActionComp->GetEatMontage());
}

void UMAGCFEatState::OnTick(float DeltaTime) {}

void UMAGCFEatState::OnExit()
{
    if (!ActionComp || !OwnerCharacter) return;

    DestroyBreadProp();

    if (auto* AnimInstance = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr)
    {
        AnimInstance->Montage_Stop(0.25f, ActionComp->GetEatMontage());
    }
    ActionComp->SetIsPlayingActionMontage(false);
    ActionComp->RestoreCharacterMovement();
}

void UMAGCFEatState::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted) return;
    if (OwnerCharacter)
    {
        if (auto* NeedComp = OwnerCharacter->GetNeedComponent())
        {
            NeedComp->SatisfyNeed(TEXT("Hunger"), 40.0f);
        }
        float NewMoney = FMath::Max(0.0f, OwnerCharacter->GetMoney() - 5.0f);
        OwnerCharacter->SetMoney(NewMoney);
        OwnerCharacter->SetHasBread(false);
    }
    if (ActionComp)
    {
        ActionComp->ClearCurrentAction();
    }
}

void UMAGCFEatState::SpawnBreadProp()
{
    if (!ActionComp || !OwnerCharacter) return;
    if (!ActionComp->GetBreadClass()) return;

    UWorld* World = OwnerCharacter->GetWorld();
    if (!World) return;

    FActorSpawnParameters Params;
    Params.Owner = OwnerCharacter;

    AActor* Bread = World->SpawnActor<AActor>(ActionComp->GetBreadClass(), FTransform::Identity, Params);
    if (Bread)
    {
        FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
        Bread->AttachToComponent(OwnerCharacter->GetMesh(), Rules, ActionComp->GetHandSocketName());
        ActionComp->SetSpawnedBreadActor(Bread);
        OwnerCharacter->SetHasBread(true);
    }
}

void UMAGCFEatState::DestroyBreadProp()
{
    if (!ActionComp) return;

    if (AActor* Bread = ActionComp->GetSpawnedBreadActor())
    {
        Bread->Destroy();
        ActionComp->SetSpawnedBreadActor(nullptr);
    }
}
