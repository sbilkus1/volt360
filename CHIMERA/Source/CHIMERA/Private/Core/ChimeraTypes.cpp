#include "Core/ChimeraTypes.h"

static const TArray<FRingDef> BuildRings()
{
	TArray<FRingDef> Rings;
	FRingDef R;

	R.Ring = ERing::Nexus; R.Name = TEXT("The Nexus"); R.Genre = TEXT("Hub / Narrative Core");
	R.Source = TEXT("Original"); R.SizeKm2 = 4.f;
	R.Description = TEXT("Nexus Tower dungeon, Reality Anchors, Library of Lost Worlds, the Council Chamber.");
	R.ZoneAnchor = FVector(0.f, 0.f, 100.f); Rings.Add(R);

	R.Ring = ERing::AetherCity; R.Name = TEXT("Aether City"); R.Genre = TEXT("Superhero Sci-Fi");
	R.Source = TEXT("Avengers + Cyberpunk 2077"); R.SizeKm2 = 180.f;
	R.Description = TEXT("18 power archetypes, cyberware, quickhacks, kaiju events every 2h, hero/villain meter.");
	R.ZoneAnchor = FVector(50000.f, 0.f, 100.f); Rings.Add(R);

	R.Ring = ERing::NeoKingdom; R.Name = TEXT("Neo-Kingdom"); R.Genre = TEXT("Magical Fantasy Academy");
	R.Source = TEXT("Descendants 3"); R.SizeKm2 = 25.f;
	R.Description = TEXT("Auradon Prep Reborn, house system, musical numbers, spell creation, royal court politics.");
	R.ZoneAnchor = FVector(0.f, 50000.f, 100.f); Rings.Add(R);

	R.Ring = ERing::MercyHeights; R.Name = TEXT("Mercy Heights"); R.Genre = TEXT("Medical Management Drama");
	R.Source = TEXT("Two Point Hospital + Grey's Anatomy"); R.SizeKm2 = 15.f;
	R.Description = TEXT("Hospital management, weird illnesses, surgery minigame, relationship drama, M&M conferences.");
	R.ZoneAnchor = FVector(-50000.f, 0.f, 100.f); Rings.Add(R);

	R.Ring = ERing::MidWilshire; R.Name = TEXT("Mid-Wilshire"); R.Genre = TEXT("Crime / Police Procedural");
	R.Source = TEXT("The Rookie + GTA V"); R.SizeKm2 = 120.f;
	R.Description = TEXT("NPD career, body cams, use-of-force continuum, heists, wanted levels, 600+ vehicles.");
	R.ZoneAnchor = FVector(0.f, -50000.f, 100.f); Rings.Add(R);

	R.Ring = ERing::Alagaesia; R.Name = TEXT("Alagaesia Wilds"); R.Genre = TEXT("High Fantasy Dragon Realm");
	R.Source = TEXT("Eragon"); R.SizeKm2 = 400.f;
	R.Description = TEXT("Dragon bonding, Ancient Language magic, True Names, races, aerial combat.");
	R.ZoneAnchor = FVector(35355.f, 35355.f, 100.f); Rings.Add(R);

	R.Ring = ERing::MinecraftFrontier; R.Name = TEXT("Minecraft Frontier"); R.Genre = TEXT("Voxel Sandbox");
	R.Source = TEXT("Minecraft"); R.SizeKm2 = 350.f;
	R.Description = TEXT("Full voxel ring, redstone, enchanting crossovers, building contests, netherite imports.");
	R.ZoneAnchor = FVector(-35355.f, 35355.f, 100.f); Rings.Add(R);

	R.Ring = ERing::GrandColiseum; R.Name = TEXT("Grand Coliseum"); R.Genre = TEXT("Sports & Racing Megaplex");
	R.Source = TEXT("FIFA + F1 2025"); R.SizeKm2 = 80.f;
	R.Description = TEXT("Full F1 race weekend, FIFA career + UT, fantasy circuits, VAR reviews.");
	R.ZoneAnchor = FVector(35355.f, -35355.f, 100.f); Rings.Add(R);

	R.Ring = ERing::NightCity; R.Name = TEXT("Night City Expanse"); R.Genre = TEXT("Futuristic Open World");
	R.Source = TEXT("Cyberpunk 2077"); R.SizeKm2 = 200.f;
	R.Description = TEXT("Fixers, braindance, 15 gangs, netrunning, Blackwall endgame.");
	R.ZoneAnchor = FVector(-35355.f, -35355.f, 100.f); Rings.Add(R);

	// --- Cinematic universe expansions ---
	R.Ring = ERing::Seattle; R.Name = TEXT("Seattle"); R.Genre = TEXT("Medical Drama Hub");
	R.Source = TEXT("Grey's Anatomy"); R.SizeKm2 = 25.f;
	R.Description = TEXT("Grey-Sloan Memorial Hospital. Meredith Grey's legacy, the OR board, on-call rooms, the catwalk, the lobby where every intern started.");
	R.ZoneAnchor = FVector(100000.f, 0.f, 100.f); Rings.Add(R);

	R.Ring = ERing::Chicago; R.Name = TEXT("Chicago"); R.Genre = TEXT("Emergency Medicine");
	R.Source = TEXT("Chicago Med"); R.SizeKm2 = 20.f;
	R.Description = TEXT("Gaffney Chicago Medical Center. ED chaos, Dr. Charles' psychiatry wing, the rooftop where everyone hides, Goodwin's corner office.");
	R.ZoneAnchor = FVector(0.f, -100000.f, 100.f); Rings.Add(R);

	R.Ring = ERing::ChastainPark; R.Name = TEXT("Chastain Park"); R.Genre = TEXT("Medical Drama");
	R.Source = TEXT("The Resident"); R.SizeKm2 = 18.f;
	R.Description = TEXT("Chastain Park Memorial Hospital, Atlanta. Conrad's cowboy medicine, Bell's redemption arc, the Raptor's surgical theatre, Kit Voss's admin wing.");
	R.ZoneAnchor = FVector(-100000.f, 0.f, 100.f); Rings.Add(R);

	R.Ring = ERing::Cleveleys; R.Name = TEXT("Cleveleys"); R.Genre = TEXT("Seaside Town Drama");
	R.Source = TEXT("Cleveleys, Lancashire, UK (53.881°N, 3.040°W)"); R.SizeKm2 = 3.f;
	R.Description = TEXT("Postcode FY5. Population 10,754. Victoria Road, Blackpool Tramway, Rossall School, Riverdance ferry wreck. Star Wars: Andor filmed here (planet Niamos).");
	R.ZoneAnchor = FVector(0.f, 100000.f, 100.f); Rings.Add(R);

	R.Ring = ERing::VernetLesBains; R.Name = TEXT("Vernet-les-Bains"); R.Genre = TEXT("Pyrenees Spa Sanctuary");
	R.Source = TEXT("Vernet-les-Bains, France (42.55°N, 2.3886°E)"); R.SizeKm2 = 16.f;
	R.Description = TEXT("Population 1,498. Postcode 66820. Canigó mountain (2,785m). Thermal springs, 300 sunny days/year. Rudyard Kipling visited 1910-1926. Yellow train — highest station in France. Entente Cordiale monument (only one in France). St George's Anglican Church with first change ringing bells in France. River Cady, Kipling bridge.");
	R.ZoneAnchor = FVector(70711.f, -70711.f, 100.f); Rings.Add(R);

	R.Ring = ERing::Panem; R.Name = TEXT("Panem"); R.Genre = TEXT("Post-Apocalyptic Dystopia");
	R.Source = TEXT("The Hunger Games"); R.SizeKm2 = 180.f;
	R.Description = TEXT("The Capitol and all 13 Districts. Training Centre, the arena, District 12's Seam and the Hob, District 13's bunker, the Victor's Village. May the odds be ever in your favour.");
	R.ZoneAnchor = FVector(100000.f, 100000.f, 100.f); Rings.Add(R);

	return Rings;
}

