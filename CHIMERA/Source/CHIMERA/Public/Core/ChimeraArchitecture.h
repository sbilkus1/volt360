// GDD 4 — Architecture system. Replaces BasicShapes with proper mesh paths,
// procedural building generation, ring-specific architectural styles.
// On first UE5 compile, download Starter Content from the Vault for these paths.
#pragma once

#include "CoreMinimal.h"
#include "ChimeraArchitecture.generated.h"

// ======================================================================
// Mesh Path Database — real UE5 engine/starter content asset paths.
// ======================================================================
namespace ChimeraMeshes
{
	// === UE5 Starter Content (free, ships with engine) ===
	namespace StarterContent
	{
		inline const TCHAR* Floor_400x400 = TEXT("/Game/StarterContent/Architecture/Floor_400x400.Floor_400x400");
		inline const TCHAR* Wall_400x300  = TEXT("/Game/StarterContent/Architecture/Wall_400x300.Wall_400x300");
		inline const TCHAR* Wall_400x400  = TEXT("/Game/StarterContent/Architecture/Wall_400x400.Wall_400x400");
		inline const TCHAR* Pillar_Square  = TEXT("/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500");
		inline const TCHAR* Window_SM      = TEXT("/Game/StarterContent/Architecture/Window_SM.Window_SM");
		inline const TCHAR* Door_SM        = TEXT("/Game/StarterContent/Architecture/Door_SM.Door_SM");
		inline const TCHAR* Stairs         = TEXT("/Game/StarterContent/Architecture/Stairs.Stairs");
		inline const TCHAR* Roof_Tile      = TEXT("/StaticMesh'/Game/StarterContent/Shapes/Shape_Wedge_B.Shape_Wedge_B'");
		inline const TCHAR* Floor_Round    = TEXT("/Game/StarterContent/Architecture/Floor_400x400_Round.Floor_400x400_Round");

		// Props
		inline const TCHAR* Light_Ceiling  = TEXT("/Game/StarterContent/Props/Light_Ceiling.Light_Ceiling");
		inline const TCHAR* SM_Chair       = TEXT("/Game/StarterContent/Props/SM_Chair.SM_Chair");
		inline const TCHAR* SM_TableRound  = TEXT("/Game/StarterContent/Props/SM_TableRound.SM_TableRound");
		inline const TCHAR* SM_Couch       = TEXT("/Game/StarterContent/Props/SM_Couch.SM_Couch");
		inline const TCHAR* SM_Plant       = TEXT("/Game/StarterContent/Props/SM_Plant.SM_Plant");
		inline const TCHAR* SM_Lamp        = TEXT("/Game/StarterContent/Props/SM_Lamp_Wall.SM_Lamp_Wall");

		// Materials
		inline const TCHAR* M_Brick        = TEXT("/Game/StarterContent/Materials/M_Brick_Clay_New.M_Brick_Clay_New");
		inline const TCHAR* M_Concrete     = TEXT("/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles");
		inline const TCHAR* M_Wood         = TEXT("/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished");
		inline const TCHAR* M_Metal        = TEXT("/Game/StarterContent/Materials/M_Metal_Steel.M_Metal_Steel");
		inline const TCHAR* M_Glass        = TEXT("/Game/StarterContent/Materials/M_Glass.M_Glass");
		inline const TCHAR* M_Rubber       = TEXT("/Game/StarterContent/Materials/M_Rubber_Tile.M_Rubber_Tile");
		inline const TCHAR* M_Water        = TEXT("/Game/StarterContent/Materials/M_Water_Lake.M_Water_Lake");
	}

	// === Engine basic shapes (fallback) ===
	inline const TCHAR* Cube       = TEXT("/Engine/BasicShapes/Cube.Cube");
	inline const TCHAR* Cylinder   = TEXT("/Engine/BasicShapes/Cylinder.Cylinder");
	inline const TCHAR* Sphere     = TEXT("/Engine/BasicShapes/Sphere.Sphere");
	inline const TCHAR* Cone       = TEXT("/Engine/BasicShapes/Cone.Cone");
	inline const TCHAR* Plane      = TEXT("/Engine/BasicShapes/Plane.Plane");

