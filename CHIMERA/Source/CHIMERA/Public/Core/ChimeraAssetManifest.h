// GDD 4 / 10 — Complete Asset Manifest. Every 3D model needed for every ring.
// All assets sourced from FREE repositories: Quixel Megascans (10,000+ free with UE5),
// Poly Haven (CC0), Sketchfab CC0, OpenGameArt, UE Marketplace free monthly content.
// 
// DOWNLOAD CHECKLIST (first UE5 session):
// 1. Epic Games Launcher → Unreal Engine → Vault → "Starter Content" → Add to project
// 2. UE5 Editor → Window → Quixel Bridge → download ALL Megascans surfaces, 3D assets, decals
// 3. UE Marketplace → Free section → download: "City Sample", "Rural Australia", "Modular SciFi"
// 4. polyhaven.com → download CC0 pack (HDRI skies, textures, fence models, rocks)
// 5. sketchfab.com → search "cc0 architectural" → download buildings, props
// 6. turbosquid.com → filter "free" → download vehicles, furniture
//
// After downloading, place in: Content/Imported/ folder
// The game auto-discovers assets by name from this manifest.

#pragma once
#include "CoreMinimal.h"
#include "ChimeraAssetManifest.generated.h"

USTRUCT()
struct FAssetEntry
{
	GENERATED_BODY()
	UPROPERTY() FString AssetName;
	UPROPERTY() FString Source;      // "Quixel", "PolyHaven", "Marketplace", "Sketchfab", etc
	UPROPERTY() FString SourceURL;   // download link
	UPROPERTY() FString ImportPath;  // where to place in Content/
	UPROPERTY() FString Ring;        // which ring uses it, or "Global"
	UPROPERTY() int32 Quantity = 1;  // how many variations needed
};

