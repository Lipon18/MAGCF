// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tickable.h"
#include "MAGCFDebugSubsystem.generated.h"

UCLASS()
class MAGCF_API UMAGCFDebugSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
    virtual bool IsTickable() const override { return bIsDebugHUDActive; }
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UMAGCFDebugSubsystem, STATGROUP_Tickables); }

private:
    void ToggleGlobalDebug();
    void PrintCharacterStatus(const TArray<FString>& Args);
    void DrawDebugOverlay();

    bool bIsDebugHUDActive = false;
    float FilterExpiryTime = 0.0f;
    FString FilteredCharacterName = TEXT("");

    IConsoleCommand* ToggleCommandHandle = nullptr;
    IConsoleCommand* StatusCommandHandle = nullptr;
};
