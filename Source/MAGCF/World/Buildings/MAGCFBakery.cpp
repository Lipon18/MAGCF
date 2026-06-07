// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.

#include "MAGCFBakery.h"
#include "Components/StaticMeshComponent.h"

AMAGCFBakery::AMAGCFBakery() 
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
    RootComponent = SceneRootComponent;

    BakeryMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BakeryMeshComponent"));
    BakeryMeshComponent->SetupAttachment(RootComponent);
}
