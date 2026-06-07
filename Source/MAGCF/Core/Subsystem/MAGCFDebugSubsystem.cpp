// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#include "MAGCF/Core/Subsystem/MAGCFDebugSubsystem.h"
#include "MAGCF/Characters/MAGCFCharacter.h"
#include "MAGCF/MAGCFComponents/MAGCFAIComponent.h"
#include "MAGCF/MAGCFComponents/MAGCFNeedComponent.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void UMAGCFDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    IConsoleManager& ConsoleMan = IConsoleManager::Get();

    if (ConsoleMan.FindConsoleObject(TEXT("MAGCF.Debug")))
    {
        ConsoleMan.UnregisterConsoleObject(TEXT("MAGCF.Debug"));
    }
    ToggleCommandHandle = ConsoleMan.RegisterConsoleCommand(TEXT("MAGCF.Debug"),
        TEXT("Toggles the global real-time MAGCF framework on-screen diagnostic HUD panel overlay."),
        FConsoleCommandDelegate::CreateUObject(this, &UMAGCFDebugSubsystem::ToggleGlobalDebug), ECVF_Cheat);

    if (ConsoleMan.FindConsoleObject(TEXT("MAGCF.Status")))
    {
        ConsoleMan.UnregisterConsoleObject(TEXT("MAGCF.Status"));
    }
    StatusCommandHandle = ConsoleMan.RegisterConsoleCommand(TEXT("MAGCF.Status"),
        TEXT("Displays persistent, detailed tracking information for a specific agent target. Usage: MAGCF.Status <Name>"),
        FConsoleCommandWithArgsDelegate::CreateUObject(this, &UMAGCFDebugSubsystem::PrintCharacterStatus), ECVF_Cheat);
}

void UMAGCFDebugSubsystem::Deinitialize()
{
    IConsoleManager& ConsoleMan = IConsoleManager::Get();

  if (ToggleCommandHandle)
    {
        ConsoleMan.UnregisterConsoleObject(TEXT("MAGCF.Debug"));
        ToggleCommandHandle = nullptr;
    }

    if (StatusCommandHandle)
    {
        ConsoleMan.UnregisterConsoleObject(TEXT("MAGCF.Status"));
        StatusCommandHandle = nullptr;
    }

    Super::Deinitialize();
}

void UMAGCFDebugSubsystem::ToggleGlobalDebug()
{
    bIsDebugHUDActive = !bIsDebugHUDActive;
    FilteredCharacterName = TEXT("");
}

void UMAGCFDebugSubsystem::PrintCharacterStatus(const TArray<FString>& Args)
{
    if (Args.Num() > 0)
    {
        FilteredCharacterName = Args[0].ToLower();
        FilterExpiryTime = GetWorld()->GetTimeSeconds() + 15.0f;
        bIsDebugHUDActive = true;
    }
}

void UMAGCFDebugSubsystem::Tick(float DeltaTime)
{
    if (bIsDebugHUDActive)
    {
        DrawDebugOverlay();
    }
}

void UMAGCFDebugSubsystem::DrawDebugOverlay()
{
    if (!GEngine || !GetWorld()) return;

    FString MapName = GetWorld()->GetMapName();
    MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    TArray<AActor*> FoundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMAGCFCharacter::StaticClass(), FoundCharacters);
    uint64 KeyCounter = 2000;

    GEngine->AddOnScreenDebugMessage(KeyCounter++, 0.0f, FColor::White,
        FString::Printf(TEXT("MAGCF SYSTEM DIAGNOSTICS (Map: %s | Agents: %d)"), *MapName, FoundCharacters.Num()));

    bool bFoundTrackedAgent = false;

    for (auto* Actor : FoundCharacters)
    {
        auto* Char = Cast<AMAGCFCharacter>(Actor);
        if (!Char) continue;

        FString CharName = Char->GetName();
        if (Char->GetPersonalityConfig())
        {
            CharName = Char->GetCharacterArchetypeName();
        }
        FString CurrentGoalStr = StaticEnum<EMAGCFGoal>()->GetNameStringByValue((int64)Char->GetCurrentGoal());

        float CurrentHungerVal = 0.0f;
        if (auto* NeedComp = Char->GetNeedComponent())
        {
            if (auto* HungerNeed = NeedComp->GetNeed(TEXT("Hunger")))
            {
                CurrentHungerVal = HungerNeed->CurrentValue;
            }
        }

        FString GoalDescription = TEXT("IDLE");
        if (Char->IsBusy()) GoalDescription = FString::Printf(TEXT("EXECUTING (%s)"), *CurrentGoalStr);
        if (FilteredCharacterName.IsEmpty())
        {
            FString OverviewText = FString::Printf(
                TEXT("[%s] State: %s | Cash: $%.2f | Hunger: %.1f"), *CharName, *GoalDescription, Char->GetMoney(), CurrentHungerVal);

            GEngine->AddOnScreenDebugMessage(KeyCounter++, 0.0f, FColor::Yellow, OverviewText);
        }
        else if (CharName.ToLower().Contains(FilteredCharacterName))
        {
            bFoundTrackedAgent = true;

            GEngine->AddOnScreenDebugMessage(KeyCounter++, 0.0f, FColor::Orange, FString::Printf(TEXT("  > PROFILE: %s"), *CharName));
            GEngine->AddOnScreenDebugMessage(
                KeyCounter++, 0.0f, FColor::White, FString::Printf(TEXT("     Objective : %s"), *GoalDescription));
            GEngine->AddOnScreenDebugMessage(
                KeyCounter++, 0.0f, FColor::White, FString::Printf(TEXT("     Wallet    : $%.2f"), Char->GetMoney()));
            GEngine->AddOnScreenDebugMessage(
                KeyCounter++, 0.0f, FColor::White, FString::Printf(TEXT("     Hunger    : %.1f / 100.0"), CurrentHungerVal));
            GEngine->AddOnScreenDebugMessage(KeyCounter++, 0.0f, FColor::White,
                FString::Printf(TEXT("    • Anim Lock : %s"), Char->IsBusy() ? TEXT("TRUE") : TEXT("FALSE")));

            if (auto* AIComp = Char->FindComponentByClass<UMAGCFAIComponent>())
            {
                float NextThink = FMath::Max(0.0f, AIComp->DecisionInterval - AIComp->GetTimeSinceLastDecision());
                GEngine->AddOnScreenDebugMessage(KeyCounter++, 0.0f, FColor::Green,
                    FString::Printf(TEXT(" LLM Thinking State      : %s (Next Prompt evaluation in: %.1fs)"),
                        AIComp->GetIsThinking() ? TEXT("THINKING...") : TEXT("WAITING"), NextThink));
            }
        }
    }

    if (!FilteredCharacterName.IsEmpty())
    {
        if (!bFoundTrackedAgent)
        {
            GEngine->AddOnScreenDebugMessage(
                KeyCounter++, 0.0f, FColor::Red, FString::Printf(TEXT("[System] Agent '%s' not found."), *FilteredCharacterName));
        }
        float TimeRemaining = FilterExpiryTime - GetWorld()->GetTimeSeconds();
        if (TimeRemaining <= 0.0f)
            FilteredCharacterName = TEXT("");
        else
            GEngine->AddOnScreenDebugMessage(
                KeyCounter++, 0.0f, FColor::Silver, FString::Printf(TEXT("  (Returning to overview in: %.1fs)"), TimeRemaining));
    }
}