static const TArray<FSkillDef> BuildSkills()
{
	TArray<FSkillDef> Out;
	auto Add = [&](const char* Id, const char* Name, ESkillCategory C)
	{
		FSkillDef S; S.Id = FName(Id); S.Name = FString(Name); S.Category = C; Out.Add(S);
	};
	// Combat
	Add("Combat_Blades", "Blades", ESkillCategory::Combat);
	Add("Combat_Handguns", "Handguns", ESkillCategory::Combat);
	Add("Combat_Rifles", "Rifles", ESkillCategory::Combat);
	Add("Combat_Heavy", "Heavy Weapons", ESkillCategory::Combat);
	Add("Combat_Brawling", "Brawling", ESkillCategory::Combat);
	Add("Combat_Swordplay", "Swordplay", ESkillCategory::Combat);
	Add("Combat_DragonCombat", "Dragon Combat", ESkillCategory::Combat);
	Add("Combat_Stealth", "Stealth", ESkillCategory::Combat);
	Add("Combat_Assassination", "Assassination", ESkillCategory::Combat);
	// Magic
	Add("Magic_Fire", "Fire Magic", ESkillCategory::Magic);
	Add("Magic_Ice", "Ice Magic", ESkillCategory::Magic);
	Add("Magic_Lightning", "Lightning Magic", ESkillCategory::Magic);
	Add("Magic_Arcane", "Arcane Magic", ESkillCategory::Magic);
	Add("Magic_Necromancy", "Necromancy", ESkillCategory::Magic);
	Add("Magic_Alteration", "Alteration", ESkillCategory::Magic);
	Add("Magic_Enchanting", "Enchanting", ESkillCategory::Magic);
	Add("Magic_AncientLanguage", "Ancient Language", ESkillCategory::Magic);
	// Medical
	Add("Med_Diagnosis", "Diagnosis", ESkillCategory::Medical);
	Add("Med_Surgery", "Surgery", ESkillCategory::Medical);
	Add("Med_Pharmacology", "Pharmacology", ESkillCategory::Medical);
	Add("Med_Emergency", "Emergency Medicine", ESkillCategory::Medical);
	Add("Med_Psychiatry", "Psychiatry", ESkillCategory::Medical);
	Add("Med_DragonAnatomy", "Dragon Anatomy", ESkillCategory::Medical);
	Add("Med_Cyberware", "Cyberware Surgery", ESkillCategory::Medical);
	// Police
	Add("Pol_Firearms", "Firearms", ESkillCategory::Police);
	Add("Pol_Pursuit", "Pursuit Driving", ESkillCategory::Police);
	Add("Pol_Deescalation", "De-escalation", ESkillCategory::Police);
	Add("Pol_Investigation", "Investigation", ESkillCategory::Police);
	Add("Pol_Forensics", "Forensics", ESkillCategory::Police);
	Add("Pol_Interrogation", "Interrogation", ESkillCategory::Police);
	Add("Pol_SWAT", "SWAT Tactics", ESkillCategory::Police);
	// Sports
	Add("Spt_Dribbling", "Dribbling", ESkillCategory::Sports);
	Add("Spt_Shooting", "Shooting (soccer)", ESkillCategory::Sports);
	Add("Spt_Goalkeeping", "Goalkeeping", ESkillCategory::Sports);
	Add("Spt_RacingLine", "Racing Line", ESkillCategory::Sports);
	Add("Spt_Braking", "Braking", ESkillCategory::Sports);
	Add("Spt_TireManagement", "Tire Management", ESkillCategory::Sports);
	// Social
	Add("Soc_Persuasion", "Persuasion", ESkillCategory::Social);
	Add("Soc_Intimidation", "Intimidation", ESkillCategory::Social);
	Add("Soc_Seduction", "Seduction", ESkillCategory::Social);
	Add("Soc_Leadership", "Leadership", ESkillCategory::Social);
	Add("Soc_Performance", "Performance (music)", ESkillCategory::Social);
	Add("Soc_Dance", "Performance (dance)", ESkillCategory::Social);
	Add("Soc_Deception", "Deception", ESkillCategory::Social);
	Add("Soc_Empathy", "Empathy", ESkillCategory::Social);
	// Technical
	Add("Tech_Hacking", "Hacking", ESkillCategory::Technical);
	Add("Tech_Engineering", "Engineering", ESkillCategory::Technical);
	Add("Tech_Crafting", "Crafting", ESkillCategory::Technical);
	Add("Tech_Driving", "Driving", ESkillCategory::Technical);
	Add("Tech_DragonRiding", "Dragon Riding", ESkillCategory::Technical);
	Add("Tech_Redstone", "Redstone", ESkillCategory::Technical);
	Add("Tech_Building", "Building", ESkillCategory::Technical);
	Add("Tech_Farming", "Farming", ESkillCategory::Technical);
	Add("Tech_Survival", "Survival", ESkillCategory::Technical);
	// Business
	Add("Biz_Negotiation", "Negotiation", ESkillCategory::Business);
	Add("Biz_Management", "Management", ESkillCategory::Business);
	Add("Biz_Investment", "Investment", ESkillCategory::Business);
	Add("Biz_RealEstate", "Real Estate", ESkillCategory::Business);
	Add("Biz_Gambling", "Gambling", ESkillCategory::Business);
	return Out;
}

