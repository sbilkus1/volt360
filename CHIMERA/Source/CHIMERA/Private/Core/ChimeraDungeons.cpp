#include "Core/ChimeraDungeons.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UDungeonSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedDungeons();
}

void UDungeonSubsystem::SeedDungeons()
{
	FDungeonDef D;
	D.Type = EDungeon::NexusDepths; D.Name = TEXT("Nexus Tower Depths"); D.Ring = TEXT("Nexus");
	D.Flavor = TEXT("Endless procedural descent through the Tower's basement. Reality fractures the deeper you go. Floors 100-500."); D.MaxFloors = 500; D.BossInterval = 5; Dungeons.Add(D);

	D.Type = EDungeon::BlackwallRift; D.Name = TEXT("The Blackwall Rift"); D.Ring = TEXT("Night City");
	D.Flavor = TEXT("Cyberspace raid. Digital daemons patrol the Blackwall. ICE programs that fight back."); D.MaxFloors = 300; D.BossInterval = 6; Dungeons.Add(D);

	D.Type = EDungeon::DragonGraveyard; D.Name = TEXT("The Dragon Graveyard"); D.Ring = TEXT("Alagaesia");
	D.Flavor = TEXT("Where elder dragons go to die. Their spirits guard legendary treasure. The bones of Urû'baen litter the floor."); D.MaxFloors = 250; D.BossInterval = 4; Dungeons.Add(D);

	D.Type = EDungeon::ForgottenWard; D.Name = TEXT("The Forgotten Ward"); D.Ring = TEXT("Mercy Heights");
	D.Flavor = TEXT("An abandoned hospital wing that warps reality. Patients who were never discharged. Surgeons who never stopped operating."); D.MaxFloors = 200; D.BossInterval = 5; Dungeons.Add(D);

	D.Type = EDungeon::StadiumChampions; D.Name = TEXT("Stadium of Champions"); D.Ring = TEXT("Grand Coliseum");
	D.Flavor = TEXT("Boss rush against legendary athletes, racing champions, and sporting icons from across the multiverse."); D.MaxFloors = 100; D.BossInterval = 3; Dungeons.Add(D);
}

bool UDungeonSubsystem::EnterDungeon(EDungeon D)
{
	if (bActive) return false;
	ActiveDungeon = D; bActive = true; CurrentFloor = 1;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->ShowMessage(FString::Printf(TEXT("ENTERING: %s. Floor 1. Good luck."), *Dungeons[(int32)D].Name));
		Sess->RecordEvent(FString::Printf(TEXT("Entered dungeon: %s"), *Dungeons[(int32)D].Name));
	}
	GenerateFloor();
	return true;
}

