#include "Core/ChimeraBases.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UBaseBuildingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedCatalog();
}

void UBaseBuildingSubsystem::SeedCatalog()
{
	FMBaseDef B;
	B.Id = TEXT("aether_penthouse"); B.Name = TEXT("Aether City Penthouse"); B.Ring = TEXT("Aether City");
	B.Type = TEXT("Penthouse"); B.Cost = 150000; B.Income = 500;
	B.Bonus = TEXT("Power research lab: +10% superpower XP gain"); Catalog.Add(B);

	B.Id = TEXT("aether_lair"); B.Name = TEXT("Underground Vigilante Lair"); B.Ring = TEXT("Aether City");
	B.Type = TEXT("Lair"); B.Cost = 200000; B.Income = 300;
	B.Bonus = TEXT("Suit fabrication bay: faster suit crafting"); Catalog.Add(B);

	B.Id = TEXT("mercy_clinic"); B.Name = TEXT("Private Clinic Wing"); B.Ring = TEXT("Mercy Heights");
	B.Type = TEXT("Clinic"); B.Cost = 120000; B.Income = 800;
	B.Bonus = TEXT("Surgery success rate +15%"); Catalog.Add(B);

	B.Id = TEXT("midwilshire_safehouse"); B.Name = TEXT("NPD Safehouse"); B.Ring = TEXT("Mid-Wilshire");
	B.Type = TEXT("Safehouse"); B.Cost = 80000; B.Income = 200;
	B.Bonus = TEXT("Evidence storage: investigation skill +10%"); Catalog.Add(B);

	B.Id = TEXT("neokingdom_tower"); B.Name = TEXT("Wizard Tower"); B.Ring = TEXT("Neo-Kingdom");
	B.Type = TEXT("Tower"); B.Cost = 180000; B.Income = 400;
	B.Bonus = TEXT("Spell research chamber: magic skill gain +20%"); Catalog.Add(B);

	B.Id = TEXT("alagasia_aerie"); B.Name = TEXT("Dragon Aerie"); B.Ring = TEXT("Alagaesia");
	B.Type = TEXT("Aerie"); B.Cost = 250000; B.Income = 350;
	B.Bonus = TEXT("Dragon bond grows 2x faster when at base"); Catalog.Add(B);

	B.Id = TEXT("frontier_build"); B.Name = TEXT("Builder's Plot"); B.Ring = TEXT("Minecraft Frontier");
	B.Type = TEXT("Plot"); B.Cost = 50000; B.Income = 100;
	B.Bonus = TEXT("Unlimited building. Every block is yours."); Catalog.Add(B);

	B.Id = TEXT("coliseum_garage"); B.Name = TEXT("Racing Team HQ"); B.Ring = TEXT("Grand Coliseum");
	B.Type = TEXT("Garage"); B.Cost = 100000; B.Income = 600;
	B.Bonus = TEXT("Vehicle upgrades cost 25% less"); Catalog.Add(B);

	B.Id = TEXT("nightcity_warehouse"); B.Name = TEXT("Fixer Warehouse"); B.Ring = TEXT("Night City");
	B.Type = TEXT("Warehouse"); B.Cost = 90000; B.Income = 450;
	B.Bonus = TEXT("Hacking daemon upload speed +30%"); Catalog.Add(B);

	B.Id = TEXT("seattle_oncall"); B.Name = TEXT("On-Call Room"); B.Ring = TEXT("Seattle");
	B.Type = TEXT("Room"); B.Cost = 40000; B.Income = 250;
	B.Bonus = TEXT("Faster recovery from surgery failures"); Catalog.Add(B);

	B.Id = TEXT("panem_victor"); B.Name = TEXT("Victor's Villa"); B.Ring = TEXT("Panem");
	B.Type = TEXT("Villa"); B.Cost = 300000; B.Income = 750;
	B.Bonus = TEXT("Tribute training XP +25%, arena sponsor gifts doubled"); Catalog.Add(B);

	B.Id = TEXT("ubu_flat6"); B.Name = TEXT("UBU Flat 6 — Beach Road"); B.Ring = TEXT("Cleveleys");
	B.Type = TEXT("Flat"); B.Cost = 15000; B.Income = 50;
	B.Bonus = TEXT("Safe space. Sea view. Recovery. 24/7 support available. The tide never stops."); Catalog.Add(B);
}

bool UBaseBuildingSubsystem::BuyBase(const FString& Id)
{
	for (const FMBaseDef& D : Catalog)
	{
		if (D.Id == Id)
		{
			for (const FMPlayerBase& B : Bases) if (B.Id == Id) return false; // already owned
			UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (!Sess || Sess->GetCurrency(ECurrency::Credits) < D.Cost) return false;
			Sess->AddCurrency(ECurrency::Credits, -D.Cost);
			FMPlayerBase PB; PB.Id = Id; PB.Ring = D.Ring; PB.Level = 1;
			Bases.Add(PB);
			Sess->ShowMessage(FString::Printf(TEXT("BASE PURCHASED: %s. %s Income: %d credits/hr."), *D.Name, *D.Bonus, D.Income));
			Sess->RecordEvent(FString::Printf(TEXT("Purchased base: %s"), *D.Name));
			return true;
		}
	}
	return false;
}

bool UBaseBuildingSubsystem::UpgradeBase(const FString& Id)
{
	for (FMPlayerBase& B : Bases)
	{
		if (B.Id == Id)
		{
			if (B.Level >= 5) return false;
			int32 Cost = B.Level * 50000;
			UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Cost) return false;
			Sess->AddCurrency(ECurrency::Credits, -Cost);
			B.Level++; B.Defense += 15;
			Sess->ShowMessage(FString::Printf(TEXT("Base upgraded to level %d. Defense now %d."), B.Level, B.Defense));
			return true;
		}
	}
	return false;
}

bool UBaseBuildingSubsystem::HireStaff(const FString& BaseId, const FString& Name, const FString& Role)
{
	for (FMPlayerBase& B : Bases)
	{
		if (B.Id == BaseId)
		{
			if (B.Staff.Num() >= B.Level * 2) return false;
			UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (!Sess || Sess->GetCurrency(ECurrency::Credits) < 500) return false;
			Sess->AddCurrency(ECurrency::Credits, -500);
			FMBaseStaff S; S.Name = Name; S.Role = Role; S.Level = 1; S.Salary = 100;
			B.Staff.Add(S);
			Sess->ShowMessage(FString::Printf(TEXT("Hired %s as %s. They report to your base."), *Name, *Role));
			return true;
		}
	}
	return false;
}

int32 UBaseBuildingSubsystem::GetTotalIncome() const
{
	int32 Total = 0;
	for (const FMPlayerBase& B : Bases)
	{
		for (const FMBaseDef& D : Catalog)
			if (D.Id == B.Id) { Total += D.Income * B.Level; break; }
		for (const FMBaseStaff& S : B.Staff) Total -= S.Salary;
	}
	return Total;
}