	// === Quixel Megascans (free via Bridge in UE5, download at runtime) ===
	namespace Quixel
	{
		inline const TCHAR* BrickWall    = TEXT("/Game/Megascans/Surfaces/Brick_Wall_Old/");
		inline const TCHAR* StoneWall    = TEXT("/Game/Megascans/Surfaces/Stone_Wall_Castle/");
		inline const TCHAR* Marble       = TEXT("/Game/Megascans/Surfaces/Marble_White/");
		inline const TCHAR* Concrete_Damaged = TEXT("/Game/Megascans/Surfaces/Concrete_Damaged/");
		inline const TCHAR* Wood_Planks  = TEXT("/Game/Megascans/Surfaces/Wood_Planks_Oak/");
		inline const TCHAR* Metal_Rust   = TEXT("/Game/Megascans/Surfaces/Metal_Rusted_Iron/");
		inline const TCHAR* Moss         = TEXT("/Game/Megascans/Surfaces/Moss_Ground/");
		inline const TCHAR* Neon_Sign    = TEXT("/Game/Megascans/Surfaces/Neon_Light_Red/");
		inline const TCHAR* Dirt_Ground  = TEXT("/Game/Megascans/Surfaces/Dirt_Ground_Forest/");
	}

	// === Sky / environment ===
	inline const TCHAR* SkySphere      = TEXT("/Engine/EngineSky/BP_Sky_Sphere.BP_Sky_Sphere_C");
	inline const TCHAR* CloudSystem    = TEXT("/Engine/EngineSky/VolumetricClouds.VolumetricClouds");
	inline const TCHAR* SkyLight       = TEXT("/Engine/EngineSky/SkyLight.SkyLight");
	inline const TCHAR* DirectionalLight = TEXT("/Engine/EngineSky/DirectionalLight.DirectionalLight");
}

// ======================================================================
// Procedural Building Generator
// ======================================================================
USTRUCT(BlueprintType)
struct CHIMERA_API FBuildingSpec
{
	GENERATED_BODY()
	int32 Floors = 1;
	int32 WidthSegments = 3;     // how many wall sections wide
	int32 DepthSegments = 3;     // how many wall sections deep
	float WallHeight = 400.f;    // per floor
	bool bHasRoof = true;
	bool bHasWindows = true;
	bool bHasDoor = true;
	FString WallMaterial;
	FString RoofMaterial;
	FString FloorMaterial;
	FString Style;               // "Modern", "Classical", "Cyberpunk", "Fantasy", "Medieval", "Voxel"
};

// ======================================================================
// Ring-specific architectural profiles
// ======================================================================
namespace ChimeraArchStyle
{
	inline FBuildingSpec NexusHub()
	{
		FBuildingSpec S; S.Floors = 3; S.WidthSegments = 8; S.DepthSegments = 8;
		S.WallHeight = 500.f; S.Style = TEXT("Classical");
		S.WallMaterial = TEXT("Marble"); S.RoofMaterial = TEXT("Metal"); S.FloorMaterial = TEXT("Concrete");
		return S;
	}

	inline FBuildingSpec AetherCityTower()
	{
		FBuildingSpec S; S.Floors = 40; S.WidthSegments = 4; S.DepthSegments = 4;
		S.WallHeight = 350.f; S.Style = TEXT("Modern");
		S.WallMaterial = TEXT("Metal"); S.RoofMaterial = TEXT("Metal"); S.FloorMaterial = TEXT("Concrete");
		return S;
	}

	inline FBuildingSpec NeoKingdomCastle()
	{
		FBuildingSpec S; S.Floors = 5; S.WidthSegments = 6; S.DepthSegments = 5;
		S.WallHeight = 600.f; S.Style = TEXT("Fantasy");
		S.WallMaterial = TEXT("Stone"); S.RoofMaterial = TEXT("Stone"); S.FloorMaterial = TEXT("Stone");
		return S;
	}

	inline FBuildingSpec MercyHospital()
	{
		FBuildingSpec S; S.Floors = 3; S.WidthSegments = 10; S.DepthSegments = 5;
		S.WallHeight = 400.f; S.Style = TEXT("Modern");
		S.WallMaterial = TEXT("Concrete"); S.RoofMaterial = TEXT("Metal"); S.FloorMaterial = TEXT("Rubber");
		return S;
	}

	inline FBuildingSpec MidWilshirePrecinct()
	{
		FBuildingSpec S; S.Floors = 2; S.WidthSegments = 6; S.DepthSegments = 4;
		S.WallHeight = 400.f; S.Style = TEXT("Modern");
		S.WallMaterial = TEXT("Brick"); S.RoofMaterial = TEXT("Metal"); S.FloorMaterial = TEXT("Concrete");
		return S;
	}