FDungeonFloor UDungeonSubsystem::GenerateFloor()
{
	FDungeonFloor F; F.Floor = CurrentFloor;
	int32 FloorFactor = CurrentFloor / 10; // difficulty scales every 10 floors
	EDungeon D = ActiveDungeon;
	FDungeonDef& Def = Dungeons[(int32)D];

	bool bIsBoss = (CurrentFloor % Def.BossInterval == 0);
	F.bBossFloor = bIsBoss;

	if (bIsBoss)
	{
		switch (D)
		{
		case EDungeon::NexusDepths: F.BossName = CurrentFloor % 25 == 0 ? TEXT("The Reality Aberration") : TEXT("Keeper Wraith"); break;
		case EDungeon::BlackwallRift: F.BossName = CurrentFloor % 18 == 0 ? TEXT("ALT Cunningham") : TEXT("ICE Breaker Daemon"); break;
		case EDungeon::DragonGraveyard: F.BossName = CurrentFloor % 16 == 0 ? FString::Printf(TEXT("Elder Dragon - Floor %d"), CurrentFloor) : TEXT("Dragon Spirit"); break;
		case EDungeon::ForgottenWard: F.BossName = CurrentFloor % 20 == 0 ? TEXT("The Chief Surgeon") : TEXT("Phantom Patient"); break;
		case EDungeon::StadiumChampions: F.BossName = CurrentFloor % 12 == 0 ? TEXT("Senna's Ghost") : TEXT("Legendary Striker"); break;
		default: F.BossName = TEXT("Floor Boss"); break;
		}
		F.BossHP = 300 + FloorFactor * 200;
		F.EnemyCount = 0;
	}
	else
	{
		// Common enemies per dungeon
		switch (D)
		{
		case EDungeon::NexusDepths: F.EnemyType = FloorFactor % 3 == 0 ? TEXT("Reality Drone") : TEXT("Fractured Keeper"); break;
		case EDungeon::BlackwallRift: F.EnemyType = FloorFactor % 2 == 0 ? TEXT("Net Daemon") : TEXT("Corrupted ICE"); break;
		case EDungeon::DragonGraveyard: F.EnemyType = FloorFactor % 3 == 0 ? TEXT("Dragon Whelp") : TEXT("Bone Construct"); break;
		case EDungeon::ForgottenWard: F.EnemyType = FloorFactor % 2 == 0 ? TEXT("Spectral Patient") : TEXT("Rogue Med-Drone"); break;
		case EDungeon::StadiumChampions: F.EnemyType = FloorFactor % 3 == 0 ? TEXT("Rival Racer") : TEXT("Defender"); break;
		default: F.EnemyType = TEXT("Enemy"); break;
		}
		F.EnemyCount = 2 + FloorFactor;
		F.EnemyHP = 40 + FloorFactor * 25;
		F.EnemyDamage = 8 + FloorFactor * 4;
	}

	// Loot drop
	if (bIsBoss) F.LootDrop = TEXT("Legendary Materials + Boss Key");
	else if (CurrentFloor % 3 == 0) F.LootDrop = TEXT("Rare Crafting Component");
	else F.LootDrop = TEXT("Credits + Common Materials");

	CurrentFloorState = F;
	return F;
}

bool UDungeonSubsystem::ClearFloor()
{
	if (!bActive) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;

	int32 PlayerPower = Sess->GetSkillLevel(FName("Combat_Brawling")) + Sess->GetSkillLevel(FName("Combat_Blades")) + Sess->GetSkillLevel(FName("Magic_AncientLanguage")) + Sess->GetSkillLevel(FName("Combat_Stealth"));
	int32 EnemyPower = CurrentFloorState.bBossFloor ? CurrentFloorState.BossHP / 5 : CurrentFloorState.EnemyCount * 30 + CurrentFloor;

	bool bSuccess = FMath::RandRange(0, PlayerPower + 10) >= EnemyPower / 2;

	if (bSuccess)
	{
		// Record deepest
		if (CurrentFloor > DeepestFloors[(int32)ActiveDungeon])
			DeepestFloors[(int32)ActiveDungeon] = CurrentFloor;
		if (CurrentFloorState.bBossFloor) BossKills[(int32)ActiveDungeon]++;

		Sess->AddSkillXP(FName("Combat_Brawling"), 15 + CurrentFloor);
		Sess->ShowMessage(FString::Printf(TEXT("Floor %d cleared! %s"), CurrentFloor, CurrentFloorState.bBossFloor ? TEXT("BOSS DEFEATED!") : TEXT("Loot collected.")));
		Sess->AddCurrency(ECurrency::Credits, CurrentFloor * 200);

		CurrentFloor++;
		GenerateFloor();
		return true;
	}

	Sess->ShowMessage(FString::Printf(TEXT("DEFEATED at Floor %d. Deepest: %d. Better luck next run."), CurrentFloor, DeepestFloors[(int32)ActiveDungeon]));
	bActive = false;
	return false;
}

FString UDungeonSubsystem::GetLoot() const
{
	return CurrentFloorState.bBossFloor ?
		FString::Printf(TEXT("BOSS LOOT: %s dropped %s"), *CurrentFloorState.BossName, *CurrentFloorState.LootDrop) :
		FString::Printf(TEXT("Floor %d: %d %s(s). %s"), CurrentFloorState.Floor, CurrentFloorState.EnemyCount, *CurrentFloorState.EnemyType, *CurrentFloorState.LootDrop);
}

void UDungeonSubsystem::ExitDungeon()
{
	if (!bActive) return;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(FString::Printf(TEXT("Exited %s at Floor %d. Deepest: %d."),
			*Dungeons[(int32)ActiveDungeon].Name, CurrentFloor, DeepestFloors[(int32)ActiveDungeon]));
	bActive = false;
}
