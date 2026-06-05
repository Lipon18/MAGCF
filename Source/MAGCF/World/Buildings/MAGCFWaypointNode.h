// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.    MAGCF is an experimental research framework for autonomous AI-driven    characters and multi-agent simulation within Unreal Engine environments. Unauthorized copying, modification, distribution, or use of this software without explicit permission is prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MAGCFWaypointNode.generated.h"

UCLASS()
class MAGCF_API AMAGCFWaypointNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMAGCFWaypointNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
