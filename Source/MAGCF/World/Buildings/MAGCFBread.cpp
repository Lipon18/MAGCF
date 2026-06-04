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