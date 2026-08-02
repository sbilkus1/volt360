// GDD 4.x — Hunger Games tournament & rebellion war subsystem.
// Full 24-tribute arena simulation + Mockingjay war campaign.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "ChimeraHungerGames.generated.h"

UENUM(BlueprintType)
enum class EArenaPhase : uint8 { Idle, Reaping, Parade, Training, Arena, Victor, COUNT };

UENUM(BlueprintType)
enum class EWarPhase : uint8 { Peace, Uprising, District2, CapitolAssault, SnowFallen, FreePanem, COUNT };

USTRUCT(BlueprintType)
struct FTribute
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() int32 District = 0;        // 1..12
	UPROPERTY() bool bFemale = false;
	UPROPERTY() bool bCareer = false;
	UPROPERTY() bool bAlive = true;
	UPROPERTY() int32 Combat = 3;          // 1..10
	UPROPERTY() int32 stealth = 3;
	UPROPERTY() int32 survival = 3;
	UPROPERTY() int32 Health = 100;
	UPROPERTY() int32 Hunger = 0;          // 100 = dead
	UPROPERTY() int32 Kills = 0;
	UPROPERTY() int32 SponsorGifts = 0;
	UPROPERTY() FString Weapon;
	UPROPERTY() FString LastEvent;
};

UCLASS()
class CHIMERA_API UHungerGamesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// === Tournament management ===
	EArenaPhase GetPhase() const { return Phase; }
	void ResetGames() { Phase = EArenaPhase::Idle; } // start fresh
	const TArray<FTribute>& GetTributes() const { return Tributes; }
	int32 GetAliveCount() const;
	int32 GetDeadCount() const { return FallenTributes.Num(); }
	const TArray<FString>& GetFallen() const { return FallenTributes; }
	const TArray<FString>& GetEventLog() const { return EventLog; }
	int32 GetArenaDay() const { return ArenaDay; }

	// Start a new Hunger Games.
	void StartReaping();
	void AdvanceToTraining();
	void StartArena();
	void CrownVictor();

	// Called by interactables: sponsor a tribute by index.
	bool SponsorTribute(int32 Index);

	// === Arena Tick (called every N seconds) ===
	bool ArenaTick(float DeltaTime);

	// === Rebellion War (Mockingjay) ===
	EWarPhase GetWarPhase() const { return WarPhase; }
	const TArray<FString>& GetWarLog() const { return WarLog; }
	int32 GetWarProgress() const { return WarProgress; }
	int32 GetWarMax() const { return WarMaxProgress; }

	// Advance the rebellion — called by mission interactables.
	bool AdvanceWar(const FString& Action);
	bool bWarActive = false;

	int32 GetTotalSGamesPlayed() const { return TotalGamesPlayed; }
	FString GetVictorName() const { return VictorName; }

private:
	void GenerateTributes();
	void DoArenaEvent();
	FTribute* FindTribute(const FString& Name);

	EArenaPhase Phase = EArenaPhase::Idle;
	TArray<FTribute> Tributes;
	TArray<FString> FallenTributes;
	TArray<FString> EventLog;
	int32 ArenaDay = 0;
	float ArenaTimer = 0.f;
	FTSTicker::FDelegateHandle ArenaTicker;

	// Rebellion war state.
	EWarPhase WarPhase = EWarPhase::Peace;
	TArray<FString> WarLog;
	int32 WarProgress = 0;
	int32 WarMaxProgress = 20;
	int32 TotalGamesPlayed = 0;
	FString VictorName;
};
