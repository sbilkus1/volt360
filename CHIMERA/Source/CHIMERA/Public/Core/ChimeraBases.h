// GDD 14.2 — Base Building & Territory Control. Per-ring base ownership, staff, upgrades.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraBases.generated.h"

USTRUCT(BlueprintType)
struct FMBaseDef
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString Ring;       // which ring it's in
	UPROPERTY() FString Type;       // penthouse, lair, castle, garage, warehouse
	UPROPERTY() int32 Cost = 0;
	UPROPERTY() int32 Income = 0;   // credits/hour
	UPROPERTY() FString Bonus;      // unique bonus text
};

USTRUCT(BlueprintType)
struct FMBaseStaff
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() FString Role;       // guard, researcher, mechanic, cook, dragon handler
	UPROPERTY() int32 Level = 1;
	UPROPERTY() int32 Salary = 100;  // credits/hour
};

USTRUCT(BlueprintType)
struct FMPlayerBase
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Ring;
	UPROPERTY() int32 Level = 1;     // 1..5 base upgrade level
	UPROPERTY() int32 Defense = 0;    // 0..100
	UPROPERTY() TArray<FMBaseStaff> Staff;
	UPROPERTY() TArray<FString> Upgrades;
};

UCLASS()
class CHIMERA_API UBaseBuildingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const TArray<FMBaseDef>& GetCatalog() const { return Catalog; }
	const TArray<FMPlayerBase>& GetBases() const { return Bases; }

	bool BuyBase(const FString& Id);
	bool UpgradeBase(const FString& Id);
	bool HireStaff(const FString& BaseId, const FString& Name, const FString& Role);
	int32 GetTotalIncome() const;

private:
	void SeedCatalog();
	TArray<FMBaseDef> Catalog;
	TArray<FMPlayerBase> Bases;
};
