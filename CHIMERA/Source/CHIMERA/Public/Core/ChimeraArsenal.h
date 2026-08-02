// GDD 6.1 — Weapon proficiency, mods, dual-wield, bounty blueprints.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/ChimeraWeapons.h"
#include "ChimeraArsenal.generated.h"

// ======================================================================
// 1. WEAPON PROFICIENCY — levels per weapon class
// ======================================================================
USTRUCT() struct FWeaponProficiency
{
	GENERATED_BODY()
	int32 Level = 1;          // 1-100
	int32 XP = 0;
	int32 Kills = 0;
	EWeaponClass Class = EWeaponClass::Blade;
};
inline FString PerkName(int32 Lvl) {
	if (Lvl >= 75) return TEXT("Mastery: +30% damage, +20% speed");
	if (Lvl >= 50) return TEXT("Expert: +20% damage, +15% speed, special move unlocked");
	if (Lvl >= 25) return TEXT("Adept: +10% damage, +10% speed");
	if (Lvl >= 10) return TEXT("Novice: +5% damage");
	return TEXT("Untrained: no bonus");
}

// ======================================================================
// 2. WEAPON MODS
// ======================================================================
USTRUCT() struct FWeaponMod
{
	GENERATED_BODY()
	FString Id; FString Name; FString Slot; int32 Cost; FString Effect;
	EWeaponClass TargetClass = EWeaponClass::COUNT; // COUNT = all
};
// Mod slots: Scope, Barrel, Grip, Magazine, Stock, Enchantment

// ======================================================================
// 3. DUAL-WIELD STATE
// ======================================================================
USTRUCT() struct FDualWieldState
{
	GENERATED_BODY()
	bool bActive = false;
	FString OffhandWeapon;
	float OffhandDamage = 0.f;
	float OffhandSpeed = 1.f;
};

// ======================================================================
// 4. BOUNTY BLUEPRINT REWARDS
// ======================================================================
USTRUCT() struct FBountyBlueprint
{
	GENERATED_BODY()
	FString WeaponBlueprintId; // craftable weapon unlocked by this bounty
	FString BountyDescription;
	int32 MinBountiesCompleted = 5; // unlock after this many bounties
	bool bUnlocked = false;
};

UCLASS()
class CHIMERA_API UArsenalSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;

	// === PROFICIENCY ===
	const TArray<FWeaponProficiency>& GetProficiencies() const { return Proficiencies; }
	FWeaponProficiency* GetProficiency(EWeaponClass Cls);
	void AddWeaponXP(EWeaponClass Cls, int32 XP);
	float GetDamageMultiplier(EWeaponClass Cls) const;
	float GetSpeedMultiplier(EWeaponClass Cls) const;

	// === WEAPON MODS ===
	const TArray<FWeaponMod>& GetModCatalog() const { return Mods; }
	bool InstallMod(const FString& WeaponId, const FString& ModId);
	const TArray<FString>& GetInstalledMods(const FString& WeaponId) const;
	int32 GetModCount(const FString& WeaponId) const;

	// === DUAL-WIELD ===
	void ToggleDualWield();
	bool CanDualWield() const;
	FDualWieldState& GetDualState() { return DualState; }

	// === BOUNTY BLUEPRINTS ===
	const TArray<FBountyBlueprint>& GetBountyBlueprints() const { return BountyBlueprints; }
	void CheckBountyUnlocks(int32 TotalBounties);

private:
	void SeedMods();
	void SeedBountyBlueprints();
	TArray<FWeaponProficiency> Proficiencies;
	TArray<FWeaponMod> Mods;
	TArray<FBountyBlueprint> BountyBlueprints;
	TMap<FString, TArray<FString>> InstalledMods; // weapon ID -> mod IDs
	FDualWieldState DualState;
};