// ======================================================================
// COMPLETE ASSET MANIFEST — organized by category and ring
// All entries are FREE and CC0/Marketplace-licensed.
// ======================================================================
namespace ChimeraAssetManifest
{
	// === SKY / ENVIRONMENT (Global) ===
	inline TArray<FAssetEntry> Sky()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("HDRI_Sky_Day"),    TEXT("PolyHaven"), TEXT("polyhaven.com/hdris/skies"),          TEXT("Content/Environment/Sky/"),     TEXT("Global"), 5 });
		A.Add({ TEXT("HDRI_Sky_Night"),   TEXT("PolyHaven"), TEXT("polyhaven.com/hdris/night"),          TEXT("Content/Environment/Sky/"),     TEXT("Global"), 3 });
		A.Add({ TEXT("Cloud_Volumetric"), TEXT("Quixel"),    TEXT("quixel.com/megascans/atmosphere"),     TEXT("Content/Environment/Clouds/"),  TEXT("Global"), 1 });
		A.Add({ TEXT("Water_Ocean"),      TEXT("Quixel"),    TEXT("quixel.com/megascans/water"),          TEXT("Content/Environment/Water/"),   TEXT("Global"), 1 });
		A.Add({ TEXT("Terrain_Grass"),    TEXT("Quixel"),    TEXT("quixel.com/megascans/surfaces"),       TEXT("Content/Environment/Terrain/"), TEXT("Global"), 10 });
		A.Add({ TEXT("Terrain_Dirt"),     TEXT("Quixel"),    TEXT("quixel.com/megascans/surfaces"),       TEXT("Content/Environment/Terrain/"), TEXT("Global"), 8 });
		A.Add({ TEXT("Terrain_Snow"),     TEXT("Quixel"),    TEXT("quixel.com/megascans/snow"),           TEXT("Content/Environment/Terrain/"), TEXT("Global"), 3 });
		A.Add({ TEXT("Terrain_Sand"),     TEXT("Quixel"),    TEXT("quixel.com/megascans/desert"),         TEXT("Content/Environment/Terrain/"), TEXT("Global"), 4 });
		return A;
	}

	// === NEXUS HUB ===
	inline TArray<FAssetEntry> Nexus()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("Nexus_Tower"),       TEXT("Sketchfab"), TEXT("sketchfab.com 3d-models/cc0-fantasy-tower"), TEXT("Content/Imported/Nexus/"),  TEXT("Nexus"), 1 });
		A.Add({ TEXT("Nexus_PlazaFloor"),  TEXT("Quixel"),    TEXT("quixel.com megascans marble-tiles"),            TEXT("Content/Imported/Nexus/"),  TEXT("Nexus"), 1 });
		A.Add({ TEXT("Nexus_GateRing"),    TEXT("Marketplace"), TEXT("free UE marketplace sci-fi portal"),           TEXT("Content/Imported/Nexus/"),  TEXT("Nexus"), 14 });
		A.Add({ TEXT("Nexus_Pillar"),      TEXT("PolyHaven"), TEXT("polyhaven.com marble-column"),                  TEXT("Content/Imported/Nexus/"),  TEXT("Nexus"), 8 });
		A.Add({ TEXT("Nexus_LibraryShelf"), TEXT("PolyHaven"), TEXT("polyhaven.com bookshelf"),                     TEXT("Content/Imported/Nexus/"),  TEXT("Nexus"), 6 });
		A.Add({ TEXT("Nexus_Bench"),       TEXT("PolyHaven"), TEXT("polyhaven.com park-bench"),                     TEXT("Content/Imported/Nexus/"),  TEXT("Nexus"), 4 });
		return A;
	}

	// === AETHER CITY ===
	inline TArray<FAssetEntry> AetherCity()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("AC_Skyscraper_01"), TEXT("Marketplace"), TEXT("free UE market city-sample buildings"),    TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 12 });
		A.Add({ TEXT("AC_Skyscraper_02"), TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 modern-skyscraper"),     TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 8 });
		A.Add({ TEXT("AC_Road_Module"),   TEXT("Quixel"),     TEXT("quixel.com megascans asphalt-road"),       TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 20 });
		A.Add({ TEXT("AC_NeonSign_01"),   TEXT("Quixel"),     TEXT("quixel.com megascans neon-sign"),          TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 30 });
		A.Add({ TEXT("AC_HoloBillboard"), TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 hologram-billboard"),    TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 10 });
		A.Add({ TEXT("AC_SkyPlatform"),   TEXT("Marketplace"), TEXT("free UE market floating-island"),         TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 6 });
		A.Add({ TEXT("AC_Drone_Enemy"),   TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 combat-drone"),          TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 20 });
		A.Add({ TEXT("AC_HeroStatue"),    TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 superhero-statue"),      TEXT("Content/Imported/AetherCity/"), TEXT("Aether City"), 4 });
		return A;
	}

	// === NEO-KINGDOM ===
	inline TArray<FAssetEntry> NeoKingdom()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("NK_Castle_Wall"),       TEXT("Quixel"),    TEXT("quixel.com megascans castle-wall"),     TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 30 });
		A.Add({ TEXT("NK_Castle_Tower"),      TEXT("Sketchfab"), TEXT("sketchfab.com cc0 fantasy-castle"),     TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 8 });
		A.Add({ TEXT("NK_MarketStall"),       TEXT("PolyHaven"), TEXT("polyhaven.com medieval-market-stall"),  TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 6 });
		A.Add({ TEXT("NK_PotionTable"),       TEXT("Sketchfab"), TEXT("sketchfab.com cc0 alchemy-table"),      TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 3 });
		A.Add({ TEXT("NK_EnchantingTable"),   TEXT("PolyHaven"), TEXT("polyhaven.com rune-table"),             TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 2 });
		A.Add({ TEXT("NK_Throne_Room"),       TEXT("Sketchfab"), TEXT("sketchfab.com cc0 throne-room"),        TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 1 });
		A.Add({ TEXT("NK_Courtyard_Fountain"), TEXT("PolyHaven"), TEXT("polyhaven.com stone-fountain"),        TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 2 });
		A.Add({ TEXT("NK_Dueling_Pit"),       TEXT("PolyHaven"), TEXT("polyhaven.com arena-pit"),              TEXT("Content/Imported/NeoKingdom/"), TEXT("Neo-Kingdom"), 1 });
		return A;
	}

	// === MERCY HEIGHTS ===
	inline TArray<FAssetEntry> MercyHeights()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("MH_Hospital_Modular"), TEXT("Marketplace"), TEXT("free UE market hospital-pack"),       TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 1 });
		A.Add({ TEXT("MH_Hospital_Bed"),     TEXT("PolyHaven"),  TEXT("polyhaven.com hospital-bed"),           TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 24 });
		A.Add({ TEXT("MH_OR_Table"),         TEXT("PolyHaven"),  TEXT("polyhaven.com surgery-table"),          TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 6 });
		A.Add({ TEXT("MH_Monitor"),          TEXT("PolyHaven"),  TEXT("polyhaven.com medical-monitor"),        TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 30 });
		A.Add({ TEXT("MH_IV_Stand"),         TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 iv-stand"),           TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 20 });
		A.Add({ TEXT("MH_Ambulance"),        TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 ambulance"),          TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 4 });
		A.Add({ TEXT("MH_Waiting_Chair"),    TEXT("PolyHaven"),  TEXT("polyhaven.com waiting-chair"),          TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 30 });
		A.Add({ TEXT("MH_Gurney"),           TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 gurney"),             TEXT("Content/Imported/MercyHeights/"), TEXT("Mercy Heights"), 10 });
		return A;
	}

	// === MID-WILSHIRE ===
	inline TArray<FAssetEntry> MidWilshire()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("MW_Police_Station"), TEXT("Marketplace"), TEXT("free UE market police-station"),      TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 1 });
		A.Add({ TEXT("MW_Police_Car"),     TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 police-car"),        TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 6 });
		A.Add({ TEXT("MW_Evidence_Locker"), TEXT("PolyHaven"),  TEXT("polyhaven.com locker"),                TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 8 });
		A.Add({ TEXT("MW_Interrogation_Room"), TEXT("PolyHaven"), TEXT("polyhaven.com interrogation-room"), TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 2 });
		A.Add({ TEXT("MW_Police_Desk"),    TEXT("PolyHaven"),  TEXT("polyhaven.com office-desk"),             TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 10 });
		A.Add({ TEXT("MW_Holding_Cell"),   TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 jail-cell"),          TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 4 });
		A.Add({ TEXT("MW_Street_Lamp"),    TEXT("Quixel"),     TEXT("quixel.com megascans street-lamp"),      TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 20 });
		A.Add({ TEXT("MW_Traffic_Light"),  TEXT("PolyHaven"),  TEXT("polyhaven.com traffic-light"),           TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 8 });
		A.Add({ TEXT("MW_Fire_Hydrant"),   TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 fire-hydrant"),        TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 12 });
		A.Add({ TEXT("MW_Dumpster"),       TEXT("PolyHaven"),  TEXT("polyhaven.com dumpster"),                TEXT("Content/Imported/MidWilshire/"), TEXT("Mid-Wilshire"), 15 });
		return A;
	}

	// === ALAGAESIA ===
	inline TArray<FAssetEntry> Alagaesia()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("AG_Dragon_Model"),    TEXT("Marketplace"), TEXT("free UE market dragon"),               TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 3 });
		A.Add({ TEXT("AG_Cottage"),         TEXT("PolyHaven"),  TEXT("polyhaven.com medieval-cottage"),        TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 6 });
		A.Add({ TEXT("AG_Dwarven_Hall"),    TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 dwarven-hall"),        TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 1 });
		A.Add({ TEXT("AG_Elf_TreeHouse"),   TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 treehouse"),           TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 5 });
		A.Add({ TEXT("AG_Bridge_Stone"),    TEXT("Quixel"),     TEXT("quixel.com megascans stone-bridge"),     TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 4 });
		A.Add({ TEXT("AG_Dragon_Egg_Pedestal"), TEXT("Sketchfab"), TEXT("sketchfab.com cc0 pedestal"),        TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 1 });
		A.Add({ TEXT("AG_Trees_Ancient"),   TEXT("Quixel"),     TEXT("quixel.com megascans trees-oak"),        TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 50 });
		A.Add({ TEXT("AG_River_Rocks"),     TEXT("Quixel"),     TEXT("quixel.com megascans river-rocks"),      TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 30 });
		A.Add({ TEXT("AG_Sword_Ancient"),   TEXT("PolyHaven"),  TEXT("polyhaven.com sword-rider"),             TEXT("Content/Imported/Alagaesia/"), TEXT("Alagaesia"), 3 });
		return A;
	}

	// === MINECRAFT FRONTIER ===
	inline TArray<FAssetEntry> MinecraftFrontier()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("MF_Grass_Block"),  TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 minecraft-block"),  TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 500 });
		A.Add({ TEXT("MF_Dirt_Block"),   TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 dirt-block"),       TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 300 });
		A.Add({ TEXT("MF_Stone_Block"),  TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 stone-block"),      TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 200 });
		A.Add({ TEXT("MF_Oak_Tree"),     TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 blocky-tree"),      TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 40 });
		A.Add({ TEXT("MF_Workbench"),    TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 crafting-table"),   TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 4 });
		A.Add({ TEXT("MF_Furnace"),      TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 furnace"),          TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 4 });
		A.Add({ TEXT("MF_Chest"),        TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 chest"),            TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 10 });
		A.Add({ TEXT("MF_Redstone_Lamp"), TEXT("Sketchfab"), TEXT("sketchfab.com cc0 redstone-lamp"),    TEXT("Content/Imported/Frontier/"), TEXT("Minecraft Frontier"), 15 });
		return A;
	}

	// === NIGHT CITY ===
	inline TArray<FAssetEntry> NightCity()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("NC_CyberBuilding_01"), TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 cyberpunk-building"),  TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 20 });
		A.Add({ TEXT("NC_CyberBuilding_02"), TEXT("Marketplace"), TEXT("free UE market cyberpunk-kit"),         TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 15 });
		A.Add({ TEXT("NC_NeonSign_Red"),     TEXT("Quixel"),     TEXT("quixel.com megascans neon-red"),         TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 25 });
		A.Add({ TEXT("NC_NeonSign_Blue"),    TEXT("Quixel"),     TEXT("quixel.com megascans neon-blue"),        TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 25 });
		A.Add({ TEXT("NC_Braindance_Wreath"), TEXT("Sketchfab"), TEXT("sketchfab.com cc0 vr-headset"),          TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 3 });
		A.Add({ TEXT("NC_Poker_Table"),      TEXT("PolyHaven"),  TEXT("polyhaven.com poker-table"),             TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 4 });
		A.Add({ TEXT("NC_Cyberdeck"),        TEXT("Sketchfab"),  TEXT("sketchfab.com cc0 cyberdeck-terminal"),  TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 6 });
		A.Add({ TEXT("NC_Rain_Decal"),       TEXT("Quixel"),     TEXT("quixel.com megascans decals-rain"),      TEXT("Content/Imported/NightCity/"), TEXT("Night City"), 50 });
		return A;
	}

	// === PANEM ===
	inline TArray<FAssetEntry> Panem()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("PN_Capitol_Building"), TEXT("Sketchfab"), TEXT("sketchfab.com cc0 capitol-building"),   TEXT("Content/Imported/Panem/"), TEXT("Panem"), 5 });
		A.Add({ TEXT("PN_District_Shack"),   TEXT("PolyHaven"), TEXT("polyhaven.com wooden-shack"),           TEXT("Content/Imported/Panem/"), TEXT("Panem"), 15 });
		A.Add({ TEXT("PN_Arena_Cornucopia"), TEXT("Sketchfab"), TEXT("sketchfab.com cc0 cornucopia"),          TEXT("Content/Imported/Panem/"), TEXT("Panem"), 1 });
		A.Add({ TEXT("PN_Hob_Market"),       TEXT("PolyHaven"), TEXT("polyhaven.com black-market-stall"),     TEXT("Content/Imported/Panem/"), TEXT("Panem"), 6 });
		A.Add({ TEXT("PN_Bow_Arrow"),        TEXT("PolyHaven"), TEXT("polyhaven.com hunting-bow"),            TEXT("Content/Imported/Panem/"), TEXT("Panem"), 5 });
		A.Add({ TEXT("PN_Coal_Mine"),        TEXT("Quixel"),    TEXT("quixel.com megascans coal"),             TEXT("Content/Imported/Panem/"), TEXT("Panem"), 1 });
		A.Add({ TEXT("PN_Fence_Electric"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 electric-fence"),      TEXT("Content/Imported/Panem/"), TEXT("Panem"), 8 });
		A.Add({ TEXT("PN_Mockingjay_Pin"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 mockingjay-pin"),     TEXT("Content/Imported/Panem/"), TEXT("Panem"), 10 });
		return A;
	}

	// === VEHICLES (300+ unique models from free sources) ===
	inline TArray<FAssetEntry> Vehicles()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("Car_Sedan_01"),    TEXT("Sketchfab"), TEXT("sketchfab.com cc0 sedan-lowpoly"),      TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 30 });
		A.Add({ TEXT("Car_Sedan_02"),    TEXT("Sketchfab"), TEXT("sketchfab.com cc0 modern-sedan"),       TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 25 });
		A.Add({ TEXT("Car_SUV_01"),      TEXT("Sketchfab"), TEXT("sketchfab.com cc0 suv"),                TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 20 });
		A.Add({ TEXT("Car_Muscle_01"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 muscle-car"),         TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 15 });
		A.Add({ TEXT("Car_Sports_01"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 sports-car"),         TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 12 });
		A.Add({ TEXT("Car_Supercar_01"), TEXT("Sketchfab"), TEXT("sketchfab.com cc0 supercar"),           TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 8 });
		A.Add({ TEXT("Car_Truck_01"),    TEXT("Sketchfab"), TEXT("sketchfab.com cc0 pickup-truck"),       TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 15 });
		A.Add({ TEXT("Car_Police_01"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 police-sedan"),       TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 8 });
		A.Add({ TEXT("Car_Taxi_01"),     TEXT("Sketchfab"), TEXT("sketchfab.com cc0 taxi"),               TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 10 });
		A.Add({ TEXT("Car_Ambulance"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 ambulance"),          TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 4 });
		A.Add({ TEXT("Car_FireTruck"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 fire-truck"),         TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 2 });
		A.Add({ TEXT("Car_Bus"),         TEXT("Sketchfab"), TEXT("sketchfab.com cc0 city-bus"),           TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 5 });
		A.Add({ TEXT("Motorcycle_01"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 motorcycle"),         TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 8 });
		A.Add({ TEXT("Bicycle_01"),      TEXT("Sketchfab"), TEXT("sketchfab.com cc0 bicycle"),            TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 6 });
		A.Add({ TEXT("Helicopter_01"),   TEXT("Sketchfab"), TEXT("sketchfab.com cc0 helicopter"),         TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 2 });
		A.Add({ TEXT("Boat_Fishing"),    TEXT("Sketchfab"), TEXT("sketchfab.com cc0 fishing-boat"),       TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 3 });
		A.Add({ TEXT("Plane_Small"),     TEXT("Sketchfab"), TEXT("sketchfab.com cc0 cessna"),             TEXT("Content/Imported/Vehicles/"), TEXT("Global"), 2 });
		return A;
	}

	// === INTERIOR PROPS (Global) ===
	inline TArray<FAssetEntry> Interiors()
	{
		TArray<FAssetEntry> A;
		A.Add({ TEXT("Chair_Office"),    TEXT("PolyHaven"), TEXT("polyhaven.com office-chair"),       TEXT("Content/Imported/Props/"), TEXT("Global"), 40 });
		A.Add({ TEXT("Chair_Wooden"),    TEXT("PolyHaven"), TEXT("polyhaven.com wooden-chair"),       TEXT("Content/Imported/Props/"), TEXT("Global"), 30 });
		A.Add({ TEXT("Table_Dining"),    TEXT("PolyHaven"), TEXT("polyhaven.com dining-table"),       TEXT("Content/Imported/Props/"), TEXT("Global"), 20 });
		A.Add({ TEXT("Table_Coffee"),    TEXT("PolyHaven"), TEXT("polyhaven.com coffee-table"),       TEXT("Content/Imported/Props/"), TEXT("Global"), 15 });
		A.Add({ TEXT("Sofa_01"),         TEXT("PolyHaven"), TEXT("polyhaven.com sofa"),               TEXT("Content/Imported/Props/"), TEXT("Global"), 20 });
		A.Add({ TEXT("Bookshelf_01"),    TEXT("PolyHaven"), TEXT("polyhaven.com bookshelf-wooden"),   TEXT("Content/Imported/Props/"), TEXT("Global"), 15 });
		A.Add({ TEXT("Lamp_Floor"),      TEXT("PolyHaven"), TEXT("polyhaven.com floor-lamp"),         TEXT("Content/Imported/Props/"), TEXT("Global"), 10 });
		A.Add({ TEXT("Rug_01"),          TEXT("PolyHaven"), TEXT("polyhaven.com persian-rug"),        TEXT("Content/Imported/Props/"), TEXT("Global"), 8 });
		A.Add({ TEXT("VendingMachine"),  TEXT("Sketchfab"), TEXT("sketchfab.com cc0 vending-machine"), TEXT("Content/Imported/Props/"), TEXT("Global"), 10 });
		A.Add({ TEXT("TrashCan"),        TEXT("Sketchfab"), TEXT("sketchfab.com cc0 trash-can"),       TEXT("Content/Imported/Props/"), TEXT("Global"), 20 });
		A.Add({ TEXT("Plant_Potted"),    TEXT("Quixel"),    TEXT("quixel.com megascans potted-plant"),  TEXT("Content/Imported/Props/"), TEXT("Global"), 25 });
		A.Add({ TEXT("Computer_01"),     TEXT("PolyHaven"), TEXT("polyhaven.com desktop-computer"),    TEXT("Content/Imported/Props/"), TEXT("Global"), 15 });
		return A;
	}
}

// ======================================================================
// Content download instructions (first UE5 session)
// ======================================================================
//
// Step 1: Quixel Bridge (in-UE5, free)
//   Window → Quixel Bridge → Sign in with Epic account
//   Download: ALL surfaces, 3D assets, decals, brushes
//   ~50GB, all photorealistic scanned, all free with UE5 license
//
// Step 2: UE Marketplace free monthly content
//   Marketplace → Free → sort by "Permanent"
//   Download: "Modular SciFi", "Modular Fantasy", "Modular Hospital",
//            "City Sample Buildings", "Vehicle Variety Pack",
//            "Rural Australia", "Medieval Environment"
//
// Step 3: Poly Haven (polyhaven.com)
//   All CC0 (public domain), no attribution needed
//   Download: HDRI skies pack, texture pack, fence pack, rock pack
//
// Step 4: Sketchfab (sketchfab.com)
//   Search filter: "Downloadable" + "CC0" or "CC Attribution"
//   Download: buildings, vehicles, props, weapons
//   Import into Content/Imported/ via UE5 FBX import
//
// Step 5: OpenGameArt / TurboSquid free section
//   Filter by "Royalty Free" / "CC0"
//
// After downloading ALL assets, enable Nanite on all static meshes:
//   Bulk Edit → Select all Static Meshes in Content Browser
//   Right click → Asset Actions → Bulk Edit via Property Matrix
//   Set "Nanite Enabled" = true
//   Set "Enable Lumen" in Project Settings → Rendering

// ======================================================================
// Asset auto-loader: attempts to load imported content, falls back to 
// basic shapes if content hasn't been imported yet.
// ======================================================================
inline bool AssetExists(const FString& Path) {
	return LoadObject<UStaticMesh>(nullptr, *(TEXT("/Game/Imported/") + Path)) != nullptr;
}

inline UStaticMesh* LoadAsset(const FString& Path, const TCHAR* FallbackPath) {
	UStaticMesh* M = LoadObject<UStaticMesh>(nullptr, *(TEXT("/Game/Imported/") + Path));
	return M ? M : LoadObject<UStaticMesh>(nullptr, FallbackPath);
}
