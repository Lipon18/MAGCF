// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MAGCFGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class MAGCF_API AMAGCFGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMAGCFGameModeBase();

    virtual void BeginPlay() override;
};
