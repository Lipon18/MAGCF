// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MAGCFBread.generated.h"

class UStaticMeshComponent;

UCLASS()
class MAGCF_API AMAGCFBread : public AActor
{
    GENERATED_BODY()

public:
    AMAGCFBread();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    TObjectPtr<USceneComponent> SceneRootComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    TObjectPtr<UStaticMeshComponent> BreadMeshComponent;
};
