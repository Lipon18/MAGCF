// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MAGCFBakery.generated.h"

class UStaticMeshComponent;

UCLASS()
class MAGCF_API AMAGCFBakery : public AActor
{
    GENERATED_BODY()

public:
    AMAGCFBakery();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Economy")
    float BreadPrice = 5.0f;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    TObjectPtr<USceneComponent> SceneRootComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MAGCF|Components")
    TObjectPtr<UStaticMeshComponent> BakeryMeshComponent;
};
