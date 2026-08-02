#include "Core/ChimeraRecords.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UHallOfRecords::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedAchievements();
}

void UHallOfRecords::SeedAchievements()
{
	// GDD 14.12 - 30+ cross-discipline achievements.
	FAchievementDef A[] = {
		{ TEXT("first_kill"),  TEXT("First Blood"), TEXT("Defeat your first enemy."), 10, TEXT("kill"), 1 },
		{ TEXT("veteran"),     TEXT("Veteran"),     TEXT("Defeat 50 enemies."), 25, TEXT("kill"), 50 },
		{ TEXT("slayer"),      TEXT("Slayer"),      TEXT("Defeat 500 enemies."), 50, TEXT("kill"), 500 },
		{ TEXT("first_race"),  TEXT("Pole Position"), TEXT("Win your first race."), 10, TEXT("race_win"), 1 },
		{ TEXT("champion"),    TEXT("Champion"),    TEXT("Win 50 races."), 25, TEXT("race_win"), 50 },
		{ TEXT("first_goal"),  TEXT("GOAL!"),       TEXT("Score your first football goal."), 10, TEXT("goal"), 1 },
		{ TEXT("hat_trick"),   TEXT("Hat Trick"),   TEXT("Score 3 goals in one match."), 20, TEXT("goal"), 3 },
		{ TEXT("first_surgery"), TEXT("First Cut"), TEXT("Complete your first surgery."), 10, TEXT("surgery_done"), 1 },
		{ TEXT("pro_medic"),   TEXT("Pro Medic"),   TEXT("Complete 100 surgeries."), 25, TEXT("surgery_done"), 100 },
		{ TEXT("first_arrest"), TEXT("Badge"),      TEXT("Make your first arrest."), 10, TEXT("arrest"), 1 },
		{ TEXT("detective"),   TEXT("Detective"),   TEXT("Make 50 arrests."), 25, TEXT("arrest"), 50 },
		{ TEXT("first_hack"),  TEXT("Script Kiddie"), TEXT("Complete your first hack."), 10, TEXT("hack"), 1 },
		{ TEXT("netrunner"),   TEXT("Netrunner"),   TEXT("Complete 50 hacks."), 25, TEXT("hack"), 50 },
		{ TEXT("first_build"), TEXT("Builder"),     TEXT("Place your first block."), 10, TEXT("place_block"), 1 },
		{ TEXT("architect"),   TEXT("Architect"),   TEXT("Place 500 blocks."), 25, TEXT("place_block"), 500 },
		{ TEXT("first_craft"), TEXT("Alchemist"),   TEXT("Craft your first item."), 10, TEXT("craft_done"), 1 },
		{ TEXT("dragon_rider"),TEXT("Dragon Rider"),TEXT("Hatch and bond a dragon."), 15, TEXT("dragon_hatched"), 1 },
		{ TEXT("sky_lord"),    TEXT("Sky Lord"),    TEXT("Fly your dragon for 10 minutes."), 30, TEXT("dragon_flight_sec"), 600 },
		{ TEXT("pet_lover"),   TEXT("Pet Lover"),   TEXT("Adopt a companion."), 10, TEXT("pet_adopted"), 1 },
		{ TEXT("bounty_hunter"), TEXT("Bounty Hunter"), TEXT("Collect your first bounty."), 10, TEXT("bounty_done"), 1 },
		{ TEXT("credits_100k"),TEXT("Pocket Change"), TEXT("Earn 100,000 credits total."), 20, TEXT("credits_earned"), 100000 },
		{ TEXT("millionaire"), TEXT("Millionaire"), TEXT("Earn 1,000,000 credits total."), 35, TEXT("credits_earned"), 1000000 },
		{ TEXT("hero_50"),     TEXT("Hero of the Rings"), TEXT("Reach Hero/Villain +50."), 15, TEXT("hero_peak"), 50 },
		{ TEXT("villain_50"),  TEXT("Wanted"),      TEXT("Reach Hero/Villain -50."), 15, TEXT("villain_peak"), 50 },
		{ TEXT("explorer"),    TEXT("Explorer"),    TEXT("Visit every ring."), 20, TEXT("visit_ring"), 9 },
		{ TEXT("fisherman"),   TEXT("Fisherman"),   TEXT("Catch your first fish."), 10, TEXT("fish_caught"), 1 },
		{ TEXT("legend"),      TEXT("Living Legend"), TEXT("Reach skill level 100 in any career."), 50, TEXT("skill_100"), 1 },
		{ TEXT("legacy"),      TEXT("Legacy"),      TEXT("Retire a character and create an heir."), 30, TEXT("generation"), 1 },
		{ TEXT("generous"),    TEXT("Generous Soul"), TEXT("Reach Generosity 80."), 20, TEXT("generosity_peak"), 80 },
		{ TEXT("fame_80"),     TEXT("Famous"),      TEXT("Reach Fame 80."), 20, TEXT("fame_peak"), 80 },
		{ TEXT("infamy_80"),   TEXT("Infamous"),    TEXT("Reach Infamy 80."), 20, TEXT("infamy_peak"), 80 },
		// More career milestones
		{ TEXT("dungeon_50"),  TEXT("Dungeon Delver"), TEXT("Reach floor 50 in any dungeon."), 25, TEXT("dungeon_floor"), 50 },
		{ TEXT("dungeon_200"), TEXT("Tower Master"), TEXT("Reach floor 200 in any dungeon."), 40, TEXT("dungeon_floor"), 200 },
		{ TEXT("barn_1"),      TEXT("Barn Finder"), TEXT("Find your first barn find vehicle."), 25, TEXT("barn_finds"), 1 },
		{ TEXT("barn_5"),      TEXT("Collector"),   TEXT("Find all 5 barn find vehicles."), 50, TEXT("barn_finds"), 5 },
		{ TEXT("business_5"),  TEXT("Entrepreneur"), TEXT("Own 5 businesses."), 25, TEXT("businesses_owned"), 5 },
		{ TEXT("bases_3"),     TEXT("Property Mogul"), TEXT("Buy 3 properties across the rings."), 20, TEXT("bases_owned"), 3 },
		{ TEXT("bases_10"),    TEXT("Land Baron"),  TEXT("Buy 10 properties."), 40, TEXT("bases_owned"), 10 },
		{ TEXT("heist_1"),     TEXT("First Score"), TEXT("Complete your first heist."), 20, TEXT("heists_done"), 1 },
		{ TEXT("dates_5"),     TEXT("Social Butterfly"), TEXT("Go on 5 dates."), 15, TEXT("dates"), 5 },
		{ TEXT("dates_20"),    TEXT("Heartbreaker"), TEXT("Go on 20 dates across all relationships."), 35, TEXT("dates"), 20 },
		{ TEXT("pickpocket_10"), TEXT("Light Fingers"), TEXT("Successfully pickpocket 10 times."), 25, TEXT("pickpockets"), 10 },
		{ TEXT("tribute_win"),   TEXT("Victor"),       TEXT("Win a Hunger Games tribute fight."), 20, TEXT("tribute_wins"), 5 },
		{ TEXT("rebellion_won"), TEXT("Revolutionary"), TEXT("Win the grand rebellion."), 40, TEXT("rebellion_win"), 1 },
		{ TEXT("duel_10"),     TEXT("Master Duelist"), TEXT("Win 10 magical duels."), 25, TEXT("duels_won"), 10 },
		{ TEXT("photos_25"),    TEXT("Shutterbug"),   TEXT("Take 25 photos."), 15, TEXT("photos"), 25 },
		{ TEXT("cook_master"),  TEXT("Master Chef"),  TEXT("Cook 10 perfect dishes."), 20, TEXT("cook_perfect"), 10 },
		{ TEXT("blacksmith_50"), TEXT("Master Smith"), TEXT("Forge weapons worth 50 quality."), 25, TEXT("forge_quality"), 50 },
		{ TEXT("transition_done"), TEXT("Becoming"),  TEXT("Complete your gender transition."), 30, TEXT("transition_complete"), 1 },
		{ TEXT("wheelchair_master"), TEXT("Rolling Thunder"), TEXT("Travel 10km in wheelchair mode."), 20, TEXT("wheelchair_km"), 10 },
		{ TEXT("fear_conqueror"), TEXT("Fearless"),    TEXT("Confront all 3 of your fears in the landscape."), 30, TEXT("fears_confronted"), 3 },
		{ TEXT("tardis_rooms"), TEXT("Time Lord"),    TEXT("Explore all 8 TARDIS rooms."), 25, TEXT("tardis_rooms"), 8 },
		{ TEXT("all_skills_50"), TEXT("Jack of All Trades"), TEXT("Reach level 50 in 5 different skills."), 35, TEXT("skills_over_50"), 5 },
		{ TEXT("plat_chimera"), TEXT("Platinum CHIMERA"), TEXT("Unlock every achievement. 5,000 hours. Only the worthy."), 100, TEXT("achievements_unlocked"), 50 },
	};
	for (const FAchievementDef& a : A) Achievements.Add(a);
}

