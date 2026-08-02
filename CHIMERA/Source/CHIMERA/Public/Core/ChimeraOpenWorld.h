// GDD 1 / 4 — True Open World System. Nexopolis is one continuous landmass 
// where 11 realities fused. No teleport-only zones — you can walk, drive, or
// fly between all 15 rings across seamless terrain.
//
// Terrain: UE5 Landscape system with World Partition for streaming.
// Roads: spline-based network connecting all rings.
// Foliage: PCG (Procedural Content Generation) for trees, rocks, grass.
// Water: single ocean with rivers flowing through rings.
//
// World dimensions: ~2,400 km² total (48km × 50km playable area)
// Each ring occupies 1-5 km² of developed area within the landscape.

#pragma once
#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "ChimeraOpenWorld.generated.h"

// ======================================================================
// World geometry: concentric ring layout on real terrain
// ======================================================================
// The Nexus is at world center (0,0). Rings radiate outward.
// Instead of floating zone anchors, each ring has:
//   - A terrain region (hills, plains, mountains, coast)
//   - A developed city area at its centre
//   - Connecting roads/highways to adjacent rings
//   - Natural borders (rivers, mountain ridges, coastline)

USTRUCT(BlueprintType)
struct FWorldZone
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() FVector2D CityCenter;   // where the ring's city is (km from world origin)
	UPROPERTY() float CityRadiusKm = 2.f; // developed urban area
	UPROPERTY() float TerrainRadiusKm = 5.f; // natural terrain around city
	UPROPERTY() FString TerrainType;    // "Coastal", "Mountain", "Plains", "Forest", "Desert", "Swamp", "Snow"
	UPROPERTY() float Elevation = 0.f;  // base height above sea level (metres)
};

USTRUCT(BlueprintType)
struct FWorldRoad
{
	GENERATED_BODY()
	UPROPERTY() int32 FromRing;
	UPROPERTY() int32 ToRing;
	UPROPERTY() FString RoadType;       // "Highway", "MainRoad", "CountryRoad", "Dirt"
	UPROPERTY() float LengthKm = 5.f;
	UPROPERTY() int32 Lanes = 2;
};

namespace ChimeraOpenWorld
{
	// ==================================================================
	// 15 zone definitions — each as a real place on the landscape
	// ==================================================================
	inline TArray<FWorldZone> GetWorldZones()
	{
		TArray<FWorldZone> Z;

		// Ring 0 — Nexus at centre
		Z.Add({ TEXT("The Nexus"),       FVector2D(0, 0),       3.f, 6.f, TEXT("Plains"),    100.f });

		// Inner ring (~8km radius)
		Z.Add({ TEXT("Aether City"),     FVector2D(8, 0),      4.f, 8.f, TEXT("Coastal"),   50.f });
		Z.Add({ TEXT("Neo-Kingdom"),     FVector2D(-5, 6),     3.f, 6.f, TEXT("Forest"),    80.f });
		Z.Add({ TEXT("Mercy Heights"),   FVector2D(-8, -1),    2.f, 5.f, TEXT("Plains"),    40.f });
		Z.Add({ TEXT("Mid-Wilshire"),    FVector2D(2, -8),     5.f, 8.f, TEXT("Plains"),    30.f });
		Z.Add({ TEXT("Seattle"),         FVector2D(-9, -7),    3.f, 6.f, TEXT("Coastal"),   20.f });

		// Middle ring (~14km radius)
		Z.Add({ TEXT("Grand Coliseum"),  FVector2D(14, 3),     3.f, 6.f, TEXT("Plains"),    60.f });
		Z.Add({ TEXT("Night City"),      FVector2D(-3, -14),   4.f, 8.f, TEXT("Coastal"),   10.f });
		Z.Add({ TEXT("Chicago"),         FVector2D(-13, 5),    4.f, 7.f, TEXT("Plains"),    45.f });
		Z.Add({ TEXT("Chastain Park"),   FVector2D(10, -10),   2.f, 5.f, TEXT("Forest"),    35.f });

		// Outer ring (~22km radius)
		Z.Add({ TEXT("Alagaesia"),       FVector2D(0, -22),    5.f, 15.f, TEXT("Mountain"), 200.f });
		Z.Add({ TEXT("Minecraft Frontier"), FVector2D(20, 8),  6.f, 12.f, TEXT("Plains"),   80.f });
		Z.Add({ TEXT("Panem"),           FVector2D(-10, 18),   8.f, 14.f, TEXT("Forest"),   120.f });
		Z.Add({ TEXT("Cleveleys"),       FVector2D(-20, -5),   1.5f, 3.f, TEXT("Coastal"),  5.f });
		Z.Add({ TEXT("VernetLesBains"),  FVector2D(18, -15),   1.5f, 5.f, TEXT("Mountain"), 400.f });

		return Z;
	}

