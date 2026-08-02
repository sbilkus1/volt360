// GDD 1 (world/setting), 4 (districts), 5 (progression), 9 (economy), 12 (missions),
// 14.1 (nemesis), 14.19 (NPC brain). Shared data model for CHIMERA.
#pragma once

#include "CoreMinimal.h"
#include "ChimeraTypes.generated.h"

UENUM(BlueprintType)
enum class ERing : uint8
{
	Nexus,
	AetherCity,
	NeoKingdom,
	MercyHeights,
	MidWilshire,
	Alagaesia,
	MinecraftFrontier,
	GrandColiseum,
	NightCity,
	// GDD 1 - Cinematic universe expansions.
	Seattle,           // Grey's Anatomy – Grey-Sloan Memorial Hospital
	Chicago,           // Chicago Med – Gaffney Chicago Medical Center
	ChastainPark,      // The Resident – Chastain Park Memorial Hospital (Atlanta)
	Cleveleys,         // UK coastal: Cleveleys / Blackpool / Fleetwood
	VernetLesBains,    // French Pyrenees spa town
	Panem,             // The Hunger Games — post-apocalyptic North America
	COUNT
};

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	Body,
	Reflexes,
	Intelligence,
	Technical,
	Cool,
	Magic,
	Social,
	COUNT
};

UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
	Combat,
	Magic,
	Medical,
	Police,
	Sports,
	Social,
	Technical,
	Business,
	COUNT
};

UENUM(BlueprintType)
enum class ECurrency : uint8
{
	Credits,
	Emeralds,
	Crowns,
	Eddies,
	COUNT
};

// GDD 5.4 / 14.4 - 25+ factions; the 12 the prototype tracks.
UENUM(BlueprintType)
enum class EFaction : uint8
{
	Keepers,
	HeroGuild,
	ArasakaNex,
	NPD,
	MedicalBoard,
	DragonRiders,
	AuradonCourt,
	BuildersGuild,
	RacingFederation,
	FixersNetwork,
	VoodooBoys,
	Aldecaldos,
	COUNT
};

UENUM(BlueprintType)
enum class EFactionRep : uint8
{
	Hostile,
	Hated,
	Disliked,
	Neutral,
	Liked,
	Honored,
	Revered,
	Exalted
};

UENUM(BlueprintType)
enum class EMinigameKind : uint8
{
	BreachProtocol,
	Surgery,
	Racing,
	Lockpicking,
	Interrogation,
	Braindance,
	Dance,
	FootballShootout,
	Fishing,
	Poker,
	DroneRacing,
	AptitudeTest,
	MagicDuel,
	Pickpocket,
	SafeCrack,
	Enchanting,
	Cooking,
	Blacksmithing,
	Blackjack,
	Roulette,
	Slots,
	Photography,
	CharacterCreation,
	COUNT
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Consumable,
	Ingredient,
	Weapon,
	Armor,
	KeyItem,
	Vehicle,
	Deco,
	COUNT
};

// Identity & relationships (LGBTQIA+ inclusive).
UENUM(BlueprintType)
enum class EGender : uint8 { Male, Female, NonBinary, TransMan, TransWoman, Genderfluid, Agender, Questioning, COUNT };
UENUM(BlueprintType)
enum class EOrientation : uint8 { Straight, Gay, Lesbian, Bisexual, Pansexual, Asexual, Demisexual, Aromantic, COUNT };
UENUM(BlueprintType)
enum class ERelationshipType : uint8 { Friend, Romantic, Spouse, Partner, Sibling, Mentor, Rival, Enemy, COUNT };
UENUM(BlueprintType)
enum class ETransitionStage : uint8 { None, Exploring, SocialTransition, HRT, PreOp, PostOp, Complete, COUNT };
UENUM(BlueprintType)
enum class EAccessibilityMode : uint8 { None, Wheelchair, WheelchairTransfer, MobilityScooter, HearingImpaired, VisuallyImpaired, COUNT };