static const TArray<FString> BuildCurrencies()
{
	return { TEXT("Credits"), TEXT("Emeralds"), TEXT("Gold Crowns"), TEXT("Eddies") };
}

static const TArray<FString> BuildFactions()
{
	return { TEXT("Keepers"), TEXT("Hero Guild"), TEXT("Arasaka-Nex"), TEXT("NPD"),
		TEXT("Medical Board"), TEXT("Dragon Riders"), TEXT("Auradon Court"),
		TEXT("Builders Guild"), TEXT("Racing Federation"), TEXT("Fixers Network"),
		TEXT("Voodoo Boys"), TEXT("Aldecaldos") };
}

static const TArray<FQuestDef> BuildAct1()
{
	TArray<FQuestDef> Out;
	FQuestDef Q;

	Q.Id = FName("Q_A1_Contact"); Q.Title = TEXT("M1: First Contact");
	Q.Description = TEXT("The Crash happened. Survive the initial chaos, move, sprint, crouch. The Keepers are already watching.");
	Q.Ring = ERing::Nexus;
	{ FQuestObjective O; O.Text = TEXT("Survive the Crash (move around the Nexus)"); O.StatKey = FName("move"); O.Target = 1; Q.Objectives.Add(O); }
	Q.RewardCredits = 100; Out.Add(Q);

	Q.Id = FName("Q_A1_Keeper"); Q.Title = TEXT("M3: The Keeper's Summons");
	Q.Description = TEXT("Speak with the Keeper in the Nexus Tower. Receive your Reality Anchor.");
	Q.Ring = ERing::Nexus;
	{ FQuestObjective O; O.Text = TEXT("Talk to the Keeper"); O.StatKey = FName("talk_keeper"); O.Target = 1; Q.Objectives.Add(O); }
	Q.RewardCredits = 200; Out.Add(Q);

	Q.Id = FName("Q_A1_Fractures"); Q.Title = TEXT("M4: Echoes of What Was");
	Q.Description = TEXT("Travel to 3 rings and investigate the reality fractures. Fast travel is now unlocked.");
	Q.Ring = ERing::Nexus;
	{ FQuestObjective O; O.Text = TEXT("Visit a reality fracture (any 3 rings)"); O.StatKey = FName("visit_ring"); O.Target = 3; Q.Objectives.Add(O); }
	Q.RewardCredits = 500; Out.Add(Q);

	Q.Id = FName("Q_A1_Boss"); Q.Title = TEXT("M6: Fracture Point");
	Q.Description = TEXT("A Reality Aberration - a dragon with cybernetic implants breathing EMP fire - blocks the Aether gate. Destroy it.");
	Q.Ring = ERing::AetherCity;
	{ FQuestObjective O; O.Text = TEXT("Destroy the Reality Aberration (kill 4 drones)"); O.StatKey = FName("kill_drone"); O.Target = 4; Q.Objectives.Add(O); }
	Q.RewardCredits = 1000; Out.Add(Q);

	return Out;
}

