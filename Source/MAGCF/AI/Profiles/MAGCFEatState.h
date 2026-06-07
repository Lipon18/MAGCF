// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "MAGCF/AI/Profiles/MAGCFActionStateBase.h"
#include "MAGCFEatState.generated.h"

UCLASS()
class MAGCF_API UMAGCFEatState : public UMAGCFActionStateBase
{
    GENERATED_BODY()

public:
    virtual void OnEnter() override;
    virtual void OnTick(float DeltaTime) override;
    virtual void OnExit() override;
    virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

private:
    // Bread prop spawned into the hand socket during eating
    void SpawnBreadProp();
    void DestroyBreadProp();
};
