// GDD 14.18 - Hall of Records. Lifetime statistic tracker + chronicle viewer.
// Every meaningful action is tallied; achievements unlock permanently.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraRecords.generated.h"

USTRUCT(BlueprintType)
struct CHIMERA_API FRecordEntry
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FString Category;
	UPROPERTY(EditAnywhere) FString Text;
	UPROPERTY(EditAnywhere) int32 Value = 1;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FAchievementDef
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FString Id;
	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) FString Desc;
	UPROPERTY(EditAnywhere) int32 Points = 10;
	UPROPERTY(EditAnywhere) FString StatName;
	UPROPERTY(EditAnywhere) int32 Threshold = 1;
};

UCLASS()
class CHIMERA_API UHallOfRecords : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// GDD 14.18 - tally a stat.
	void Tally(const FName& Stat, int32 Amt = 1);

	// Return the entire chronicle (sorted newest-first, capped at 500).
	const TArray<FRecordEntry>& GetChronicle() const { return Chronicle; }

	// Top-10 tallies for a given category (e.g. "Combat", "Racing").
	TArray<FRecordEntry> GetTop(const FString& Category, int32 N = 10) const;

	// Achievement system (GDD 14.12).
	const TArray<FAchievementDef>& GetAchievements() const { return Achievements; }
	bool IsAchieved(const FString& Id) const { return UnlockedAchievements.Contains(Id); }
	void CheckAchievements();  // run after any tally; fires popup on new unlock.

	// Prestige counter per stat (GDD 14.12 - Prestige System).
	int32 GetStatPrestige(const FString& Stat) const { return StatPrestige.FindRef(Stat); }
	void PrestigeStat(const FString& Stat);

	const TSet<FString>& GetUnlocked() const { return UnlockedAchievements; }

private:
	void SeedAchievements();

	UPROPERTY() TArray<FRecordEntry> Chronicle;
	UPROPERTY() TSet<FString> UnlockedAchievements;
	UPROPERTY() TArray<FAchievementDef> Achievements;
	UPROPERTY() TMap<FString, int32> StatPrestige;
};