	// ==================================================================
	// Road network — every ring connected by physical roads
	// ==================================================================
	inline TArray<FWorldRoad> GetRoadNetwork()
	{
		TArray<FWorldRoad> R;
		// Nexus spokes (highways radiating from centre)
		R.Add({ 0, 1,  TEXT("Highway"),    8.f,  6 });
		R.Add({ 0, 2,  TEXT("Highway"),    8.f,  6 });
		R.Add({ 0, 3,  TEXT("Highway"),    8.f,  6 });
		R.Add({ 0, 4,  TEXT("Highway"),    8.f,  6 });
		R.Add({ 0, 5,  TEXT("Highway"),    8.f,  6 });

		// Inner ring road (circle connecting inner rings)
		R.Add({ 1, 2,  TEXT("MainRoad"),   7.f,  4 });
		R.Add({ 2, 4,  TEXT("MainRoad"),   9.f,  4 });
		R.Add({ 4, 3,  TEXT("MainRoad"),   8.f,  4 });
		R.Add({ 3, 5,  TEXT("MainRoad"),   6.f,  4 });
		R.Add({ 5, 1,  TEXT("MainRoad"),   10.f, 4 });

		// Middle ring connections
		R.Add({ 1, 6,  TEXT("Highway"),    7.f,  4 });
		R.Add({ 4, 7,  TEXT("Highway"),    8.f,  4 });
		R.Add({ 2, 8,  TEXT("MainRoad"),   8.f,  4 });
		R.Add({ 5, 9,  TEXT("MainRoad"),   5.f,  2 });

		// Outer ring connections
		R.Add({ 7, 10, TEXT("MainRoad"),  12.f,  4 });
		R.Add({ 6, 10, TEXT("MainRoad"),  15.f,  4 });
		R.Add({ 8, 11, TEXT("CountryRoad"), 8.f, 2 });
		R.Add({ 3, 11, TEXT("CountryRoad"), 10.f, 2 });
		R.Add({ 2, 12, TEXT("CountryRoad"), 12.f, 2 });
		R.Add({ 6, 12, TEXT("MainRoad"),   8.f,  4 });
		R.Add({ 5, 13, TEXT("CountryRoad"), 9.f, 2 });
		R.Add({ 9, 14, TEXT("Mountain"),    6.f, 2 });
		R.Add({ 10, 12,TEXT("Mountain"),   10.f, 2 });
		R.Add({ 11, 14,TEXT("CountryRoad"), 8.f, 2 });

		return R;
	}

