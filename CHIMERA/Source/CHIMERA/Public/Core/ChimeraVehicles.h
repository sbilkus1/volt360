// GDD 6.5 / 14 — Vehicle system. GTA V-style: steal, buy, sell, upgrade, garage.
// 100+ real cars from major manufacturers since 1980.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraVehicles.generated.h"

UENUM(BlueprintType)
enum class EVehicleClass : uint8 { Compact, Sedan, SUV, Sports, Super, Muscle, Truck, Motorcycle, Classic, Rally, Hypercar, COUNT };

UENUM(BlueprintType)
enum class EUpgradeCategory : uint8 { Engine, Brakes, Transmission, Turbo, Suspension, Armor, Tires, Nitrous, COUNT };

USTRUCT(BlueprintType)
struct CHIMERA_API FVehicleUpgrade
{
	GENERATED_BODY()
	UPROPERTY() int32 Level = 0;   // 0=stock, 1=street, 2=sport, 3=race, 4=elite
	UPROPERTY() int32 Cost = 0;    // credits to install
	UPROPERTY() FString Name;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FVehicleDef
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Make;
	UPROPERTY() FString Model;
	UPROPERTY() int32 Year = 2000;
	UPROPERTY() EVehicleClass VClass = EVehicleClass::Sedan;
	UPROPERTY() int32 BaseValue = 15000;  // credits
	UPROPERTY() int32 TopSpeed = 180;     // km/h
	UPROPERTY() int32 Acceleration = 50;  // 1..100
	UPROPERTY() int32 Handling = 50;
	UPROPERTY() FString Country;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FPlayerVehicle
{
	GENERATED_BODY()
	UPROPERTY() FString Id;                    // which car
	UPROPERTY() FString Plate;                 // generated license plate
	UPROPERTY() bool bOwned = false;           // bought = true, stolen = false
	UPROPERTY() bool bInsured = false;
	UPROPERTY() int32 Condition = 100;          // 0..100, decreases with crashes
	UPROPERTY() int32 Mileage = 0;
	UPROPERTY() int32 Upgrades[(int32)EUpgradeCategory::COUNT] = {0}; // upgrade levels
};

UCLASS()
class CHIMERA_API UVehicleSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Catalog.
	const TArray<FVehicleDef>& GetCatalog() const { return Catalog; }
	const FVehicleDef* FindVehicle(const FString& Id) const;

	// Player garage (max 20 vehicles).
	const TArray<FPlayerVehicle>& GetGarage() const { return Garage; }
	int32 GetGarageSlots() const { return GarageSlots; }
	void ExpandGarage() { GarageSlots += 5; }

	// Actions.
	bool BuyVehicle(const FString& Id);
	bool StealVehicle(const FString& Id);
	bool SellVehicle(int32 GarageIdx);
	bool StoreVehicle(const FString& Id, bool bOwned);
	int32 GetVehicleValue(const FPlayerVehicle& V) const;

	// Upgrades.
	bool ApplyUpgrade(int32 GarageIdx, EUpgradeCategory Cat);
	int32 GetUpgradeCost(int32 Level) const;
	const FVehicleUpgrade& GetUpgradeInfo(EUpgradeCategory Cat, int32 Level) const;

private:
	void SeedCatalog();
	void SeedUpgrades();

	TArray<FVehicleDef> Catalog;
	TArray<FPlayerVehicle> Garage;
	TArray<FVehicleUpgrade> UpgradeCatalog[8]; // per category
	int32 GarageSlots = 10;
};
