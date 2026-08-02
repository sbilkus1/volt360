#include "Core/ChimeraVehicleSpawns.h"
#include "CHIMERA.h"

#define S(id,ring,zone,rarity,upg) { FMVehicleSpawn S; S.VehicleId=id; S.Ring=ring; S.SubZone=zone; S.Rarity=rarity; S.UpgradeLevel=upg; Spawns.Add(S); }

void UVehicleSpawnSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedSpawns();
}

void UVehicleSpawnSystem::SeedSpawns()
{
	// === NEXUS — rare supercars, diplomats ===
	S("s_class", "Nexus", "Plaza", ESpawnRarity::Uncommon, 0);
	S("phantom", "Nexus", "Plaza", ESpawnRarity::Rare, 0);
	S("chiron", "Nexus", "Council", ESpawnRarity::Legendary, 0);
	S("century", "Nexus", "Plaza", ESpawnRarity::Rare, 1);

	// === AETHER CITY — supers, cyber, neon ===
	S("aventador_svj", "Aether City", "Skyline", ESpawnRarity::Rare, 1);
	S("huracan_performante", "Aether City", "Downtown", ESpawnRarity::Uncommon, 0);
	S("tesla_roadster", "Aether City", "Skyline", ESpawnRarity::Uncommon, 1);
	S("i8", "Aether City", "Downtown", ESpawnRarity::Common, 0);
	S("neon_cyber", "Aether City", "Underground", ESpawnRarity::Rare, 2);
	S("918_spyder", "Aether City", "Hero Lair", ESpawnRarity::Legendary, 3);
	S("rimac_nevera", "Aether City", "Skyline", ESpawnRarity::Legendary, 2);

	// === NEO-KINGDOM — classics, nobles, royal ===
	S("db5", "Neo-Kingdom", "Royal Court", ESpawnRarity::Rare, 0);
	S("phantom_ii", "Neo-Kingdom", "Royal Court", ESpawnRarity::Legendary, 0);
	S("e_type", "Neo-Kingdom", "Countryside", ESpawnRarity::Uncommon, 1);
	S("x19", "Neo-Kingdom", "Countryside", ESpawnRarity::Common, 0);
	S("miura_sv", "Neo-Kingdom", "Royal Stables", ESpawnRarity::Legendary, 2);

	// === MERCY HEIGHTS — practical, doctors, ambulances ===
	S("e_class", "Mercy Heights", "Hospital", ESpawnRarity::Common, 0);
	S("x5", "Mercy Heights", "Hospital", ESpawnRarity::Common, 1);
	S("volvo_v90", "Mercy Heights", "Hospital", ESpawnRarity::Common, 0);
	S("range_rover", "Mercy Heights", "VIP Wing", ESpawnRarity::Uncommon, 1);

	// === MID-WILSHIRE — muscle, police, street ===
	S("charger_hellcat", "Mid-Wilshire", "Precinct", ESpawnRarity::Uncommon, 1);
	S("challenger_demon", "Mid-Wilshire", "Drag Strip", ESpawnRarity::Rare, 2);
	S("camaro_zl1", "Mid-Wilshire", "Downtown", ESpawnRarity::Common, 0);
	S("crown_vic", "Mid-Wilshire", "Precinct", ESpawnRarity::Common, 3);
	S("impala_67", "Mid-Wilshire", "Lowrider Meet", ESpawnRarity::Rare, 3);
	S("sierra_rs500", "Mid-Wilshire", "Warehouse", ESpawnRarity::Legendary, 4);

	// === ALAGAESIA — old, rugged, dragon-scale ===
	S("hilux", "Alagaesia", "Wilds", ESpawnRarity::Common, 0);
	S("defender", "Alagaesia", "Village", ESpawnRarity::Common, 1);
	S("unimog", "Alagaesia", "Mountains", ESpawnRarity::Uncommon, 0);
	S("959_dakar", "Alagaesia", "Dragon Peak", ESpawnRarity::Legendary, 4);
	S("lm002", "Alagaesia", "Wilds", ESpawnRarity::Rare, 2);

	// === MINECRAFT FRONTIER — voxel, blocky, off-road ===
	S("jimny", "Minecraft Frontier", "Village", ESpawnRarity::Common, 0);
	S("cybertruck", "Minecraft Frontier", "Desert", ESpawnRarity::Uncommon, 1);
	S("tacoma_trd", "Minecraft Frontier", "Caves", ESpawnRarity::Uncommon, 0);
	S("g63_6x6", "Minecraft Frontier", "Deep Mine", ESpawnRarity::Legendary, 3);

	// === GRAND COLISEUM — race cars, F1, track weapons ===
	S("911_gt3_rs_992", "Grand Coliseum", "Pits", ESpawnRarity::Rare, 2);
	S("f40", "Grand Coliseum", "Paddock", ESpawnRarity::Legendary, 0);
	S("senna", "Grand Coliseum", "Paddock", ESpawnRarity::Legendary, 1);
	S("gt3_rs", "Grand Coliseum", "Track", ESpawnRarity::Rare, 1);
	S("focus_rs", "Grand Coliseum", "Parking", ESpawnRarity::Common, 0);
	S("m4_gts", "Grand Coliseum", "Pits", ESpawnRarity::Rare, 2);
	S("f1", "Grand Coliseum", "Museum", ESpawnRarity::Legendary, 4);

	// === NIGHT CITY — dark, cyber, illegal ===
	S("gtr_r34", "Night City", "Underground", ESpawnRarity::Uncommon, 1);
	S("supra_mk4", "Night City", "Underground", ESpawnRarity::Uncommon, 2);
	S("rx7_fd", "Night City", "Highway", ESpawnRarity::Uncommon, 1);
	S("silvia_s15", "Night City", "Docks", ESpawnRarity::Common, 0);
	S("slr_mclaren", "Night City", "Afterlife Club", ESpawnRarity::Legendary, 3);
	S("venom_gt", "Night City", "Black Market", ESpawnRarity::Legendary, 4);

	// === SEATTLE — practical, doctors, rain-resistant ===
	S("outback", "Seattle", "Hospital", ESpawnRarity::Common, 0);
	S("prius_prime", "Seattle", "Hospital", ESpawnRarity::Common, 0);
	S("volvo_xc90", "Seattle", "Hospital", ESpawnRarity::Common, 1);
	S("911_turbo_997", "Seattle", "Waterfront", ESpawnRarity::Rare, 1);

	// === CHICAGO — muscle, wind, Dauntless ===
	S("mustang_dark_horse", "Chicago", "Dauntless HQ", ESpawnRarity::Uncommon, 1);
	S("viper_acr", "Chicago", "Downtown", ESpawnRarity::Rare, 0);
	S("wrangler_392", "Chicago", "Fence", ESpawnRarity::Uncommon, 1);
	S("gt500_super_snake", "Chicago", "Abandoned Mill", ESpawnRarity::Legendary, 3);

	// === CHASTAIN PARK — Atlanta luxury, doctors ===
	S("escalade_v", "Chastain Park", "VIP Parking", ESpawnRarity::Common, 1);
	S("s65_amg", "Chastain Park", "Admin Wing", ESpawnRarity::Uncommon, 2);
	S("cullinan", "Chastain Park", "VIP Parking", ESpawnRarity::Rare, 0);

	// === CLEVELEYS — British, coastal, seaside ===
	S("mini_cooper_s", "Cleveleys", "Seafront", ESpawnRarity::Common, 0);
	S("fiesta_st", "Cleveleys", "High Street", ESpawnRarity::Common, 0);
	S("defender_90", "Cleveleys", "Farm Track", ESpawnRarity::Uncommon, 1);
	S("db5", "Cleveleys", "Promenade", ESpawnRarity::Legendary, 2);
	S("morris_minor", "Cleveleys", "Beach Road", ESpawnRarity::Rare, 1);

	// === VERNET-LES-BAINS — French, mountain, classic ===
	S("alpine_a110", "VernetLesBains", "Mountain Pass", ESpawnRarity::Uncommon, 0);
	S("ds_23", "VernetLesBains", "Town Square", ESpawnRarity::Rare, 1);
	S("2cv", "VernetLesBains", "Countryside", ESpawnRarity::Common, 0);
	S("bugatti_type35", "VernetLesBains", "Chateau", ESpawnRarity::Legendary, 4);

	// === PANEM — utilitarian, Capitol excess ===
	S("beatle", "Panem", "District 12", ESpawnRarity::Common, 0);
	S("tundra_trd", "Panem", "District 7", ESpawnRarity::Uncommon, 0);
	S("chiron", "Panem", "Capitol", ESpawnRarity::Legendary, 1);
	S("rolls_royce_ghost", "Panem", "Capitol", ESpawnRarity::Rare, 2);
	S("dmax_at35", "Panem", "District 10", ESpawnRarity::Common, 0);

	// === BARN FINDS (legendary, hidden, with hints) ===
	FMVehicleSpawn BF;
	BF.VehicleId = TEXT("f40"); BF.Ring = TEXT("Alagaesia"); BF.SubZone = TEXT("Abandoned Barn");
	BF.Rarity = ESpawnRarity::Legendary; BF.UpgradeLevel = 4;
	BF.UniqueName = TEXT("The Dragon Rider's F40"); BF.Hint = TEXT("Where dragons once slept, a red horse waits in a crumbling stable."); Spawns.Add(BF);

	BF.VehicleId = TEXT("250_gto"); BF.Ring = TEXT("Neo-Kingdom"); BF.SubZone = TEXT("Forgotten Crypt");
	BF.Rarity = ESpawnRarity::Legendary; BF.UpgradeLevel = 4;
	BF.UniqueName = TEXT("The King's 250 GTO"); BF.Hint = TEXT("A king buried with his greatest treasure. Follow the royal crypts."); Spawns.Add(BF);

	BF.VehicleId = TEXT("959_sport"); BF.Ring = TEXT("Minecraft Frontier"); BF.SubZone = TEXT("Voxel Cave");
	BF.Rarity = ESpawnRarity::Legendary; BF.UpgradeLevel = 4;
	BF.UniqueName = TEXT("Blocky 959"); BF.Hint = TEXT("A German ghost in a voxel cave. Look for the glow of the headlights between blocks."); Spawns.Add(BF);

	BF.VehicleId = TEXT("countach_5000qv"); BF.Ring = TEXT("Night City"); BF.SubZone = TEXT("Derelict Warehouse");
	BF.Rarity = ESpawnRarity::Legendary; BF.UpgradeLevel = 4;
	BF.UniqueName = TEXT("Neon Countach"); BF.Hint = TEXT("An 80s icon rusting in a Night City warehouse. Neon lights still flicker around it."); Spawns.Add(BF);

	BF.VehicleId = TEXT("eleanor"); BF.Ring = TEXT("Mid-Wilshire"); BF.SubZone = TEXT("Chop Shop Lockup");
	BF.Rarity = ESpawnRarity::Legendary; BF.UpgradeLevel = 4;
	BF.UniqueName = TEXT("Eleanor"); BF.Hint = TEXT("The unicorn. 1967 Shelby GT500. The last one was spotted in a Mid-Wilshire lockup."); Spawns.Add(BF);
}

