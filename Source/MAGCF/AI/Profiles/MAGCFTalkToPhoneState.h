// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "MAGCF/AI/Profiles/MAGCFActionStateBase.h"
#include "MAGCFTalkToPhoneState.generated.h"

UCLASS()
class MAGCF_API UMAGCFTalkToPhoneState : public UMAGCFActionStateBase
{
    GENERATED_BODY()

public:
    virtual void OnEnter() override;
    virtual void OnTick(float DeltaTime) override;
    virtual void OnExit() override;
    virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

private:
    // How long the call has been going — ends the call after a duration
    float CallElapsedTime = 0.0f;

    UPROPERTY()
    float CallDuration = 15.0f;
};
