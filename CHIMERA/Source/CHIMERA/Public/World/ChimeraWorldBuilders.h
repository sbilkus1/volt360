// GDD 1 (world/setting), 4 (district breakdowns), 14.2 (base building sites).
// AChimeraWorld procedurally builds Nexopolis: the Nexus hub at the centre plus
// one themed zone per ring, linked by Ring Gates. Everything uses engine basic
// shapes, so the prototype runs with zero content assets.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "Core/ChimeraArchitecture.h"
#include "GameFramework/Actor.h"
#include "ChimeraWorldBuilders.generated.h"

class AStaticMeshActor;
class AChimeraInteractable;
class ARingTeleporter;
class ANPCObject;

// The world is built this far above the template map's terrain so no template
// landscape can intersect the playable area.
static constexpr float kChimeraWorldAltitude = 100000.f;

UCLASS()
class CHIMERA_API AChimeraWorld : public AActor
{
	GENERATED_BODY()

public:
	AChimeraWorld() { PrimaryActorTick.bCanEverTick = false; }
	virtual void BeginPlay() override;

private:
	void BuildHub(UWorld* W);
	void BuildZone(UWorld* W, const FRingDef& Ring);

	static AStaticMeshActor* PlaceMesh(UWorld* W, const FString& MeshPath, FVector Loc, FVector Scale);
	static AChimeraInteractable* PlaceInteractable(UWorld* W, FVector Loc, FVector Scale,
		const FString& Prompt, EMinigameKind Kind, const FString& Message, FName StatKey, bool bDrone);
	static ARingTeleporter* PlaceGate(UWorld* W, FVector Loc, ERing Dest, const FString& Label);
	static ANPCObject* SpawnNpc(UWorld* W, FVector Loc, const FString& Name, const FString& Role);

	// GDD 4 — Procedural building construction.
	static void BuildProceduralStructure(UWorld* W, FVector Origin, const FBuildingSpec& Spec);
	static void BuildHospital(UWorld* W, FVector Origin);
	static void BuildCastle(UWorld* W, FVector Origin);
	static void BuildHighrise(UWorld* W, FVector Origin);
	static void BuildPrecinct(UWorld* W, FVector Origin);
	static void BuildShop(UWorld* W, FVector Origin);
	static void BuildHouse(UWorld* W, FVector Origin);

	int32 NpcNameCursor = 0;
};
