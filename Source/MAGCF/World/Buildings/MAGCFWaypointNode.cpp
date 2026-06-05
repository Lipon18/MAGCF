// MAGCF - Multi-Agent Generative Character Framework Copyright (c) 2026 Your Lipon / Psycho Games. All Rights Reserved.    MAGCF is an experimental research framework for autonomous AI-driven    characters and multi-agent simulation within Unreal Engine environments. Unauthorized copying, modification, distribution, or use of this software without explicit permission is prohibited.


#include "MAGCF/World/Buildings/MAGCFWaypointNode.h"

// Sets default values
AMAGCFWaypointNode::AMAGCFWaypointNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMAGCFWaypointNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMAGCFWaypointNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