// GDD 14.19.5 — Player-selectable AI difficulty (separate from game difficulty).
UENUM(BlueprintType)
enum class EAIDifficulty : uint8 { VeryEasy, Easy, Normal, Hard, Expert, Realistic, COUNT };

// Core fears (Divergent-style fear landscape).
USTRUCT(BlueprintType)
struct CHIMERA_API FFearDef
{
	GENERATED_BODY()
	UPROPERTY() FString Name;         // e.g. "Heights", "Drowning", "Betrayal"
	UPROPERTY() FString Description;
	UPROPERTY() FString Manifestation; // what it looks like in the fear landscape
};

// Police investigation (GDD 6.7).
UENUM(BlueprintType)
enum class EEvidenceType : uint8 { Witness, Forensic, Digital, Surveillance, Physical, Confession, COUNT };
UENUM(BlueprintType)
enum class ECaseStatus : uint8 { Open, Active, Stalled, Solved, Cold, Dismissed, COUNT };

USTRUCT(BlueprintType)
struct CHIMERA_API FEvidence
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() EEvidenceType Type = EEvidenceType::Physical;
	UPROPERTY() FString Description;
	UPROPERTY() int32 Weight = 1;     // how strong this evidence is
};

USTRUCT(BlueprintType)
struct FPoliceCase
{
	GENERATED_BODY()
	UPROPERTY() FString CaseID;
	UPROPERTY() FString Title;
	UPROPERTY() FString Victim;
	UPROPERTY() FString SuspectTemplate; // randomly generated suspect
	UPROPERTY() ECaseStatus Status = ECaseStatus::Open;
	UPROPERTY() TArray<FEvidence> EvidenceCollected;
	UPROPERTY() TArray<FString> Leads;
	UPROPERTY() int32 RequiredEvidence = 4;
	UPROPERTY() int32 Difficulty = 1;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FRingDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) ERing Ring = ERing::Nexus;
	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) FString Genre;
	UPROPERTY(EditAnywhere) FString Source;
	UPROPERTY(EditAnywhere) float SizeKm2 = 0.f;
	UPROPERTY(EditAnywhere) FString Description;
	UPROPERTY(EditAnywhere) FVector ZoneAnchor = FVector::ZeroVector; // teleport destination
};

USTRUCT(BlueprintType)
struct CHIMERA_API FSkillDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FName Id;
	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) ESkillCategory Category = ESkillCategory::Combat;
	UPROPERTY(EditAnywhere) int32 PerkRanks = 3;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FSkillState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FName Id;
	UPROPERTY(EditAnywhere) int32 Level = 1;
	UPROPERTY(EditAnywhere) int32 XP = 0;
	UPROPERTY(EditAnywhere) int32 Perks = 0;
	UPROPERTY(EditAnywhere) int32 	LegendaryStars = 0;
	// GDD 14.19.5 - difficulty adaptation preset.
	bool bScalesDifficulty = false; // set by session
};

UENUM(BlueprintType)
enum class EDifficultyPreset : uint8
{
	StoryMode,
	Easy,
	Normal,
	Hard,
	Legendary,
	COUNT
};

USTRUCT(BlueprintType)
struct CHIMERA_API FStockDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Symbol;
	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) float Price = 100.f;
	UPROPERTY(EditAnywhere) float Volatility = 0.03f;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Text;
	UPROPERTY(EditAnywhere) FName StatKey; // what advances it
	UPROPERTY(EditAnywhere) int32 Target = 1;
	UPROPERTY(EditAnywhere) int32 Progress = 0;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FQuestDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FName Id;
	UPROPERTY(EditAnywhere) FString Title;
	UPROPERTY(EditAnywhere) FString Description;
	UPROPERTY(EditAnywhere) ERing Ring = ERing::Nexus;
	UPROPERTY(EditAnywhere) TArray<FQuestObjective> Objectives;
	UPROPERTY(EditAnywhere) int32 RewardCredits = 0;
	UPROPERTY(EditAnywhere) int32 Rating = 0; // 0=F..6=S (GDD 12.9)
	UPROPERTY(EditAnywhere) bool bRadiant = false;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FDisease
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) TArray<FString> Symptoms;
	UPROPERTY(EditAnywhere) FString Treatment;
	UPROPERTY(EditAnywhere) int32 Severity = 1;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) TArray<FString> Ingredients;
	UPROPERTY(EditAnywhere) FName ResultItem; // produced item id
	UPROPERTY(EditAnywhere) int32 Difficulty = 1;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FItemDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FName Id;
	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) EItemCategory Category = EItemCategory::Ingredient;
	UPROPERTY(EditAnywhere) int32 Value = 0; // credits
	UPROPERTY(EditAnywhere) FString Description;
};

