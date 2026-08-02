#include "Core/ChimeraArsenal.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UArsenalSubsystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	for (int32 i = 0; i < (int32)EWeaponClass::COUNT; ++i)
	{
		FWeaponProficiency P; P.Class = (EWeaponClass)i; Proficiencies.Add(P);
	}
	SeedMods();
	SeedBountyBlueprints();
}

// ======================================================================
// PROFICIENCY
// ======================================================================
FWeaponProficiency* UArsenalSubsystem::GetProficiency(EWeaponClass Cls)
{
	for (auto& P : Proficiencies) if (P.Class == Cls) return &P;
	return nullptr;
}

void UArsenalSubsystem::AddWeaponXP(EWeaponClass Cls, int32 XP)
{
	auto* P = GetProficiency(Cls);
	if (!P) return;
	P->XP += XP; P->Kills++;
	int32 Need = P->Level * 50;
	while (P->XP >= Need) { P->XP -= Need; P->Level++; Need = P->Level * 50;
		auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (Sess && (P->Level == 10 || P->Level == 25 || P->Level == 50 || P->Level == 75 || P->Level == 100))
			Sess->ShowMessage(FString::Printf(TEXT("Weapon Mastery: %s Level %d — %s"),
				*StaticEnum<EWeaponClass>()->GetNameStringByValue((int64)Cls), P->Level, *PerkName(P->Level))); }
}

float UArsenalSubsystem::GetDamageMultiplier(EWeaponClass Cls) const
{
	for (auto& P : Proficiencies) if (P.Class == Cls) {
		if (P.Level >= 75) return 1.3f; if (P.Level >= 50) return 1.2f;
		if (P.Level >= 25) return 1.1f; if (P.Level >= 10) return 1.05f; return 1.f; }
	return 1.f;
}

float UArsenalSubsystem::GetSpeedMultiplier(EWeaponClass Cls) const
{
	for (auto& P : Proficiencies) if (P.Class == Cls) {
		if (P.Level >= 75) return 1.2f; if (P.Level >= 50) return 1.15f;
		if (P.Level >= 25) return 1.1f; return 1.f; }
	return 1.f;
}

// ======================================================================
// WEAPON MODS
// ======================================================================
void UArsenalSubsystem::SeedMods()
{
	auto M = [&](const TCHAR* Id, const TCHAR* Nm, const TCHAR* Slot, int32 Cost, const TCHAR* Eff, EWeaponClass Cls = EWeaponClass::COUNT) {
		FWeaponMod Mo; Mo.Id = Id; Mo.Name = Nm; Mo.Slot = Slot; Mo.Cost = Cost; Mo.Effect = Eff; Mo.TargetClass = Cls; Mods.Add(Mo);
	};
	M(TEXT("red_dot"), TEXT("Red Dot Sight"), TEXT("Scope"), 2000, TEXT("+15% accuracy at range. Less zoom than ACOG, faster target acquisition."));
	M(TEXT("acog_4x"), TEXT("ACOG 4x Scope"), TEXT("Scope"), 4000, TEXT("+30% accuracy at range. Zoomed. Best for rifles and marksman weapons."));
	M(TEXT("suppressor"), TEXT("Suppressor"), TEXT("Barrel"), 3000, TEXT("Stealth shots: enemies don't alert others. -10% damage. Quiet."));
	M(TEXT("extended_barrel"), TEXT("Extended Barrel"), TEXT("Barrel"), 2500, TEXT("+20% range. +10% damage. Longer barrel = faster bullet = harder hit."));
	M(TEXT("ergo_grip"), TEXT("Ergonomic Grip"), TEXT("Grip"), 1500, TEXT("+15% weapon swap speed. +10% accuracy. Fits your hand perfectly."));
	M(TEXT("extended_mag"), TEXT("Extended Magazine"), TEXT("Magazine"), 2500, TEXT("+50% magazine capacity. More shots between reloads. Heavier."));
	M(TEXT("quick_mag"), TEXT("Speed Loader"), TEXT("Magazine"), 2000, TEXT("Reloads 40% faster. Competition-grade magazine well."));
	M(TEXT("collapsible_stock"), TEXT("Collapsible Stock"), TEXT("Stock"), 1800, TEXT("+15% movement speed while aiming. Compact. Tactical."));
	M(TEXT("flame_rune"), TEXT("Flame Rune Enchant"), TEXT("Enchantment"), 5000, TEXT("Adds fire damage. Blades ignite on hit. Bullets leave burning trails. Ancient Language fire."));
	M(TEXT("ice_rune"), TEXT("Frost Rune Enchant"), TEXT("Enchantment"), 5000, TEXT("Adds ice damage. Slows enemies by 30%. Freezes water on contact."));
	M(TEXT("lightning_rune"), TEXT("Lightning Rune Enchant"), TEXT("Enchantment"), 6000, TEXT("Adds lightning damage. Chain lightning jumps to 2 nearby enemies. Storm essence."));
	M(TEXT("vampire_rune"), TEXT("Vampire Rune Enchant"), TEXT("Enchantment"), 8000, TEXT("Leeches 5% of damage dealt as health. Dark magic. Morally questionable. Effective."));
}