int32 UVehicleSpawnSystem::RarityWeight(ESpawnRarity R)
{
	switch (R) { case ESpawnRarity::Common: return 70; case ESpawnRarity::Uncommon: return 20;
		case ESpawnRarity::Rare: return 8; case ESpawnRarity::Legendary: return 2; default: return 0; }
}

TArray<FString> UVehicleSpawnSystem::GetSpawnsForRing(const FString& Ring, ESpawnRarity MaxRarity) const
{
	TArray<FString> Result;
	for (const FMVehicleSpawn& S : Spawns)
	{
		if (S.Ring == Ring || S.Ring == TEXT("All"))
			if ((int32)S.Rarity <= (int32)MaxRarity)
				Result.Add(S.VehicleId);
	}
	return Result;
}

FString UVehicleSpawnSystem::GetRandomSpawn(const FString& Ring) const
{
	TArray<FMVehicleSpawn> Pool;
	for (const FMVehicleSpawn& S : Spawns)
	{
		if (S.Ring == Ring || S.Ring == TEXT("All"))
		{
			int32 Weight = RarityWeight(S.Rarity);
			for (int32 i = 0; i < Weight; ++i) Pool.Add(S);
		}
	}
	if (Pool.Num() == 0) return TEXT("corolla");
	return Pool[FMath::RandRange(0, Pool.Num() - 1)].VehicleId;
}

FMVehicleSpawn UVehicleSpawnSystem::GetBarnFind(const FString& Ring) const
{
	for (const FMVehicleSpawn& S : Spawns)
		if (S.Ring == Ring && S.Rarity == ESpawnRarity::Legendary && S.UpgradeLevel >= 4 && !S.UniqueName.IsEmpty())
			return S;
	FMVehicleSpawn Empty; return Empty;
}