// GDD 9.3 - Real estate.
USTRUCT(BlueprintType)
struct CHIMERA_API FPropertyDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) FString Ring;
	UPROPERTY(EditAnywhere) int32 Cost = 0;
	UPROPERTY(EditAnywhere) int32 IncomePerHour = 0; // passive income
	UPROPERTY(EditAnywhere) FString Description;
};

// GDD 9.4 - Insurance.
USTRUCT(BlueprintType)
struct CHIMERA_API FInsuranceDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) int32 PremiumPerHour = 0;
	UPROPERTY(EditAnywhere) FString Description;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FMemoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Text;
	UPROPERTY(EditAnywhere) float TimeStamp = 0.f;
	UPROPERTY(EditAnywhere) bool bImportant = false;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FPersonality
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) float Openness = 50.f;
	UPROPERTY(EditAnywhere) float Conscientiousness = 50.f;
	UPROPERTY(EditAnywhere) float Extraversion = 50.f;
	UPROPERTY(EditAnywhere) float Agreeableness = 50.f;
	UPROPERTY(EditAnywhere) float Neuroticism = 50.f;
	UPROPERTY(EditAnywhere) float Ambition = 50.f;
	UPROPERTY(EditAnywhere) float Morality = 50.f;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FNeedState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) float Hunger = 0.f;
	UPROPERTY(EditAnywhere) float Thirst = 0.f;
	UPROPERTY(EditAnywhere) float Fatigue = 0.f;
	UPROPERTY(EditAnywhere) float Safety = 100.f;
	UPROPERTY(EditAnywhere) float Social = 50.f;
	UPROPERTY(EditAnywhere) float Esteem = 50.f;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FNemesisInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) FString PersonalityType; // Braggart / Silent / Strategist / Fanatic / Tragic
	UPROPERTY(EditAnywhere) FString Scars;
	UPROPERTY(EditAnywhere) int32 Rank = 1;
	UPROPERTY(EditAnywhere) int32 Encounters = 0;
	UPROPERTY(EditAnywhere) bool bRedemptionOffered = false;
};

// ---------------------------------------------------------------------------
// Static GDD data (prototype scope: representative subsets of the full catalog).
// ---------------------------------------------------------------------------
struct CHIMERA_API FChimeraData
{
	// GDD 1.1 - the 9 rings (Nexopolis: 11 realities crashed into one city).
	static const TArray<FRingDef>& Rings();

	// GDD 5.2 - 70+ skill trees; representative ~30 here.
	static const TArray<FSkillDef>& Skills();

	// GDD 9.1 - currencies.
	static const TArray<FString>& CurrencyNames();

	// GDD 14.4 - factions.
	static const TArray<FString>& FactionNames();

	// GDD 12.2 - Act 1 tutorial arc.
	static const TArray<FQuestDef>& Act1Quests();

	// GDD 6.8 / 4.4 - diseases.
	static const TArray<FDisease>& Diseases();

	// GDD 6.4 - potions / recipes.
	static const TArray<FRecipe>& Recipes();

	// GDD 6.4 / 9.x - item catalog.
	static const TArray<FItemDef>& Items();

	// GDD 9.3 - real estate.
	static const TArray<FPropertyDef>& Properties();

	// GDD 9.4 - insurance plans.
	static const TArray<FInsuranceDef>& Insurances();

	// GDD 14.19 - namer pool.
	static const TArray<FString>& NpcNames();
};
