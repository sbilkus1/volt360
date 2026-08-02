// GDD 3.5/3.6 (session + legacy loops), 5 (progression), 8 (narrative state),
// 9 (economy), 11 (cross-ring coherence). Single source of truth for persistent
// character + world state. All simulations read/write through here.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraSessionSubsystem.generated.h"

UCLASS()
class CHIMERA_API UChimeraSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// GDD 1.3 / 3.5 - persistent world state.
	ERing CurrentRing = ERing::Nexus;
	FString LifePath;
	bool bCharacterCreated = false;
	bool bHeir = false;
	int32 Generations = 0;
	float HoursPlayed = 0.f;

	// GDD 5 - progression.
	TArray<FSkillState> Skills;
	TArray<int32> Attributes;   // EAttribute::COUNT, 1..100
	TArray<int32> Currencies;   // ECurrency::COUNT
	TArray<int32> FactionRep;   // EFaction::COUNT, -100..+100
	TArray<int32> FactionTerritory; // EFaction::COUNT, % control
	int32 WantedLevel = 0;      // GDD 4.5 GTA wanted system (0..6)
	int32 HeroVillainMeter = 0; // GDD 4.2 hero(+)/villain(-)
	FString Governor;           // GDD 14.7 election winner
	TArray<FString> EventLog;   // chronicle (GDD 14.18)
	TMap<FName, int32> Stats;   // quest/stat funnel
	bool bHasDragon = false;    // GDD 4.6/6.9 dragon egg hatched
	FString LastMessage;        // transient HUD message
	float LastMessageTime = -100.f;

	// Quest state.
	TArray<FName> CompletedQuests;
	FQuestDef ActiveQuest;
	FString CurrentObjective;

	void AddPlayTime(float Seconds);

	// --- skills (GDD 5.2/5.3) ---
	FSkillState* GetSkill(FName Id);
	int32 GetSkillLevel(FName Id);
	void AddSkillXP(FName Id, int32 XP);
	void SpendPerk(FName Id);
	void LegendaryReset(FName Id);

	// --- attributes / currencies / rep ---
	int32 GetAttribute(EAttribute A) const { return Attributes[(int32)A]; }
	void SetAttribute(EAttribute A, int32 V) { Attributes[(int32)A] = FMath::Clamp(V, 1, 100); }
	int32 GetCurrency(ECurrency C) const { return Currencies[(int32)C]; }
	void AddCurrency(ECurrency C, int32 Amt);
	int32 GetRep(EFaction F) const { return FactionRep[(int32)F]; }
	void AddRep(EFaction F, int32 Amt);

	// GDD 11 - cross-ring consequence funnel. Any system can push an event.
	void RecordEvent(const FString& What);
	void AddStat(FName Key, int32 Amt = 1);
	void ShowMessage(const FString& M) { LastMessage = M; LastMessageTime = 6.f; }

	// GDD 12 - quests.
	void SetActiveQuest(const FQuestDef& Q);
	void CompleteActiveQuest();

	// GDD 3.6 - Legacy system. Retire current character; next character is heir.
	void RetireCharacter();
	void StartHeir();

	// GDD 6.4 - Inventory + crafting.
	void AddItem(FName Id, int32 Count = 1);
	bool RemoveItem(FName Id, int32 Count = 1);
	int32 GetItemCount(FName Id) const { return Inventory.FindRef(Id); }
	bool HasItem(FName Id) const { return GetItemCount(Id) > 0; }
	// Attempt craft. Returns true and consumes ingredients + awards result on success.
	bool Craft(FName RecipeId);

	// GDD 9.3 - Real estate. Buying costs credits and starts passive income.
	bool BuyProperty(const FString& Name);
	bool SellProperty(const FString& Name);
	const TArray<FString>& GetOwnedProperties() const { return OwnedProperties; }
	int32 GetPassiveIncomePerHour() const;
	bool IsPropertyOwned(const FString& Name) const;

	// GDD 9.4 - Insurance. Active plans drain credits each in-game hour.
	bool BuyInsurance(const FString& Name);
	bool CancelInsurance(const FString& Name);
	bool HasInsurance(const FString& Name) const { return ActiveInsurances.Contains(Name); }
	int32 GetInsurancePremiumPerHour() const;
	void PayInsurancePremium(); // called on hour tick

	// GDD 14.19 - NPC relationships. -100 (enemy) .. +100 (friend).
	void AddRelationship(FString NpcName, int32 Amt);
	int32 GetRelationship(const FString& NpcName) const { return Relationships.FindRef(NpcName); }
	const TMap<FString, int32>& GetAllRelationships() const { return Relationships; }

	// GDD 14.8 - Reputation & Fame (multi-axis). -100..+100 each.
	void AddFame(int32 Amt) { Fame = FMath::Clamp(Fame + Amt, -100, 100); }
	void AddInfamy(int32 Amt) { Infamy = FMath::Clamp(Infamy + Amt, -100, 100); }
	void AddGenerosity(int32 Amt) { Generosity = FMath::Clamp(Generosity + Amt, -100, 100); }
	int32 GetFame() const { return Fame; }
	int32 GetInfamy() const { return Infamy; }
	int32 GetGenerosity() const { return Generosity; }

	// GDD 14.3 - Companion & Pet system.
	bool AdoptPet(const FString& Type, const FString& Name);
	const FString& GetPetType() const { return PetType; }
	const FString& GetPetName() const { return PetName; }
	int32 GetPetBond() const { return PetBond; }
	int32 GetPetLevel() const { return PetLevel; }
	void BondWithPet(int32 Amt = 5);   // training/interaction raises bond
	void TrainPet(int32 Amt = 10);
	bool HasPet() const { return !PetType.IsEmpty(); }

	// GDD 6.9 - Dragon system (egg -> hatchling -> bonded rider).
	bool BondDragon();                 // called when the egg hatches
	int32 GetDragonBond() const { return DragonBond; }
	void BondWithDragon(int32 Amt = 5);
	void FeedDragon(int32 Meat = 1);   // consumes item, raises bond
	const FString& GetDragonName() const { return DragonName; }
	void SetDragonName(const FString& N) { DragonName = N; }
	bool CanFlyDragon() const { return bHasDragon && DragonBond >= 20; }

	// GDD 14.9 - Bounties placed on the player.
	int32 GetPlayerBounty() const { return PlayerBounty; }
	void SetPlayerBounty(int32 V) { PlayerBounty = FMath::Clamp(V, 0, 100000); }

	// GDD 1.3 - Day/Night + Weather clock. Advances with play time.
	void TickClock(float DeltaTime);
	int32 GetGameDay() const { return GameDay; }
	float GetHourOfDay() const { return HourOfDay; }
	bool IsNight() const { return HourOfDay < 6.f || HourOfDay >= 20.f; }
	const FString& GetWeather() const { return Weather; }
	const FString& GetSeason() const { return Season; }
	void ForceWeather(const FString& W) { Weather = W; }
	bool bFestivalActive = false;
	FString ActiveFestival;
	// GDD 14.11 - seasonal festival effects. Buffs last while active.
	float FestivalXPBonus = 1.f;

	// GDD 14.18 / 9.x - News ticker. Headlines react to world events.
	const TArray<FString>& GetNews() const { return News; }
	FString GetLatestHeadline() const { return News.Num() > 0 ? News[0] : TEXT("Nexopolis City News: reality remains stable."); }

	// GDD 3.5 - Save/Load. JSON slots in <ProjectSaved>/SaveGames/<Slot>.json.
	bool SaveGame(const FString& Slot);
	bool LoadGame(const FString& Slot);

	// Persistent fields added by this tranche.
	UPROPERTY() TMap<FName, int32> Inventory;   // GDD 6.4
	UPROPERTY() TArray<FString> OwnedProperties; // GDD 9.3
	UPROPERTY() TArray<FString> ActiveInsurances; // GDD 9.4
	UPROPERTY() TMap<FString, int32> Relationships; // GDD 14.19
	int32 GameDay = 1;        // GDD 1.3
	float HourOfDay = 6.f;
	FString Weather = TEXT("Clear");
	FString Season = TEXT("Spring");
	TArray<FString> News;     // GDD 14.18

	// GDD 14.8 - fame/infamy/generosity axes.
	int32 Fame = 0;
	int32 Infamy = 0;
	int32 Generosity = 0;

	// GDD 14.3 - companion/pet state.
	FString PetType;
	FString PetName;
	int32 PetBond = 0;
	int32 PetLevel = 1;

	// GDD 6.9 - dragon state.
	FString DragonName = TEXT("Ember");
	int32 DragonBond = 0;

	// GDD 14.9 - bounty on the player's head.
	int32 PlayerBounty = 0;

	// GDD 14.19.5 - difficulty adaptation.
	UPROPERTY() EDifficultyPreset Difficulty = EDifficultyPreset::Normal;
	float GetDamageMultiplier() const; // multiplier for enemy damage TO player
	float GetHealthMultiplier() const; // multiplier for enemy health
	float GetMinigameTimeMultiplier() const; // multiplier for minigame time windows

	// GDD 5.1 — identity, fears, and LGBTQIA+.
	UPROPERTY() EGender PlayerGender = EGender::NonBinary;
	UPROPERTY() EOrientation PlayerOrientation = EOrientation::Pansexual;
	UPROPERTY() FString Pronouns = TEXT("they/them");
	UPROPERTY() ETransitionStage TransitionStage = ETransitionStage::None;
	UPROPERTY() TArray<FString> PlayerFears;
	void SetGender(EGender G);
	void SetOrientation(EOrientation O);
	void BeginTransition();
	void AdvanceTransition();

	// GDD 14.15 — Accessibility & disability.
	UPROPERTY() EAccessibilityMode Accessibility = EAccessibilityMode::None;
	void SetAccessibility(EAccessibilityMode Mode);
	bool CanDriveAdaptively() const { return Accessibility == EAccessibilityMode::Wheelchair || Accessibility == EAccessibilityMode::WheelchairTransfer; }
	int32 WheelchairSpeed = 60;
	void BoostWheelchair() { WheelchairSpeed = FMath::Min(120, WheelchairSpeed + 10); }

	// GDD 14.12 — Prestige title system.
	FString GetPrestigeTitle() const;

	// GDD 14.19.5 — Player-selectable AI difficulty.
	UPROPERTY() EAIDifficulty AIDifficulty = EAIDifficulty::Normal;
	void SetAIDifficulty(EAIDifficulty D);
	float GetAIAccuracyMultiplier() const;     // affects NPC shooting/combat
	float GetAIReactionMultiplier() const;     // affects NPC response time
	float GetAIAggressionMultiplier() const;    // affects NPC hostility/ambush rate
	float GetAILearningMultiplier() const;      // affects NPC skill adaptation speed
	bool UseTeamTactics() const;                // Hard+: flanking, coordinated attacks

private:
	void SeedDefaults();
	void TickHourly();  // hour boundary: passive income, insurance, weather, news
	void GenerateWeather();
	void GenerateNews();

	// JSON field helpers.
	static void WriteInt(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, int32 Value);
	static void WriteFloat(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, float Value);
	static void WriteString(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, const FString& Value);
	static void WriteName(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, FName Value);
	static void WriteIntArray(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, const TArray<int32>& Values);
	static void WriteStringArray(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, const TArray<FString>& Values);
	static int32 ReadInt(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, int32 Fallback = 0);
	static float ReadFloat(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, float Fallback = 0.f);
	static FString ReadString(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, const FString& Fallback = TEXT(""));
	static FName ReadName(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, FName Fallback = NAME_None);
	static void ReadIntArray(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, TArray<int32>& Out);
	static void ReadStringArray(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, TArray<FString>& Out);
};
