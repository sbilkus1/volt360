// GDD 14.1 (nemesis), 14.4 (faction warfare), 14.5 (disasters), 14.7 (politics).
// Persistent world simulations that respond to player actions.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "Containers/Ticker.h"
#include "TimerManager.h"
#include "ChimeraWorldSim.generated.h"

// GDD 14.1 - Nemesis System. NPCs you wronged remember, adapt, and evolve.
UCLASS()
class CHIMERA_API UNemesisSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Called whenever the player humiliates/defeats/arrests/harms an NPC.
	void OnPlayerWronged(const FString& NpcName, const FString& How);
	FNemesisInfo* FindNemesis(const FString& NpcName);
	const TArray<FNemesisInfo>& GetNemeses() const { return Nemeses; }
	int32 Rank() const { return Nemeses.Num(); }

	// GDD 14.1 - nemesis ambush. Returns true and fills an act description.
	bool NemesisAct(FString& OutName, FString& OutAct);

private:
	bool Tick(float DeltaTime);
	TArray<FNemesisInfo> Nemeses;
	float AmbushTimer = 0.f;
	FTSTicker::FDelegateHandle TickerHandle;
};

// GDD 14.4 - Faction warfare / dynamic territory map.
UCLASS()
class CHIMERA_API UFactionWarfareSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Percentage control per faction (persisted in session).
	const TArray<int32>& GetTerritory() const { return Territory; }
	EFaction GetLeadingFaction() const;
	bool bBattleInProgress = false;
	FString BattleReport;

private:
	bool Tick(float DeltaTime);
	TArray<int32> Territory;
	float WarTimer = 0.f;
	FTSTicker::FDelegateHandle TickerHandle;
};

// GDD 14.5 - Disaster & catastrophe system (server-wide events).
UCLASS()
class CHIMERA_API UDisasterSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	bool bDisasterActive = false;
	FString ActiveDisaster;
	ERing DisasterRing = ERing::Nexus;
	float RemainingSeconds = 0.f;

private:
	void TryTriggerDisaster();
	void ClockTick();
	void EndDisaster();
	FTimerHandle DisasterTimerHandle;
	FTimerHandle ClockTimerHandle;
	FTimerHandle ClearTimerHandle;
};

// GDD 14.7 - Politics & governance (ring elections).
UCLASS()
class CHIMERA_API UPoliticsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	FString Governor;
	FString Rival;

private:
	void RunElection();
	FTimerHandle ElectionTimerHandle;
};
