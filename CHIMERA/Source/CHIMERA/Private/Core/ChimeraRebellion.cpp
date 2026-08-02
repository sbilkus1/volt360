#include "Core/ChimeraRebellion.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraHungerGames.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void URebellionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedFactions();
}

void URebellionSubsystem::Deinitialize()
{
	if (Ticker.IsValid()) FTSTicker::GetCoreTicker().RemoveTicker(Ticker);
	Super::Deinitialize();
}

void URebellionSubsystem::SeedFactions()
{
	// Every CHIMERA universe contributes factions to the grand rebellion.
	// MOCKINGJAY SIDE: Katniss, Tris, Eragon, Meredith Grey, John Nolan, Finnick, Rue's spirit
	FRebelFaction F;
	F.Side = TEXT("Mockingjay");
	F.Strength = 9; F.Name = TEXT("District 12 Rebellion"); F.Leader = TEXT("Katniss Everdeen"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 7; F.Name = TEXT("District 13 Army"); F.Leader = TEXT("President Coin"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 7; F.Name = TEXT("District 4 Navy"); F.Leader = TEXT("Finnick Odair"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("District 7 Axemen"); F.Leader = TEXT("Johanna Mason"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("District 3 Engineers"); F.Leader = TEXT("Beetee Latier"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 5; F.Name = TEXT("District 8 Textile Workers"); F.Leader = TEXT("Commander Paylor"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 5; F.Name = TEXT("District 11 Harvesters"); F.Leader = TEXT("Thresh"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 9; F.Name = TEXT("The Divergent"); F.Leader = TEXT("Tris Prior"); F.Ring = TEXT("Chicago"); Factions.Add(F);
	F.Strength = 8; F.Name = TEXT("Dauntless Rebellion"); F.Leader = TEXT("Four (Tobias Eaton)"); F.Ring = TEXT("Chicago"); Factions.Add(F);
	F.Strength = 5; F.Name = TEXT("Amity Peacekeepers"); F.Leader = TEXT("Johanna Reyes"); F.Ring = TEXT("Chicago"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("Candor Truth-Speakers"); F.Leader = TEXT("Jack Kang"); F.Ring = TEXT("Chicago"); Factions.Add(F);
	F.Strength = 9; F.Name = TEXT("Dragon Riders"); F.Leader = TEXT("Eragon Shadeslayer"); F.Ring = TEXT("Alagaesia"); Factions.Add(F);
	F.Strength = 8; F.Name = TEXT("The Varden"); F.Leader = TEXT("Nasuada"); F.Ring = TEXT("Alagaesia"); Factions.Add(F);
	F.Strength = 7; F.Name = TEXT("Elves of Ellesmera"); F.Leader = TEXT("Queen Islanzadi"); F.Ring = TEXT("Alagaesia"); Factions.Add(F);
	F.Strength = 7; F.Name = TEXT("Dwarves of Farthen Dur"); F.Leader = TEXT("King Orik"); F.Ring = TEXT("Alagaesia"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("NPD Mid-Wilshire"); F.Leader = TEXT("John Nolan"); F.Ring = TEXT("Mid-Wilshire"); Factions.Add(F);
	F.Strength = 7; F.Name = TEXT("Grey-Sloan Memorial"); F.Leader = TEXT("Meredith Grey"); F.Ring = TEXT("Seattle"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("Gaffney Medical Center"); F.Leader = TEXT("Dr. Charles"); F.Ring = TEXT("Chicago"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("Chastain Park Memorial"); F.Leader = TEXT("Conrad Hawkins"); F.Ring = TEXT("ChastainPark"); Factions.Add(F);
	F.Strength = 5; F.Name = TEXT("The Keepers of the Nexus"); F.Leader = TEXT("Keeper Elyon"); F.Ring = TEXT("Nexus"); Factions.Add(F);
	// CAPITOL SIDE: Snow, Jeanine Matthews, Mutts, Career tributes, corrupt factions
	F.Side = TEXT("Capitol");
	F.Strength = 10; F.Name = TEXT("The Capitol"); F.Leader = TEXT("President Snow"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 8; F.Name = TEXT("Career Tributes"); F.Leader = TEXT("Cato"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 9; F.Name = TEXT("Erudite Regime"); F.Leader = TEXT("Jeanine Matthews"); F.Ring = TEXT("Chicago"); Factions.Add(F);
	F.Strength = 7; F.Name = TEXT("Dauntless Traitors"); F.Leader = TEXT("Eric"); F.Ring = TEXT("Chicago"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("Peacekeeper Forces"); F.Leader = TEXT("Seneca Crane"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 5; F.Name = TEXT("The Gamemakers"); F.Leader = TEXT("Plutarch Heavensbee (covert)"); F.Ring = TEXT("Panem"); Factions.Add(F);
	F.Strength = 6; F.Name = TEXT("The Empire"); F.Leader = TEXT("Galbatorix (invoked in spirit)"); F.Ring = TEXT("Alagaesia"); Factions.Add(F);
	F.Strength = 5; F.Name = TEXT("Dark Keepers"); F.Leader = TEXT("A corrupted Council member"); F.Ring = TEXT("Nexus"); Factions.Add(F);
	// Populate ally lists.
	for (const FRebelFaction& RF : Factions)
	{
		if (RF.Side == TEXT("Mockingjay")) MockingjayAllies.Add(RF.Name);
		else CapitolAllies.Add(RF.Name);
	}
}

void URebellionSubsystem::IgniteRebellion()
{
	if (Phase != ERebelPhase::Peace) return;
	Phase = ERebelPhase::Sparks;
	Progress = 0;
	RebelLog.Reset();
	RebelLog.Add(TEXT("THE GRAND REBELLION IGNITES. Every ring, every universe, every character chooses a side."));
	RebelLog.Add(TEXT("MOCKINGJAY ALLIANCE: Katniss & Tris & Eragon unite. The Mockingjay calls. The Divergent rises. The Dragon roars."));
	RebelLog.Add(TEXT("CAPITOL-ERUDITE AXIS: Snow & Jeanine & the Empire's shadow form an unholy pact."));
	Ticker = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &URebellionSubsystem::Tick), 10.f);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->ShowMessage(TEXT("THE REBELLION BEGINS. Choose your side. The whole CHIMERA world divides."));
		Sess->RecordEvent(TEXT("Grand rebellion ignited across all rings."));
	}
}

bool URebellionSubsystem::Tick(float DeltaTime)
{
	if (Phase >= ERebelPhase::Victory) return false;
	TickTimer += DeltaTime;
	if (TickTimer >= 10.f)
	{
		TickTimer = 0.f;
		// Background: push events happen regardless of player action.
		if (Progress >= MaxProgress) { Phase = ERebelPhase::Victory; return false; }
		if (Progress >= 20) { Phase = ERebelPhase::CapitolSiege; if (!RebelLog.Contains(TEXT("The Capitol City Circle is breached."))) RebelLog.Add(TEXT("The Capitol City Circle is breached. The Star Squad enters. Snow's mansion is within reach.")); }
		if (Progress >= 10) { Phase = ERebelPhase::Coalition; if (!RebelLog.Contains(TEXT("Coalition formed. All 15 rings have committed forces."))) RebelLog.Add(TEXT("Coalition formed. All 15 rings have committed forces. The armies march on the Capitol.")); }
		if (Progress >= 5) { Phase = ERebelPhase::DistrictRising; if (!RebelLog.Contains(TEXT("Districts rise. Panem burns from within."))) RebelLog.Add(TEXT("Districts rise. Panem burns from within. The Seam, the Hob, the Nut — all fall to the Mockingjay.")); }
	}
	return true;
}

bool URebellionSubsystem::RebelAction(const FString& Skill, const FString& Description)
{
	if (Phase < ERebelPhase::Sparks || Phase >= ERebelPhase::Victory) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	int32 Lvl = Sess->GetSkillLevel(FName(*Skill));
	bool bSuccess = FMath::RandRange(0, Lvl + 10) >= 10;
	if (bSuccess)
	{
		Progress++;
		RebelLog.Add(Description);
		Sess->AddSkillXP(FName(*Skill), 20);
		Sess->AddFame(3);
		Sess->ShowMessage(FString::Printf(TEXT("REBELLION: %s — success. Progress %d/%d."), *Description, Progress, MaxProgress));
		if (Progress >= MaxProgress)
		{
			Phase = ERebelPhase::Victory;
			Sess->ShowMessage(TEXT("THE MOCKINGJAY WINS. Snow is executed. The Divergent live free. The dragons soar over a liberated Panem."));
			Sess->RecordEvent(TEXT("Grand rebellion victorious. The Mockingjay flies. CHIMERA is free."));
		}
		return true;
	}
	Sess->ShowMessage(FString::Printf(TEXT("REBELLION: %s — stalled. The Capitol fights back."), *Description));
	return false;
}