	// ==================================================================
	// Terrain types → biome settings for landscape material
	// ==================================================================
	inline FString GetBiomeFoliage(const FString& TerrainType)
	{
		if (TerrainType == TEXT("Coastal"))   return TEXT("PalmTrees, BeachGrass, Driftwood, Seagulls");
		if (TerrainType == TEXT("Mountain"))  return TEXT("PineTrees, Boulders, AlpineFlowers, Eagles");
		if (TerrainType == TEXT("Forest"))    return TEXT("OakTrees, Ferns, Mushrooms, Deer");
		if (TerrainType == TEXT("Plains"))    return TEXT("Grass, Wildflowers, Hedgerows, Rabbits");
		if (TerrainType == TEXT("Desert"))    return TEXT("Cacti, Scrub, Vultures, Tumbleweed");
		if (TerrainType == TEXT("Swamp"))     return TEXT("CypressTrees, Moss, Fireflies, Crocodiles");
		if (TerrainType == TEXT("Snow"))      return TEXT("SnowPines, IceFormations, PolarBears, Auroras");
		return TEXT("Grass, Trees");
	}

	// ==================================================================
	// UE5 World Partition Setup
	// ==================================================================
	// In DefaultEngine.ini:
	//   [/Script/Engine.WorldPartition]
	//   WorldPartition.EnableStreaming=1
	//   WorldPartition.GridSize=51200  (50km grid, matches our world)
	//   WorldPartition.CellSize=25600
	//
	// In the UE5 editor after first compile:
	//   1. Create new empty level
	//   2. Tools → Convert Level → Enable World Partition
	//   3. Add Landscape actor (48km×50km, 1009×1009 resolution)
	//   4. Sculpt terrain using zone elevations above
	//   5. Paint biome materials using the terrain types above
	//   6. Add PCG volume → configure to spawn foliage from GetBiomeFoliage()
	//   7. Add road splines following GetRoadNetwork()
	//   8. Place city buildings at the CityCenter coordinates
	//   9. Enable World Partition streaming (auto-loads cells as player moves)

	// ==================================================================
	// Water system
	// ==================================================================
	// Single ocean at Z=0m surrounds the landmass.
	// Rivers flow from mountain zones (Alagaesia, VernetLesBains) to the sea.
	// Lakes in Minecraft Frontier and Neo-Kingdom.
	//
	// UE5 Water plugin setup:
	//   Plugin → Enable "Water" and "Water Extras"
	//   Add Ocean actor at Z=0, covering full world extent
	//   Add River actors along splines from mountains to coast
	//   Add Lake actors in Frontier and Neo-Kingdom depression zones

	// ==================================================================
	// NPC population density — more NPCs in cities, fewer in wilderness
	// ==================================================================
	inline int32 GetNPCDensity(const FString& Ring, bool bIsCity)
	{
		if (!bIsCity) return 2;  // 2 NPCs per km² in rural areas
		if (Ring == TEXT("Mid-Wilshire") || Ring == TEXT("Night City")) return 80;
		if (Ring == TEXT("Aether City") || Ring == TEXT("Neo-Kingdom")) return 60;
		if (Ring == TEXT("Seattle") || Ring == TEXT("Chicago")) return 50;
		return 30;
	}

	// ==================================================================
	// Vehicle traffic density
	// ==================================================================
	inline float GetTrafficDensity(const FString& RoadType)
	{
		if (RoadType == TEXT("Highway")) return 0.8f;     // car every 1.25s
		if (RoadType == TEXT("MainRoad")) return 0.5f;   // car every 2s
		if (RoadType == TEXT("CountryRoad")) return 0.2f; // car every 5s
		if (RoadType == TEXT("Dirt") || RoadType == TEXT("Mountain")) return 0.05f;
		return 0.3f;
	}

	// ==================================================================
	// Travel times between rings (at 120km/h highway speed)
	// ==================================================================
	inline float GetTravelTimeMinutes(const FWorldRoad& Road)
	{
		return (Road.LengthKm / 120.f) * 60.f;
	}

	// ==================================================================
	// Ring Gate system — kept for fast-travel but physical roads exist
	// ==================================================================
	// Ring gates become fast-travel points rather than the only way to travel.
	// Walk/drive/fly between rings takes real time.
	// Gate fast-travel is instant (in-game time passes during transit).
	// Fast travel cost: credits proportional to distance, free with season pass.
}
