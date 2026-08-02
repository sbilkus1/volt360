// GDD 6.5 — Vehicle spawn system. GTA/Forza-style: specific cars in specific places.
// Rarity tiers, pre-upgraded rare finds, barn find legendaries.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraVehicleSpawns.generated.h"

UENUM()
enum class ESpawnRarity : uint8 { Common, Uncommon, Rare, Legendary, COUNT };

USTRUCT()
struct FMVehicleSpawn
{
	GENERATED_BODY()
	UPROPERTY() FString VehicleId;
	UPROPERTY() FString Ring;        // which ring, or "All"
	UPROPERTY() FString SubZone;     // e.g. "Beach", "Downtown", "Docks", "Garage"
	UPROPERTY() ESpawnRarity Rarity = ESpawnRarity::Common;
	UPROPERTY() int32 UpgradeLevel = 0; // 0=stock, 1-4=pre-upgraded tiers
	UPROPERTY() FString UniqueName;  // for legendary barn finds
	UPROPERTY() FString Hint;        // barn find clue text
};

UCLASS()
class CHIMERA_API UVehicleSpawnSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const TArray<FMVehicleSpawn>& GetSpawns() const { return Spawns; }

	// Get vehicles that spawn in this ring, weighted by rarity.
	TArray<FString> GetSpawnsForRing(const FString& Ring, ESpawnRarity MaxRarity = ESpawnRarity::Rare) const;

	// Get a single random vehicle for a ring (respects rarity weights).
	FString GetRandomSpawn(const FString& Ring) const;

	// Barn find: get a legendary vehicle hidden in this ring.
	FMVehicleSpawn GetBarnFind(const FString& Ring) const;

	// Weight chance: Common=70, Uncommon=20, Rare=8, Legendary=2
	static int32 RarityWeight(ESpawnRarity R);

private:
	void SeedSpawns();
	TArray<FMVehicleSpawn> Spawns;
};