static const TArray<FDisease> BuildDiseases()
{
	TArray<FDisease> Out;
	auto Add = [&](const char* Name, TArray<FString> Symptoms, const char* Treatment, int32 Severity)
	{
		FDisease D;
		D.Name = FString(Name);
		D.Symptoms = Symptoms;
		D.Treatment = FString(Treatment);
		D.Severity = Severity;
		Out.Add(D);
	};
	Add("Light-Headedness", { TEXT("Head glows") }, "Dim the lights", 1);
	Add("Jest Infection", { TEXT("Becomes a clown"), TEXT("Honks") }, "Remove squeaky nose", 2);
	Add("Pandemic", { TEXT("Stuck in mime mode") }, "Invisible box therapy", 2);
	Add("Cyberpunk Rejection", { TEXT("Body rejects chrome") }, "Remove cyberware", 3);
	Add("Dragon Flu", { TEXT("Coughs fire") }, "Sulfuric syrup", 4);
	Add("Magical Overload", { TEXT("Randomly levitates") }, "Dispel + sedatives", 5);
	return Out;
}

static const TArray<FRecipe> BuildRecipes()
{
	TArray<FRecipe> Out;
	auto Add = [&](const char* Name, TArray<FString> Ingredients, FName ResultItem, int32 Difficulty)
	{
		FRecipe R;
		R.Name = FString(Name);
		R.Ingredients = Ingredients;
		R.ResultItem = ResultItem;
		R.Difficulty = Difficulty;
		Out.Add(R);
	};
	Add("Potion of Hacking", { TEXT("NetherWart"), TEXT("CyberneticImplant") }, FName("PotionOfHacking"), 3);
	Add("Speed Potion", { TEXT("BlazePowder"), TEXT("Sugar") }, FName("SpeedPotion"), 2);
	Add("Dragon Salve", { TEXT("FireFern"), TEXT("DragonScaleDust") }, FName("DragonSalve"), 4);
	Add("Healing Draught", { TEXT("GoldenApple"), TEXT("RegenCrystal") }, FName("HealingDraught"), 2);
	return Out;
}