	inline FBuildingSpec AlagaesiaCottage()
	{
		FBuildingSpec S; S.Floors = 1; S.WidthSegments = 3; S.DepthSegments = 2;
		S.WallHeight = 350.f; S.Style = TEXT("Medieval");
		S.WallMaterial = TEXT("Stone"); S.RoofMaterial = TEXT("Wood"); S.FloorMaterial = TEXT("Wood");
		return S;
	}

	inline FBuildingSpec NightCityHighrise()
	{
		FBuildingSpec S; S.Floors = 30; S.WidthSegments = 4; S.DepthSegments = 4;
		S.WallHeight = 350.f; S.Style = TEXT("Cyberpunk");
		S.WallMaterial = TEXT("Concrete"); S.RoofMaterial = TEXT("Metal"); S.FloorMaterial = TEXT("Concrete");
		return S;
	}

	inline FBuildingSpec CleveleysShop()
	{
		FBuildingSpec S; S.Floors = 2; S.WidthSegments = 3; S.DepthSegments = 2;
		S.WallHeight = 300.f; S.Style = TEXT("Classical");
		S.WallMaterial = TEXT("Brick"); S.RoofMaterial = TEXT("Tile"); S.FloorMaterial = TEXT("Wood");
		return S;
	}

	inline FBuildingSpec CheapHouse()
	{
		FBuildingSpec S; S.Floors = 1; S.WidthSegments = 2; S.DepthSegments = 2;
		S.WallHeight = 300.f; S.Style = TEXT("Modern");
		S.WallMaterial = TEXT("Brick"); S.RoofMaterial = TEXT("Tile"); S.FloorMaterial = TEXT("Wood");
		return S;
	}
}

// ======================================================================
// Material remapping: logical name → actual UE5 asset path
// ======================================================================
namespace ChimeraMaterials
{
	inline const TCHAR* GetPath(const FString& LogicalName)
	{
		if (LogicalName == TEXT("Brick"))    return ChimeraMeshes::StarterContent::M_Brick;
		if (LogicalName == TEXT("Concrete")) return ChimeraMeshes::StarterContent::M_Concrete;
		if (LogicalName == TEXT("Wood"))     return ChimeraMeshes::StarterContent::M_Wood;
		if (LogicalName == TEXT("Metal"))    return ChimeraMeshes::StarterContent::M_Metal;
		if (LogicalName == TEXT("Glass"))    return ChimeraMeshes::StarterContent::M_Glass;
		if (LogicalName == TEXT("Rubber"))   return ChimeraMeshes::StarterContent::M_Rubber;
		if (LogicalName == TEXT("Water"))    return ChimeraMeshes::StarterContent::M_Water;
		if (LogicalName == TEXT("Marble"))   return ChimeraMeshes::Quixel::Marble;
		if (LogicalName == TEXT("Stone"))    return ChimeraMeshes::Quixel::StoneWall;
		if (LogicalName == TEXT("Neon"))     return ChimeraMeshes::Quixel::Neon_Sign;
		if (LogicalName == TEXT("Tile"))     return ChimeraMeshes::StarterContent::M_Concrete;
		if (LogicalName == TEXT("Dirt"))     return ChimeraMeshes::Quixel::Dirt_Ground;
		return ChimeraMeshes::StarterContent::M_Concrete; // fallback
	}
}

// ======================================================================
// Procedural Building Generator (call at runtime to spawn detailed structures)
// ======================================================================
// Usage on UE5 machine:
//   UWorld* W = GetWorld();
//   FBuildingSpec Spec = ChimeraArchStyle::NexusHub();
//   AChimeraWorld::BuildProceduralStructure(W, FVector(0,0,0), Spec);

// Building blocks needed:
//   - Floor slabs (WidthSegments × DepthSegments × Floors)
//   - Wall sections (4 sides × Floors, each WidthSegments or DepthSegments long)
//   - Window cutouts on walls with bHasWindows
//   - Door on front face with bHasDoor
//   - Roof (flat, gabled, or dome based on Style)
//   - Trim/cornices at floor transitions
//   - Base/foundation slab

// Material application:
//   Each building block gets the appropriate StarterContent or Quixel material
//   based on the Style and logical material names.

// On first UE5 compile:
//   1. Download "Starter Content" from Vault → Add to project
//   2. Open Quixel Bridge → Download Megascans surfaces referenced above
//   3. The code loads meshes via LoadObject<UStaticMesh> and applies materials via
//      MeshComponent->SetMaterial(0, LoadObject<UMaterialInterface>(MaterialPath))
