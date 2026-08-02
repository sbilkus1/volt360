// GDD 14.12 — Raid Dungeons. 5 mega-dungeons with procedural floors, bosses, loot.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraDungeons.generated.h"

UENUM()
enum class EDungeon : uint8 { NexusDepths, BlackwallRift, DragonGraveyard, ForgottenWard, StadiumChampions, COUNT };

USTRUCT()
struct FDungeonFloor
{
	GENERATED_BODY()
	UPROPERTY() int32 Floor = 1;
	UPROPERTY() FString EnemyType;   // "Drone", "Cultist", "Spectre", etc
	UPROPERTY() int32 EnemyCount = 3;
	UPROPERTY() int32 EnemyHP = 50;
	UPROPERTY() int32 EnemyDamage = 10;
	UPROPERTY() bool bBossFloor = false;
	UPROPERTY() FString BossName;
	UPROPERTY() int32 BossHP = 500;
	UPROPERTY() FString LootDrop;    // items dropped on clear
};

USTRUCT()
struct FDungeonDef
{
	GENERATED_BODY()
	UPROPERTY() EDungeon Type = EDungeon::NexusDepths;
	UPROPERTY() FString Name;
	UPROPERTY() FString Ring;         // which ring the entrance is in
	UPROPERTY() FString Flavor;
	UPROPERTY() int32 MaxFloors = 500;
	UPROPERTY() int32 BossInterval = 5;
};

UCLASS()
class CHIMERA_API UDungeonSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Dungeon management.
	const TArray<FDungeonDef>& GetDungeons() const { return Dungeons; }
	EDungeon GetActiveDungeon() const { return ActiveDungeon; }
	int32 GetCurrentFloor() const { return CurrentFloor; }
	bool IsActive() const { return bActive; }
	int32 GetDeepestFloor(EDungeon D) const { return DeepestFloors[(int32)D]; }
	int32 GetBossesKilled(EDungeon D) const { return BossKills[(int32)D]; }

	// Enter a dungeon and begin descent.
	bool EnterDungeon(EDungeon D);
	// Generate the current floor.
	FDungeonFloor GenerateFloor();
	// Attempt to clear the current floor (combat check).
	bool ClearFloor();
	// Get loot from current floor.
	FString GetLoot() const;
	// Exit the dungeon.
	void ExitDungeon();

private:
	void SeedDungeons();
	FDungeonFloor CurrentFloorState;
	TArray<FDungeonDef> Dungeons;
	EDungeon ActiveDungeon = EDungeon::NexusDepths;
	bool bActive = false;
	int32 CurrentFloor = 1;
	int32 DeepestFloors[5] = {0};
	int32 BossKills[5] = {0};
};
