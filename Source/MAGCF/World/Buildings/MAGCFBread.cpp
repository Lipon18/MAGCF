// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/World/Buildings/MAGCFBread.h"
#include "Components/StaticMeshComponent.h"

AMAGCFBread::AMAGCFBread()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
    RootComponent = SceneRootComponent;

    BreadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BreadMeshComponent"));
    BreadMeshComponent->SetupAttachment(RootComponent);
    BreadMeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}