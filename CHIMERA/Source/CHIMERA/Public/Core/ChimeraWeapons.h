// GDD 6.1 — Complete Weapon Catalog. Every weapon from every universe.
// Dragon Rider swords (Eragon), Dauntless arsenal (Divergent), District weapons (Hunger Games),
// surgical tools (Grey's), NPD sidearms (The Rookie), superhero weapons, and legendary blades.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraWeapons.generated.h"

UENUM() enum class EWeaponClass : uint8 { Blade, Sword, Dagger, Bow, Gun, Rifle, Shotgun, Sniper, Spear, Axe, Blunt, Thrown, Explosive, Serum, Shield, Tool, COUNT };

USTRUCT() struct FWeaponDef
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString Universe;    // "Eragon", "Divergent", "Hunger Games", "Greys", "The Rookie", "General"
	UPROPERTY() FString Wielder;     // who canonically uses/wields this
	UPROPERTY() EWeaponClass Class = EWeaponClass::Sword;
	UPROPERTY() int32 Damage = 25;
	UPROPERTY() float Speed = 1.f;   // attack speed multiplier
	UPROPERTY() FString Material;    // what it's made of
	UPROPERTY() FString Color;       // blade color (for Rider swords — matches dragon)
	UPROPERTY() FString Special;     // unique ability
	UPROPERTY() FString Lore;        // story behind the weapon
	UPROPERTY() int32 CreditCost = 1000;
	UPROPERTY() bool bCrafted = false;
	UPROPERTY() TArray<FString> RequiredPieces;
};

UCLASS()
class CHIMERA_API UWeaponSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FWeaponDef>& GetCatalog() const { return Catalog; }
	const FWeaponDef* Find(const FString& Id) const;
	bool CraftWeapon(const FString& Id);
	FString GetEquippedWeapon() const { return EquippedWeapon; }
	void Equip(const FString& Id);
	const FWeaponDef* GetEquipped() const { return Find(EquippedWeapon); }

private:
	void SeedCatalog();
	TArray<FWeaponDef> Catalog;
	FString EquippedWeapon;
};
