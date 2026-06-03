// /*===============================================================================
//
//
// MAGCF - Multi-Agent Generative Character Framework
//
// Copyright (c) 2026 Your Lipon / Psycho Games.
//
// All Rights Reserved.
//
// MAGCF is an experimental research framework for autonomous AI-driven characters and multi-agent simulation within Unreal Engine
// environments.
//
// Unauthorized copying, modification, distribution, or use of this software
//
// without explicit permission is prohibited.
//
//
// ===============================================================================*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAGCF/AI/Profiles/MAGCFProfiles.h"
#include "MAGCFNeedComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGCF_API UMAGCFNeedComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMAGCFNeedComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    FNPCNeedState* GetNeed(FName NeedName);
    void SatisfyNeed(FName NeedName, float Amount);
    bool CheckIfAnyNeedIsCritical(FName& OutCriticalNeedName);
    FString GetNeedsAsJSONString() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Needs")
    TMap<FName, FNPCNeedState> TrackedNeeds;
};
