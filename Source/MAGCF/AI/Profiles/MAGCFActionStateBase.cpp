// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/AI/Profiles/MAGCFActionStateBase.h"

UMAGCFActionStateBase::UMAGCFActionStateBase() : ActionComp(nullptr), OwnerCharacter(nullptr) {}

void UMAGCFActionStateBase::Initialize(UMAGCFActionComponent* InActionComp, AMAGCFCharacter* InOwnerCharacter)
{
    ActionComp = InActionComp;
    OwnerCharacter = InOwnerCharacter;
}
