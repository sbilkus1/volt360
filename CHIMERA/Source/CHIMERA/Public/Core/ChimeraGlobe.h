// GDD 1 — Globe Redesign. CHIMERA is a planet called Nexopolis Prime.
// The Crash fused 11 realities into a single supercontinent. All 15 regions
// are physically connected with natural borders — no teleport gates needed.
// You can walk, drive, fly, or sail between every location seamlessly.
//
// Planet: Nexopolis Prime
// Radius: ~2,500 km (smaller than Earth for playability, ~1/3 Earth)
// Circumference: ~15,700 km
// Surface: 70% ocean, 30% land
// Continents: 4 (Nexopolis supercontinent + 3 smaller)
// Population: ~50M NPCs (procedurally generated from the 100+ named NPCs)

#pragma once
#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "ChimeraGlobe.generated.h"

// ======================================================================
// Spherical coordinate system
// ======================================================================
USTRUCT(BlueprintType)
struct FGeoCoord
{
	GENERATED_BODY()
	float Latitude = 0.f;   // -90 to +90
	float Longitude = 0.f;  // -180 to +180
	float Altitude = 0.f;   // metres above sea level
};

USTRUCT(BlueprintType)
struct FGlobeRegion
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() FString DisplayName;
	UPROPERTY() FString Genre;
	UPROPERTY() FGeoCoord Center;         // region centre
	UPROPERTY() float RadiusKm = 5.f;     // developed urban radius
	UPROPERTY() float WildRadiusKm = 20.f; // surrounding wilderness
	UPROPERTY() FString Climate;          // "Temperate", "Tropical", "Arctic", "Mediterranean", "Desert"
	UPROPERTY() FString Terrain;          // "Coastal", "Mountain", "Plains", "Forest", "Swamp", "Canyon", "Volcanic"
	UPROPERTY() FString Continent;        // "Nexopolis", "Austral", "Boreal", "Oriental"
	UPROPERTY() int32 Population;         // estimated NPC count
	UPROPERTY() bool bCoastal = false;
	UPROPERTY() bool bHasRiver = false;
};

// ======================================================================
// Physical borders between regions (no teleport gates)
// ======================================================================
USTRUCT(BlueprintType)
struct FRegionBorder
{
	GENERATED_BODY()
	UPROPERTY() FString RegionA;
	UPROPERTY() FString RegionB;
	UPROPERTY() FString BorderType;  // "River", "MountainRange", "Highway", "MainRoad", "Forest", "Canyon", "Cliff"
	UPROPERTY() FString Crossing;    // "Bridge", "Tunnel", "Ferry", "Pass", "Ford", "Causeway", "None"
	UPROPERTY() float LengthKm = 10.f;
};

