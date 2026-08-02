#include "Core/ChimeraEconomy.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UBAWSAQSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// GDD 9.1/9.2 - market seeds.
	struct Seed { const char* Sym; const char* Name; float Price; float Vol; };
	static const Seed Seeds[] = {
		{ "PHAR", "Nexus Pharma", 100.f, 0.04f },
		{ "INSR", "Chimera Mutual", 80.f, 0.03f },
		{ "SECR", "Blackwall Security", 120.f, 0.05f },
		{ "CONS", "Frontier Concrete", 60.f, 0.03f },
		{ "FUNR", "Eternal Rest", 40.f, 0.02f },
		{ "SPRT", "Coliseum Sports", 150.f, 0.06f },
		{ "TECH", "Arasaka-Nex", 220.f, 0.07f },
		{ "AUTO", "Meridian Motors", 90.f, 0.04f },
		{ "MEDI", "Night City Media", 70.f, 0.03f },
		{ "DEFN", "Aether Defense", 110.f, 0.05f },
		{ "AGRO", "Alagaesia Herbs", 50.f, 0.02f },
		{ "ENRG", "Reality Power Grid", 75.f, 0.03f }
	};
	for (const Seed& S : Seeds)
	{
		FStockDef D;
		D.Symbol = FString(S.Sym);
		D.Name = FString(S.Name);
		D.Price = S.Price;
		D.Volatility = S.Vol;
		Stocks.Add(D);
		Holdings.Add(0);
	}
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UBAWSAQSubsystem::Tick), 2.0f);
}

void UBAWSAQSubsystem::Deinitialize()
{
	if (TickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	}
	Super::Deinitialize();
}

bool UBAWSAQSubsystem::Tick(float DeltaTime)
{
	// Random drift + mean reversion.
	for (FStockDef& S : Stocks)
	{
		float Drift = FMath::FRandRange(-S.Volatility, S.Volatility);
		S.Price = FMath::Max(5.f, S.Price * (1.f + Drift));
	}
	return true;
}

void UBAWSAQSubsystem::ApplyEvent(const FName& Tag)
{
	auto Move = [&](int32 Sym, float Mult)
	{
		if (Sym >= 0 && Sym < Stocks.Num()) Stocks[Sym].Price *= Mult;
	};
	// GDD 9.2 mapping: hospital cure -> pharma up; dragon attacks -> insurance+construction;
	// super casualties -> funeral; race results -> sponsor; crime -> security.
	if (Tag == "hospital_cure") { Move(0, 1.06f); Move(1, 0.98f); }
	else if (Tag == "crime_wave") { Move(2, 1.05f); Move(0, 0.99f); }
	else if (Tag == "dragon_attack") { Move(1, 1.04f); Move(3, 1.03f); }
	else if (Tag == "collateral_damage") { Move(4, 1.06f); }
	else if (Tag == "race_win") { Move(5, 1.04f); Move(8, 1.02f); }
	else if (Tag == "super_heroics") { Move(5, 1.03f); Move(2, 1.02f); }
	else if (Tag == "hack") { Move(6, 1.05f); Move(2, 0.98f); }
	else if (Tag == "disaster") { Move(1, 1.05f); Move(3, 1.04f); Move(9, 1.03f); }
	else if (Tag == "arrest") { Move(2, 1.02f); Move(10, 0.99f); }
}

void UBAWSAQSubsystem::BuyStock(int32 Index, int32 Shares)
{
	if (Index < 0 || Index >= Stocks.Num()) return;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		int32 Cost = (int32)(Stocks[Index].Price * Shares);
		if (Sess->GetCurrency(ECurrency::Credits) >= Cost)
		{
			Sess->AddCurrency(ECurrency::Credits, -Cost);
			Holdings[Index] += Shares;
			Sess->RecordEvent(FString::Printf(TEXT("Bought %d x %s for %d credits"), Shares, *Stocks[Index].Symbol, Cost));
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 6.10 - Management systems (all run simultaneously, delegate to AI).
// ---------------------------------------------------------------------------
void UManagementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	HospitalName = TEXT("Mercy Heights - St. Nexus Wing");
	F1TeamName = TEXT("Chimera Racing");
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UManagementSubsystem::Tick), 5.0f);
}

void UManagementSubsystem::Deinitialize()
{
	if (TickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	}
	Super::Deinitialize();
}

bool UManagementSubsystem::Tick(float DeltaTime)
{
	// Hospital rating from cure throughput; crime index wanders; F1 points stable.
	float TargetRating = FMath::Clamp(5.f + HospitalCures * 0.5f, 0.f, 100.f);
	HospitalRating = FMath::Lerp(HospitalRating, TargetRating, 0.05f);
	CrimeIndex = FMath::Clamp(CrimeIndex + FMath::FRandRange(-0.02f, 0.02f), 0.f, 1.f);
	return true;
}

void UManagementSubsystem::OnSurgeryDone(bool bSuccess)
{
	if (bSuccess)
	{
		HospitalCures++;
		if (auto* B = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>()) B->ApplyEvent(TEXT("hospital_cure"));
	}
}

void UManagementSubsystem::OnArrest()
{
	ClearanceRate = FMath::Clamp(ClearanceRate + 0.01f, 0.f, 1.f);
	if (auto* B = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>()) B->ApplyEvent(TEXT("arrest"));
}

void UManagementSubsystem::OnRaceFinished(int32 Position)
{
	if (Position <= 10)
	{
		F1ChampionshipPoints += FMath::Max(0, 26 - Position);
		if (auto* B = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>()) B->ApplyEvent(TEXT("race_win"));
	}
}

void UManagementSubsystem::OnCrimeSpike()
{
	CrimeIndex = FMath::Min(1.f, CrimeIndex + 0.1f);
	if (auto* B = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>()) B->ApplyEvent(TEXT("crime_wave"));
}
