// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/MAGCFComponents/MAGCFActionComponent.h"
#include "MAGCF/Characters/MAGCFCharacter.h"
#include "MAGCF/AI/Profiles/MAGCFActionStateBase.h"
#include "MAGCF/MAGCF.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

UMAGCFActionComponent::UMAGCFActionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMAGCFActionComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<AMAGCFCharacter>(GetOwner());
}

void UMAGCFActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentActionState)
    {
        CurrentActionState->OnTick(DeltaTime);
    }
}

void UMAGCFActionComponent::ExecuteGoal(EMAGCFGoal Goal)
{
    if (Goal == EMAGCFGoal::E_NONE || !OwnerCharacter) return;
    if (CurrentGoalType == Goal && CurrentActionState) return;

    const auto* const TargetStateClass = ActionStateClasses.Find(Goal);
    if (!TargetStateClass || !*TargetStateClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Goal execution dropped: No valid State Class bound for Goal Enum Index %d"), (int32)Goal);
        return;
    }

    ClearCurrentAction();

    CurrentGoalType = Goal;
    CurrentActionState = NewObject<UMAGCFActionStateBase>(this, ActionStateClasses[Goal]);

    if (CurrentActionState)
    {
        CurrentActionState->Initialize(this, OwnerCharacter);
        CurrentActionState->OnEnter();
    }
}

void UMAGCFActionComponent::ClearCurrentAction()
{
    if (CurrentActionState)
    {
        CurrentActionState->OnExit();
        CurrentActionState = nullptr;
    }

    if (OwnerCharacter && OwnerCharacter->GetCurrentGoal() == CurrentGoalType)
    {
        OwnerCharacter->SetCurrentGoal(EMAGCFGoal::E_NONE);
    }
    CurrentGoalType = EMAGCFGoal::E_NONE;
}

void UMAGCFActionComponent::CancelCurrentAction()
{
    ClearCurrentAction();

    if (OwnerCharacter && bIsPlayingActionMontage)
    {
        const auto* const MeshComp = OwnerCharacter->GetMesh();
        auto* const AnimInstance = MeshComp ? MeshComp->GetAnimInstance() : nullptr;
        if (AnimInstance)
        {
            AnimInstance->Montage_Stop(0.2f);
        }
    }
}

bool UMAGCFActionComponent::InternalPlayActionMontage(UAnimMontage* const TargetMontage, const FName StartSection)
{
    if (bIsPlayingActionMontage || !TargetMontage || !OwnerCharacter) return false;

    const auto* const MeshComp = OwnerCharacter->GetMesh();
    auto* const AnimInstance = MeshComp ? MeshComp->GetAnimInstance() : nullptr;

    if (AnimInstance)
    {
        bIsPlayingActionMontage = true;
        AnimInstance->Montage_Play(TargetMontage);

        if (StartSection != NAME_None && TargetMontage->IsValidSectionName(StartSection))
        {
            AnimInstance->Montage_JumpToSection(StartSection, TargetMontage);
        }
        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &UMAGCFActionComponent::OnCurrentStateMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, TargetMontage);
        return true;
    }
    return false;
}

void UMAGCFActionComponent::OnCurrentStateMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsPlayingActionMontage = false;
    if (CurrentActionState)
    {
        CurrentActionState->OnMontageEnded(Montage, bInterrupted);
    }
}

void UMAGCFActionComponent::HaltCharacterVelocity()
{
    if (!OwnerCharacter) return;

    auto* const AI_Controller = Cast<AAIController>(OwnerCharacter->GetController());
    if (AI_Controller)
    {
        AI_Controller->StopMovement();
    }

    auto* const MoveComp = OwnerCharacter->GetCharacterMovement();
    if (MoveComp)
    {
        MoveComp->DisableMovement();
        MoveComp->StopMovementImmediately();
    }
}

void UMAGCFActionComponent::RestoreCharacterMovement()
{
    if (OwnerCharacter)
    {
        auto* const MoveComp = OwnerCharacter->GetCharacterMovement();
        if (MoveComp)
        {
            MoveComp->SetMovementMode(MOVE_Walking);
        }
    }
}