void UHallOfRecords::Tally(const FName& Stat, int32 Amt)
{
	// Append chronicle entry.
	FRecordEntry E;
	E.Category = TEXT("General");
	E.Text = Stat.ToString();
	E.Value = Amt;
	Chronicle.Insert(E, 0);
	if (Chronicle.Num() > 500) Chronicle.SetNum(500);

	CheckAchievements();
}

TArray<FRecordEntry> UHallOfRecords::GetTop(const FString& Category, int32 N) const
{
	TArray<FRecordEntry> Result;
	for (const FRecordEntry& E : Chronicle)
		if (E.Category == Category || Category.IsEmpty())
			Result.Add(E);
	Result.Sort([](const FRecordEntry& A, const FRecordEntry& B) { return A.Value > B.Value; });
	if (Result.Num() > N) Result.SetNum(N);
	return Result;
}

void UHallOfRecords::CheckAchievements()
{
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return;

	// Build a lookup of cumulative stat values from the chronicle.
	TMap<FString, int32> Cum;
	for (const FRecordEntry& E : Chronicle) Cum.FindOrAdd(E.Text) += E.Value;
	// Inject real-time session state.
	Cum.FindOrAdd(TEXT("hero_peak")) = FMath::Max(Cum.FindRef(TEXT("hero_peak")), Sess->HeroVillainMeter);
	Cum.FindOrAdd(TEXT("villain_peak")) = FMath::Max(Cum.FindRef(TEXT("villain_peak")), -Sess->HeroVillainMeter);
	Cum.FindOrAdd(TEXT("fame_peak")) = FMath::Max(Cum.FindRef(TEXT("fame_peak")), Sess->GetFame());
	Cum.FindOrAdd(TEXT("infamy_peak")) = FMath::Max(Cum.FindRef(TEXT("infamy_peak")), Sess->GetInfamy());
	Cum.FindOrAdd(TEXT("generosity_peak")) = FMath::Max(Cum.FindRef(TEXT("generosity_peak")), Sess->GetGenerosity());
	Cum.FindOrAdd(TEXT("skills_100")) = 0;
	for (const FSkillState& S : Sess->Skills) if (S.Level >= 100) Cum[TEXT("skill_100")] = 1;
	Cum.FindOrAdd(TEXT("rings_visited")) = Cum.FindOrAdd(TEXT("rings_visited")) + (int32)Sess->CurrentRing;
	Cum.FindOrAdd(TEXT("generation")) = Sess->Generations;

	for (const FAchievementDef& A : Achievements)
	{
		if (IsAchieved(A.Id)) continue;
		if (Cum.FindRef(A.StatName) >= A.Threshold)
		{
			UnlockedAchievements.Add(A.Id);
			Sess->ShowMessage(FString::Printf(TEXT("ACHIEVEMENT: %s (%d pts) - %s"), *A.Name, A.Points, *A.Desc));
			Sess->RecordEvent(FString::Printf(TEXT("Unlocked achievement: %s"), *A.Name));
		}
	}
}

void UHallOfRecords::PrestigeStat(const FString& Stat)
{
	int32& P = StatPrestige.FindOrAdd(Stat);
	P++;
}