static const TArray<FItemDef> BuildItems()
{
	TArray<FItemDef> Out;
	auto Add = [&](FName Id, const char* Name, EItemCategory Cat, int32 Value, const char* Desc)
	{
		FItemDef I;
		I.Id = Id; I.Name = FString(Name); I.Category = Cat; I.Value = Value; I.Description = FString(Desc);
		Out.Add(I);
	};
	// Ingredients (harvestable / lootable).
	Add(FName("NetherWart"), "Nether Wart", EItemCategory::Ingredient, 12, "Smells of brimstone. Grows where reality is thinnest.");
	Add(FName("BlazePowder"), "Blaze Powder", EItemCategory::Ingredient, 18, "Combustible. Used in alchemy and revenge.");
	Add(FName("Sugar"), "Sugar", EItemCategory::Ingredient, 2, "Sweet. Slightly radioactive in the Frontier.");
	Add(FName("FireFern"), "Fire Fern", EItemCategory::Ingredient, 25, "Warm to the touch. Alagaesia herbalist staple.");
	Add(FName("DragonScaleDust"), "Dragon Scale Dust", EItemCategory::Ingredient, 60, "Ground dragon scale. Extremely conductive.");
	Add(FName("GoldenApple"), "Golden Apple", EItemCategory::Ingredient, 45, "Restores everything. Shiny.");
	Add(FName("RegenCrystal"), "Regen Crystal", EItemCategory::Ingredient, 80, "Pulses with light. Illegal in three rings.");
	Add(FName("CyberneticImplant"), "Cybernetic Implant", EItemCategory::Ingredient, 90, "Second-hand chrome. It dreams of flight.");
	Add(FName("Brightsteel"), "Brightsteel Ingot", EItemCategory::Ingredient, 250, "Meteorite metal. Forges Riders' swords.");
	Add(FName("VoxelBlock"), "Voxel Block", EItemCategory::Ingredient, 5, "A perfect cube. Endlessly placeable.");
	Add(FName("RawMeat"), "Raw Meat", EItemCategory::Ingredient, 8, "Hunted game. Dragons love it. So do werecats.");
	// Consumables / crafted results.
	Add(FName("HealingDraught"), "Healing Draught", EItemCategory::Consumable, 40, "Closes wounds and knits feelings.");
	Add(FName("SpeedPotion"), "Speed Potion", EItemCategory::Consumable, 30, "Time slows. You don't.");
	Add(FName("DragonSalve"), "Dragon Salve", EItemCategory::Consumable, 55, "Soothes burnt wings and burnt hands.");
	Add(FName("PotionOfHacking"), "Potion of Hacking", EItemCategory::Consumable, 70, "The network tastes like elderberries.");
	Add(FName("NexusCoffee"), "Nexus Coffee", EItemCategory::Consumable, 6, "Three rings' worth of caffeine.");
	// Key items.
	Add(FName("RealityAnchor"), "Reality Anchor", EItemCategory::KeyItem, 0, "Given by the Keeper. Your home key to the Nexus.");
	Add(FName("DataShard"), "Data Shard", EItemCategory::KeyItem, 30, "Encrypted memories. Decrypted with a BD wreath.");
	Add(FName("DragonEgg"), "Dragon Egg", EItemCategory::KeyItem, 0, "It thumps. Something inside wants out.");
	// Gear.
	Add(FName("RidersSword"), "Rider's Sword", EItemCategory::Weapon, 400, "Brightsteel blade. Its colour matches your dragon.");
	Add(FName("Scrubs"), "Hospital Scrubs", EItemCategory::Armor, 20, "Surprisingly comfortable under a cape.");
	// Vehicles / deco.
	Add(FName("Hoverbike"), "Hoverbike", EItemCategory::Vehicle, 5000, "Aether City hoverbike. Zero emissions, maximum smug.");
	Add(FName("Aquarium"), "Aquarium", EItemCategory::Deco, 300, "Houses a cyber-dolphin if you're brave.");
	return Out;
}