bool UArsenalSubsystem::InstallMod(const FString& WeaponId, const FString& ModId)
{
	for (auto& Mo : Mods)
	{
		if (Mo.Id != ModId) continue;
		auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Mo.Cost) return false;
		Sess->AddCurrency(ECurrency::Credits, -Mo.Cost);
		InstalledMods.FindOrAdd(WeaponId).Add(ModId);
		Sess->ShowMessage(FString::Printf(TEXT("Mod installed: %s — %s"), *Mo.Name, *Mo.Effect));
		return true;
	}
	return false;
}

const TArray<FString>& UArsenalSubsystem::GetInstalledMods(const FString& WeaponId) const
{
	static TArray<FString> Empty;
	auto* Found = InstalledMods.Find(WeaponId);
	return Found ? *Found : Empty;
}

int32 UArsenalSubsystem::GetModCount(const FString& WeaponId) const { return GetInstalledMods(WeaponId).Num(); }

// ======================================================================
// DUAL-WIELD
// ======================================================================
void UArsenalSubsystem::ToggleDualWield()
{
	DualState.bActive = !DualState.bActive;
	auto* WS = GetGameInstance()->GetSubsystem<UWeaponSystem>();
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!WS || !Sess) return;
	if (DualState.bActive)
	{
		const FWeaponDef* Main = WS->GetEquipped();
		if (!Main) { DualState.bActive = false; return; }
		// Find an offhand of same class or dagger
		for (auto& W : WS->GetCatalog())
		{
			if (W.Id != Main->Id && W.bCrafted && (W.Class == Main->Class || W.Class == EWeaponClass::Dagger))
			{
				DualState.OffhandWeapon = W.Id;
				DualState.OffhandDamage = W.Damage * 0.6f; // offhand does 60% damage
				DualState.OffhandSpeed = W.Speed;
				Sess->ShowMessage(FString::Printf(TEXT("Dual-wielding: %s + %s. Offhand: 60%% damage."), *Main->Name, *W.Name));
				return;
			}
		}
		DualState.bActive = false;
		Sess->ShowMessage(TEXT("No compatible offhand weapon crafted. Craft another blade or dagger first."));
	}
	else Sess->ShowMessage(TEXT("Dual-wield deactivated. Single weapon focus restored."));
}

bool UArsenalSubsystem::CanDualWield() const
{
	const auto* WS = GetGameInstance()->GetSubsystem<UWeaponSystem>();
	const FWeaponDef* Main = WS ? WS->GetEquipped() : nullptr;
	return Main && (Main->Class == EWeaponClass::Sword || Main->Class == EWeaponClass::Dagger || Main->Class == EWeaponClass::Blade);
}

// ======================================================================
// BOUNTY BLUEPRINTS
// ======================================================================
void UArsenalSubsystem::SeedBountyBlueprints()
{
	auto B = [&](const TCHAR* WepId, const TCHAR* Desc, int32 Min) {
		FBountyBlueprint BB; BB.WeaponBlueprintId = WepId; BB.BountyDescription = Desc; BB.MinBountiesCompleted = Min; BountyBlueprints.Add(BB);
	};
	B(TEXT("brisingr"), TEXT("A Rider's true blade. Forged in fire and named in battle."), 10);
	B(TEXT("zarroc"), TEXT("The sword of Morzan. Red as the blood it's spilled."), 8);
	B(TEXT("excalibur"), TEXT("Only the worthy may claim it. The Convergence's chosen."), 50);
	B(TEXT("lightsaber_blue"), TEXT("A weapon from a galaxy that may or may not have been part of the Crash."), 30);
	B(TEXT("finnick_trident"), TEXT("District 4's finest. The golden boy's golden weapon."), 5);
	B(TEXT("katniss_bow"), TEXT("The Mockingjay's bow. Feeds families. Starts revolutions."), 3);
	B(TEXT("tris_knife"), TEXT("The knife Tris chose on her first day. It saved her life."), 2);
	B(TEXT("four_knife"), TEXT("Four's throwing knives. Precise. Deadly. Reluctant."), 4);
}

void UArsenalSubsystem::CheckBountyUnlocks(int32 TotalBounties)
{
	for (auto& BB : BountyBlueprints)
	{
		if (!BB.bUnlocked && TotalBounties >= BB.MinBountiesCompleted)
		{
			BB.bUnlocked = true;
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (Sess) Sess->ShowMessage(FString::Printf(TEXT("BOUNTY REWARD UNLOCKED: %s — %s"), *BB.WeaponBlueprintId, *BB.BountyDescription));
		}
	}
}