namespace ChimeraGlobe
{
	// ======================================================================
	// Nexopolis Prime — the 15 regions of the supercontinent
	// ======================================================================
	inline TArray<FGlobeRegion> GetRegions()
	{
		TArray<FGlobeRegion> R;

		// CENTRAL HEARTLAND
		R.Add({ TEXT("Nexus"), TEXT("The Nexus"), TEXT("Hub / Narrative Core"),
			{ 0.f, 0.f }, 4.f, 12.f, TEXT("Temperate"), TEXT("Plains"), TEXT("Nexopolis"), 500000, false, true });

		// COASTAL EAST (Aether City is a massive coastal megacity)
		R.Add({ TEXT("AetherCity"), TEXT("Aether City"), TEXT("Superhero Sci-Fi"),
			{ 2.f, 6.f }, 8.f, 25.f, TEXT("Temperate"), TEXT("Coastal"), TEXT("Nexopolis"), 2000000, true, false });
		R.Add({ TEXT("GrandColiseum"), TEXT("Grand Coliseum"), TEXT("Sports & Racing"),
			{ 5.f, 10.f }, 5.f, 15.f, TEXT("Mediterranean"), TEXT("Coastal"), TEXT("Nexopolis"), 800000, true, false });

		// MOUNTAINOUS NORTH (cold, fantasy realm)
		R.Add({ TEXT("Alagaesia"), TEXT("Alagaësia Wilds"), TEXT("High Fantasy"),
			{ -1.f, 4.f }, 6.f, 40.f, TEXT("Arctic"), TEXT("Mountain"), TEXT("Nexopolis"), 300000, false, true });
		R.Add({ TEXT("VernetLesBains"), TEXT("Vernet-les-Bains"), TEXT("Pyrenees Sanctuary"),
			{ -3.f, 8.f }, 2.f, 10.f, TEXT("Arctic"), TEXT("Mountain"), TEXT("Nexopolis"), 15000, false, true });

		// WESTERN PLAINS (sprawling urban, police, medical)
		R.Add({ TEXT("MidWilshire"), TEXT("Mid-Wilshire"), TEXT("Crime / Police"),
			{ 0.f, -4.f }, 6.f, 20.f, TEXT("Temperate"), TEXT("Plains"), TEXT("Nexopolis"), 1500000, false, true });
		R.Add({ TEXT("MercyHeights"), TEXT("Mercy Heights"), TEXT("Medical"),
			{ -2.f, -6.f }, 3.f, 12.f, TEXT("Temperate"), TEXT("Plains"), TEXT("Nexopolis"), 400000, false, false });
		R.Add({ TEXT("Seattle"), TEXT("Seattle"), TEXT("Medical Drama"),
			{ -4.f, -8.f }, 4.f, 15.f, TEXT("Temperate"), TEXT("Coastal"), TEXT("Nexopolis"), 700000, true, true });
		R.Add({ TEXT("ChastainPark"), TEXT("Chastain Park"), TEXT("Medical"),
			{ -6.f, -4.f }, 3.f, 10.f, TEXT("Temperate"), TEXT("Forest"), TEXT("Nexopolis"), 300000, false, false });

		// SOUTHERN INDUSTRIAL / DYSTOPIAN
		R.Add({ TEXT("NightCity"), TEXT("Night City"), TEXT("Cyberpunk"),
			{ 2.f, -8.f }, 6.f, 20.f, TEXT("Temperate"), TEXT("Coastal"), TEXT("Nexopolis"), 1800000, true, false });
		R.Add({ TEXT("Panem"), TEXT("Panem"), TEXT("Post-Apocalyptic"),
			{ 6.f, -2.f }, 10.f, 30.f, TEXT("Mediterranean"), TEXT("Forest"), TEXT("Nexopolis"), 2500000, false, true });

		// EASTERN FANTASY / EDUCATION
		R.Add({ TEXT("NeoKingdom"), TEXT("Neo-Kingdom"), TEXT("Magical Academy"),
			{ 3.f, 3.f }, 4.f, 18.f, TEXT("Temperate"), TEXT("Forest"), TEXT("Nexopolis"), 500000, false, true });
		R.Add({ TEXT("MinecraftFrontier"), TEXT("Minecraft Frontier"), TEXT("Voxel Sandbox"),
			{ 8.f, 4.f }, 8.f, 30.f, TEXT("Temperate"), TEXT("Plains"), TEXT("Nexopolis"), 200000, false, true });

		// FAR SOUTHWEST COAST
		R.Add({ TEXT("Chicago"), TEXT("Chicago"), TEXT("Emergency Medicine"),
			{ -8.f, 2.f }, 5.f, 18.f, TEXT("Temperate"), TEXT("Plains"), TEXT("Nexopolis"), 2700000, true, true });

		// SEASIDE (far northwest coast)
		R.Add({ TEXT("Cleveleys"), TEXT("Cleveleys"), TEXT("Seaside Town"),
			{ -6.f, 10.f }, 1.5f, 5.f, TEXT("Temperate"), TEXT("Coastal"), TEXT("Nexopolis"), 12000, true, false });

		return R;
	}

	// ======================================================================
	// Natural Borders — physical boundaries between regions
	// ======================================================================
	inline TArray<FRegionBorder> GetBorders()
	{
		TArray<FRegionBorder> B;
		// Rivers form natural borders
		B.Add({ TEXT("Nexus"), TEXT("NeoKingdom"), TEXT("River"), TEXT("Bridge") });
		B.Add({ TEXT("MidWilshire"), TEXT("MercyHeights"), TEXT("River"), TEXT("Bridge") });
		B.Add({ TEXT("Alagaesia"), TEXT("VernetLesBains"), TEXT("MountainRange"), TEXT("Pass") });
		B.Add({ TEXT("Panem"), TEXT("MinecraftFrontier"), TEXT("River"), TEXT("Ferry") });
		B.Add({ TEXT("Seattle"), TEXT("ChastainPark"), TEXT("Forest"), TEXT("Highway") });

		// Mountain ranges
		B.Add({ TEXT("Alagaesia"), TEXT("Nexus"), TEXT("MountainRange"), TEXT("Tunnel") });
		B.Add({ TEXT("VernetLesBains"), TEXT("Cleveleys"), TEXT("MountainRange"), TEXT("Pass") });

		// Coastlines with ferry connections
		B.Add({ TEXT("AetherCity"), TEXT("GrandColiseum"), TEXT("Coastal"), TEXT("Ferry") });
		B.Add({ TEXT("NightCity"), TEXT("Panem"), TEXT("River"), TEXT("Bridge") });
		B.Add({ TEXT("Cleveleys"), TEXT("Seattle"), TEXT("Coastal"), TEXT("Ferry") });

		// Urban sprawl connections
		B.Add({ TEXT("MidWilshire"), TEXT("Nexus"), TEXT("Highway"), TEXT("None") });
		B.Add({ TEXT("Chicago"), TEXT("MercyHeights"), TEXT("Highway"), TEXT("None") });
		B.Add({ TEXT("Panem"), TEXT("NightCity"), TEXT("MainRoad"), TEXT("None") });

		return B;
	}