static const TArray<FPropertyDef> BuildProperties()
{
	TArray<FPropertyDef> Out;
	auto Add = [&](const char* Name, const char* Ring, int32 Cost, int32 Income, const char* Desc)
	{
		FPropertyDef P;
		P.Name = FString(Name); P.Ring = FString(Ring); P.Cost = Cost; P.IncomePerHour = Income; P.Description = FString(Desc);
		Out.Add(P);
	};
	Add("Frontier Cabin", "Minecraft Frontier", 10000, 40, "A cube-shaped home in a cube-shaped world.");
	Add("Mid-Wilshire Safehouse", "Mid-Wilshire", 30000, 120, "Bulletproof. Free coffee for NPD.");
	Add("Nexus Loft", "The Nexus", 50000, 200, "Centre of everything. Neighbours are keepers.");
	Add("Coliseum Garage", "Grand Coliseum", 25000, 160, "Pit-lane access. Free race-day parking.");
	Add("Alagaesia Keep", "Alagaesia Wilds", 150000, 600, "A tower for you and your dragon.");
	Add("Aether Penthouse", "Aether City", 200000, 800, "Cloud-level penthouse. Lab included.");
	return Out;
}

static const TArray<FInsuranceDef> BuildInsurances()
{
	TArray<FInsuranceDef> Out;
	auto Add = [&](const char* Name, int32 Premium, const char* Desc)
	{
		FInsuranceDef I;
		I.Name = FString(Name); I.PremiumPerHour = Premium; I.Description = FString(Desc);
		Out.Add(I);
	};
	Add("Health", 30, "Covers hospital bills.");
	Add("Vehicle", 40, "Covers repair costs. Dragon fire excluded.");
	Add("Dragon", 200, "Very expensive. Yes, it exists.");
	Add("Malpractice", 150, "For doctors. You'll need it.");
	Add("Liability", 100, "Covers superhero collateral damage.");
	Add("Life", 50, "Pays out to your other characters if one dies in hardcore.");
	return Out;
}

static const TArray<FString> BuildNpcNames()
{
	return { TEXT("Valkyrie"), TEXT("Dr. Kain"), TEXT("Officer Chen"), TEXT("Prince Malak"),
		TEXT("Thorn"), TEXT("Block"), TEXT("Aria Santos"), TEXT("Striker"),
		TEXT("Ghostwire"), TEXT("Keeper Elyon"), TEXT("Raven"), TEXT("Mabel"),
		TEXT("Rex"), TEXT("Nova"), TEXT("Tank"), TEXT("Pixel") };
}

const TArray<FRingDef>& FChimeraData::Rings() { static const TArray<FRingDef> R = BuildRings(); return R; }
const TArray<FSkillDef>& FChimeraData::Skills() { static const TArray<FSkillDef> S = BuildSkills(); return S; }
const TArray<FString>& FChimeraData::CurrencyNames() { static const TArray<FString> C = BuildCurrencies(); return C; }
const TArray<FString>& FChimeraData::FactionNames() { static const TArray<FString> F = BuildFactions(); return F; }
const TArray<FQuestDef>& FChimeraData::Act1Quests() { static const TArray<FQuestDef> Q = BuildAct1(); return Q; }
const TArray<FDisease>& FChimeraData::Diseases() { static const TArray<FDisease> D = BuildDiseases(); return D; }
const TArray<FRecipe>& FChimeraData::Recipes() { static const TArray<FRecipe> R = BuildRecipes(); return R; }

const TArray<FItemDef>& FChimeraData::Items() { static const TArray<FItemDef> I = BuildItems(); return I; }

const TArray<FPropertyDef>& FChimeraData::Properties() { static const TArray<FPropertyDef> P = BuildProperties(); return P; }

const TArray<FInsuranceDef>& FChimeraData::Insurances() { static const TArray<FInsuranceDef> I = BuildInsurances(); return I; }

const TArray<FString>& FChimeraData::NpcNames() { static const TArray<FString> N = BuildNpcNames(); return N; }