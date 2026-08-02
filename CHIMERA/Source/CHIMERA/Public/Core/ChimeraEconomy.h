// GDD 9 (economy) + 6.10 (management). BAWSAQ 2.0 global stock market driven by
// player + simulated world actions, plus passive management sims (hospital, F1,
// police) that all run simultaneously in the background.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "ChimeraEconomy.generated.h"

UCLASS()
class CHIMERA_API UBAWSAQSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// GDD 9.2 - every player action across every server feeds the market.
	void ApplyEvent(const FName& Tag);
	void BuyStock(int32 Index, int32 Shares);
	const TArray<FStockDef>& GetStocks() const { return Stocks; }

private:
	bool Tick(float DeltaTime);
	TArray<FStockDef> Stocks;
	TArray<int32> Holdings;
	FTSTicker::FDelegateHandle TickerHandle;
};

UCLASS()
class CHIMERA_API UManagementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	float HospitalRating = 0.f;   // GDD 4.4 (Two Point Hospital)
	int32 HospitalCures = 0;
	int32 F1ChampionshipPoints = 0; // GDD 4.8
	float ClearanceRate = 0.5f;   // GDD 4.5 police
	float CrimeIndex = 0.5f;

	FString HospitalName;
	FString F1TeamName;

	void OnSurgeryDone(bool bSuccess);
	void OnArrest();
	void OnRaceFinished(int32 Position);
	void OnCrimeSpike();

private:
	bool Tick(float DeltaTime);
	FTSTicker::FDelegateHandle TickerHandle;
};