	// ======================================================================
	// Climate zones (latitude-based)
	// ======================================================================
	inline FString GetClimateAtLatitude(float Lat)
	{
		if (FMath::Abs(Lat) > 60.f) return TEXT("Arctic");      // poles
		if (FMath::Abs(Lat) > 40.f) return TEXT("Temperate");   // mid-latitudes
		if (FMath::Abs(Lat) > 20.f) return TEXT("Mediterranean"); // warm
		return TEXT("Tropical");                                 // equator
	}

	// ======================================================================
	// Seamless world wrapping
	// ======================================================================
	// UE5 World Partition handles the streaming. The world is organised as:
	//   Nexopolis supercontinent (centre, ~100km × 80km)
	//   Northern ocean (arctic, icebergs, polar research station)
	//   Southern ocean (warm, tropical islands, underwater Drowned District)
	//   Eastern ocean (deep water, sea serpents)
	//   Western ocean (trade routes, shipping lanes)
	//
	// Player wraps at world edges:
	//   Going west past -50km → appears at +50km east
	//   Going north past +40km → appears at -40km south
	//   This creates seamless globe illusion

	// ======================================================================
	// Travel infrastructure
	// ======================================================================
	// HIGHWAYS: Nexus → every region (6-lane, 130km/h limit)
	// MAIN ROADS: between adjacent regions (4-lane, 100km/h)
	// COUNTRY ROADS: rural connections (2-lane, 80km/h)
	// MOUNTAIN PASSES: switchbacks through ranges (2-lane, 60km/h)
	// FERRIES: coastal crossings (30km/h)
	// TUNNELS: under mountain ranges (2-lane, 90km/h)
	// BRIDGES: over rivers (2-4 lanes)
	//
	// BOATS: player-owned, purchasable. Sail between coastal regions.
	// PLANES: small aircraft between regions. Purchasable at Grand Coliseum.
	// DRAGON: fly over any terrain at 200km/h+.
	// TRAIN: Nexopolis Express connects all major regions (free with season pass).

	// ======================================================================
	// Region development levels
	// ======================================================================
	inline float GetRegionDensity(int32 Population)
	{
		if (Population > 1500000) return 1.f;   // megacity
		if (Population > 500000) return 0.7f;   // major city
		if (Population > 100000) return 0.4f;   // town
		return 0.1f;                            // village/rural
	}

	// ======================================================================
	// The Drowned District (underwater ring, accessible by submarine/diving)
	// ======================================================================
	// GDD 14.10 — Located in the Southern Ocean at Lat -15, Lon +8.
	// A fragment of a water-world reality that merged during the Crash.
	// Depth: -500m to -2000m. Requires submarine or underwater breathing spell.
	// Contains: sunken ruins, marine life, underwater bases, sea serpents.

	// ======================================================================
	// UE5 Implementation Notes
	// ======================================================================
	// 1. Create a World Partition level spanning 100km × 80km
	// 2. Landscape: use the elevations from GetRegions() to sculpt terrain
	// 3. Coastline: use ocean actor at Z=0, sculpt land above it
	// 4. Rivers: spline-based Water plugin rivers
	// 5. Roads: spline-based with the traffic system
	// 6. Cities: PCG volumes placed at region CityCenter coordinates
	// 7. Foliage: PCG volumes using GetClimateAtLatitude() for biome selection
	// 8. World wrap: custom actor that teleports player at boundary edges
}
