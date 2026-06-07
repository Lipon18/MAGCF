// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MAGCFActionStateBase.generated.h"

class UMAGCFActionComponent;
class AMAGCFCharacter;
class UAnimMontage;

UCLASS(Abstract, BlueprintType, Blueprintable)
class MAGCF_API UMAGCFActionStateBase : public UObject
{
    GENERATED_BODY()

public:
    UMAGCFActionStateBase();

    virtual void OnEnter() {}
    virtual void OnTick(float DeltaTime) {}
    virtual void OnExit() {}
    virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {}
    virtual void Initialize(UMAGCFActionComponent* InActionComp, AMAGCFCharacter* InOwnerCharacter);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "MAGCF|Action State")
    UMAGCFActionComponent* ActionComp;

    UPROPERTY(BlueprintReadOnly, Category = "MAGCF|Action State")
    AMAGCFCharacter* OwnerCharacter;
};