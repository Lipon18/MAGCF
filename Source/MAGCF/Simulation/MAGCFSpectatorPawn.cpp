// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/Simulation/MAGCFSpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpectatorPawnMovement.h"

AMAGCFSpectatorPawn::AMAGCFSpectatorPawn()
{
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("SpectatorCameraComponent"));
    CameraComp->SetupAttachment(GetRootComponent());
    CameraComp->bUsePawnControlRotation = true;

    if (auto* SpecMovement = Cast<USpectatorPawnMovement>(GetMovementComponent()))
    {
        SpecMovement->MaxSpeed = 1200.f;
        SpecMovement->Acceleration = 4000.f;
        SpecMovement->Deceleration = 4000.f;
    }

    if (GetCollisionComponent())
    {
        GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AMAGCFSpectatorPawn::BeginPlay()
{
    Super::BeginPlay();
}

