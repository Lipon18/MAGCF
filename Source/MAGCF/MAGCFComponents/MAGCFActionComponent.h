// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Lipon / Psycho Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAGCF/Enums/EGoals.h"
#include "Templates/SubclassOf.h"
#include "MAGCFActionComponent.generated.h"

class AMAGCFCharacter;
class UAnimMontage;
class UMAGCFActionStateBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGCF_API UMAGCFActionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMAGCFActionComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void ExecuteGoal(EMAGCFGoal Goal);
    void CancelCurrentAction();
    void ClearCurrentAction();

    FORCEINLINE bool IsPlayingActionMontage() const { return bIsPlayingActionMontage; }
    FORCEINLINE bool HasSpawnedProp() const { return SpawnedBreadActor != nullptr; }
    FORCEINLINE bool IsCurrentlyInState(const EMAGCFGoal Goal) const { return CurrentGoalType == Goal; }

    bool InternalPlayActionMontage(UAnimMontage* const TargetMontage, const FName StartSection = NAME_None);
    void HaltCharacterVelocity();
    void RestoreCharacterMovement();

    FORCEINLINE UAnimMontage* GetEatMontage() const { return EatMontage; }
    FORCEINLINE UAnimMontage* GetDanceMontage() const { return DanceMontage; }
    FORCEINLINE UAnimMontage* GetPhoneMontage() const { return PhoneMontage; }
    FORCEINLINE UAnimMontage* GetGroundSitMontage() const { return GroundSitMontage; }
    FORCEINLINE UAnimMontage* GetBoxingJab1Montage() const { return BoxingJab1Montage; }
    FORCEINLINE UAnimMontage* GetBoxingJab2Montage() const { return BoxingJab2Montage; }
    FORCEINLINE UAnimMontage* GetPushUpMontage() const { return PushUpMontage; }

    FORCEINLINE FName GetHandSocketName() const { return HandSocketName; }
    FORCEINLINE TSubclassOf<AActor> GetBreadClass() const { return BreadClass; }
    FORCEINLINE float GetGroundRestEnergyRecoveryRate() const { return GroundRestEnergyRecoveryRate; }

    FORCEINLINE AActor* GetSpawnedBreadActor() const { return SpawnedBreadActor; }
    FORCEINLINE void SetSpawnedBreadActor(AActor* const NewProp) { SpawnedBreadActor = NewProp; }
    FORCEINLINE void SetIsPlayingActionMontage(const bool bPlaying) { bIsPlayingActionMontage = bPlaying; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Animations")
    TObjectPtr<UAnimMontage> EatMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Animations")
    TObjectPtr<UAnimMontage> DanceMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Animations")
    TObjectPtr<UAnimMontage> PhoneMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Animations")
    TObjectPtr<UAnimMontage> GroundSitMontage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Animations")
    TObjectPtr<UAnimMontage> BoxingJab1Montage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Animations")
    TObjectPtr<UAnimMontage> BoxingJab2Montage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Animations")
    TObjectPtr<UAnimMontage> PushUpMontage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF|Action|Configuration")
    FName HandSocketName = TEXT("Hand_R_Socket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAGCF|Action|Configuration")
    TSubclassOf<AActor> BreadClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MAGCF|Action|Configuration")
    float GroundRestEnergyRecoveryRate = 12.0f;

private:
    UPROPERTY()
    AMAGCFCharacter* OwnerCharacter = nullptr;

    UPROPERTY()
    AActor* SpawnedBreadActor = nullptr;

    UPROPERTY()
    UMAGCFActionStateBase* CurrentActionState = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "MAGCF|AI")
    TMap<EMAGCFGoal, TSubclassOf<UMAGCFActionStateBase>> ActionStateClasses;

    bool bIsPlayingActionMontage = false;
    EMAGCFGoal CurrentGoalType = EMAGCFGoal::E_NONE;

    UFUNCTION()
    void OnCurrentStateMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};