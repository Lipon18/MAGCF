// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MAGCFAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class AMAGCFBakery;

UCLASS()
class MAGCF_API AMAGCFAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMAGCFAIController();

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    FORCEINLINE AMAGCFBakery* GetKnownBakeryLocation() const { return CachedBakeryTarget; }

protected:
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Perception")
    TObjectPtr<UAIPerceptionComponent> PerceptionComp;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
    UPROPERTY()
    AMAGCFBakery* CachedBakeryTarget = nullptr;
};
