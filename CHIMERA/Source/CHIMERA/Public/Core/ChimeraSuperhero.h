// GDD 4.2 — Aether City: 18 superhero power archetypes, suits, weapons.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraSuperhero.generated.h"

UENUM(BlueprintType)
enum class EPowerArchetype : uint8
{
	Flight, SuperStrength, SuperSpeed, Telekinesis, Telepathy,
	EnergyBlast, ForceField, Shapeshift, Invisibility, Regeneration,
	Pyrokinesis, Cryokinesis, Electrokinesis, Technopathy,
	Portals, SizeShift, GravityControl, TimeDilation,
	COUNT
};

USTRUCT(BlueprintType)
struct CHIMERA_API FSuperPowerDef
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() EPowerArchetype Type = EPowerArchetype::Flight;
	UPROPERTY() FString ActiveDesc;   // what it does in combat
	UPROPERTY() FString PassiveDesc;  // what it gives passively
	UPROPERTY() bool bCombat = true;  // usable in combat
	UPROPERTY() int32 UnlockLevel = 1;
	UPROPERTY() int32 CooldownSecs = 5;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FSuperPowerState
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() int32 Level = 1;   // 1..100
	UPROPERTY() int32 XP = 0;
	UPROPERTY() float Cooldown = 0.f;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FSuperSuitDef
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString Material;
	UPROPERTY() FString VisualStyle;
	UPROPERTY() int32 Armor = 5;        // 1..10
	UPROPERTY() int32 Mobility = 5;
	UPROPERTY() int32 PowerAmp = 5;     // amplifies power effectiveness
	UPROPERTY() int32 Stealth = 5;
	UPROPERTY() bool bHasFlightStabilizers = false;
	UPROPERTY() FString SpecialTrait;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FSuperWeaponDef
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString DamageType;  // energy, kinetic, plasma, cryo, sonic
	UPROPERTY() int32 BaseDamage = 30;
	UPROPERTY() int32 Range = 1;     // 0=melee, 1=mid, 2=long
	UPROPERTY() FString SpecialEffect;
};

UCLASS()
class CHIMERA_API USuperheroSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Power catalog.
	const TArray<FSuperPowerDef>& GetPowerCatalog() const { return PowerCatalog; }
	FSuperPowerState* GetPowerState(const FString& Id);
	const TArray<FSuperPowerState>& GetPowers() const { return Powers; }

	// Unlock a new power.
	bool UnlockPower(const FString& Id);
	// Add XP to a power (from combat/use).
	void AddPowerXP(const FString& Id, int32 XP);
	// Use a power in combat — returns damage dealt (0 if on cooldown).
	int32 ActivatePower(const FString& Id);

	// Suit catalog.
	const TArray<FSuperSuitDef>& GetSuitCatalog() const { return SuitCatalog; }
	void EquipSuit(const FString& Id);
	const FSuperSuitDef* GetEquippedSuit() const { return EquippedIdx >= 0 ? &SuitCatalog[EquippedIdx] : nullptr; }
	int32 GetEquippedSuitIndex() const { return EquippedIdx; }

	// Weapon catalog.
	const TArray<FSuperWeaponDef>& GetWeaponCatalog() const { return WeaponCatalog; }
	void EquipWeapon(const FString& Id);
	const FSuperWeaponDef* GetEquippedWeapon() const { return WeaponIdx >= 0 ? &WeaponCatalog[WeaponIdx] : nullptr; }

	// === Craftable Hero/Villain Blueprint system (Marvel + DC) ===
	USTRUCT(BlueprintType)
	struct FHeroBlueprint
	{
		GENERATED_BODY()
		UPROPERTY() FString Id;
		UPROPERTY() FString Name;
		UPROPERTY() FString Universe;     // "Marvel" or "DC"
		UPROPERTY() FString Alias;        // civilian name
		UPROPERTY() FString Type;         // "Suit", "Weapon", "Power"
		UPROPERTY() FString Description;
		UPROPERTY() TArray<FString> Ingredients;
		UPROPERTY() int32 CraftLevel = 1;
		UPROPERTY() bool bCrafted = false;
	};
	const TArray<FHeroBlueprint>& GetBlueprints() const { return Blueprints; }
	bool CraftBlueprint(const FString& Id);
	const FHeroBlueprint* GetBlueprint(const FString& Id) const;

private:
	void SeedBlueprints();
	TArray<FHeroBlueprint> Blueprints;
};
