// GDD 3 / 14 — 8 Missing AAA Features
// 1. Sleep/Time Skip  2. Fast Travel  3. Minimap  4. Nightlife
// 5. Stock Trading  6. Underwater  7. Emergency Jobs  8. Wildlife

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraLiving.generated.h"

// ======================================================================
// 1. SLEEP / TIME SKIP
// ======================================================================
UCLASS()
class CHIMERA_API USleepSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void Sleep(int32 Hours);   // skip forward Hours game-time. Heal, recover, advance time.
	bool CanSleep() const;     // beds at bases, hotels in every ring
	FString GetTimeOfDay() const;
};

// ======================================================================
// 2. FAST TRAVEL — Train, Taxi, Air
// ======================================================================
UENUM() enum class ETravelType : uint8 { Train, Taxi, Air, COUNT };
USTRUCT() struct FTravelRoute { FString From; FString To; int32 Cost; float Minutes; };

UCLASS()
class CHIMERA_API UFastTravel : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FTravelRoute>& GetRoutes() const { return Routes; }
	bool Travel(const FString& Destination, ETravelType Type);
	static FString GetTrainStation(const FString& Ring);
	static FString GetAirport(const FString& Ring);
private:
	TArray<FTravelRoute> Routes;
};

// ======================================================================
// 3. MINIMAP — GTA V-style
// ======================================================================
// Renders in HUD DrawMainHUD. Shows:
//   Player orientation (triangle), nearby interactables (dots),
//   Mission objectives (yellow blips), wanted direction (red/blue flash),
//   Collectible proximity (green glow when near unfound collectible).

UCLASS()
class CHIMERA_API UMinimapSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// Called by HUD — returns array of blip positions relative to player
	struct FMinimapBlip { float Angle; float Dist; FLinearColor Color; FString Label; };
	TArray<FMinimapBlip> GetBlips() const;
	void UpdateBlips(const FString& CurrentRing);
	bool IsNearCollectible() const;
private:
	TArray<FMinimapBlip> CachedBlips;
	float CollectibleRange = 200.f;
};

// ======================================================================
// 4. BAR / NIGHTLIFE
// ======================================================================
UENUM() enum class EDrinkType : uint8 { Beer, Wine, Cocktail, Shot, Water, COUNT };
USTRUCT() struct FDrink { FString Name; EDrinkType Type; int32 Cost; float Strength; FString Description; };

UCLASS()
class CHIMERA_API UBarSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FDrink>& GetMenu() const { return Menu; }
	bool BuyDrink(const FString& Name);
	int32 GetDrunkenness() const { return Drunkenness; }
	void SoberUp(float Amount);
	bool bInBarFight = false;
	FString BarFight();
	FString KaraokeSong();

private:
	TArray<FDrink> Menu;
	int32 Drunkenness = 0; // 0-100
};

// ======================================================================
// 5. STOCK MARKET TRADING TERMINAL
// ======================================================================
UCLASS()
class CHIMERA_API UStockTrading : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	// Get current stock prices (from BAWSAQ subsystem)
	struct FStockQuote { FString Symbol; FString Name; float Price; float Change; FString Tip; };
	TArray<FStockQuote> GetQuotes() const;
	bool BuyStock(const FString& Symbol, int32 Shares);
	bool SellStock(const FString& Symbol, int32 Shares);
	int32 GetPortfolioValue() const;
	TArray<FString> GetTips() const;

private:
	TMap<FString, int32> Portfolio; // symbol -> shares owned
};

// ======================================================================
// 6. UNDERWATER EXPLORATION
// ======================================================================
UENUM() enum class EDivingGear : uint8 { None, Snorkel, Scuba, Submarine, COUNT };
USTRUCT() struct FUnderwaterSite { FString Name; FString Ring; int32 Depth; FString Discovery; FString Treasure; };

UCLASS()
class CHIMERA_API UUnderwaterSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	EDivingGear GetGear() const { return CurrentGear; }
	bool BuyGear(EDivingGear Gear);
	const TArray<FUnderwaterSite>& GetSites() const { return Sites; }
	FString ExploreSite(const FString& SiteName);
	int32 GetMaxDepth() const;

private:
	EDivingGear CurrentGear = EDivingGear::None;
	TArray<FUnderwaterSite> Sites;
};

// ======================================================================
// 7. PARAMEDIC / FIREFIGHTER JOBS
// ======================================================================
UENUM() enum class EEmergencyJob : uint8 { None, Paramedic, Firefighter, COUNT };
USTRUCT() struct FEmergencyCall { FString Location; int32 Distance; int32 TimeLimit; FString Victim; FString Reward; };

UCLASS()
class CHIMERA_API UEmergencyJobs : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	EEmergencyJob GetJob() const { return CurrentJob; }
	void StartJob(EEmergencyJob Job);
	FEmergencyCall GetCall() const;
	bool CompleteCall();
	int32 GetJobsCompleted() const { return JobsCompleted; }
	FString GetRewardUnlock() const; // vehicle unlocked at milestones

private:
	EEmergencyJob CurrentJob = EEmergencyJob::None;
	int32 JobsCompleted = 0;
	FEmergencyCall CurrentCall;
	void GenerateCall();
};

// ======================================================================
// 8. WILDLIFE / ANIMALS
// ======================================================================
UENUM() enum class EAnimalType : uint8 { Deer, Boar, Wolf, Bear, Rabbit, Bird, Fish, Mutt, COUNT };
USTRUCT() struct FAnimalDef { FString Name; EAnimalType Type; FString Ring; FString Habitat; bool bHostile; FString Drop; };
USTRUCT() struct FLegendaryFish { FString Name; FString Ring; FString Location; FString Bait; float WeightKg; };

UCLASS()
class CHIMERA_API UWildlifeSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FAnimalDef>& GetAnimals() const { return Animals; }
	FString Hunt(const FString& Ring);
	FString Fish(const FString& Ring, bool bLegendaryAttempt = false);
	const TArray<FLegendaryFish>& GetLegendaryFish() const { return LegendaryFish; }
	int32 GetLegendaryFishCaught() const { return LegendaryCaught; }

private:
	TArray<FAnimalDef> Animals;
	TArray<FLegendaryFish> LegendaryFish;
	int32 LegendaryCaught = 0;
};
