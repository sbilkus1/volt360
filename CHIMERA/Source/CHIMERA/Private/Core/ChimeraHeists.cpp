#include "Core/ChimeraHeists.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraArc.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UHeistSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedHeists();
	SeedCrew();
}

void UHeistSubsystem::SeedHeists()
{
	FHeistDef H[] = {
		{ TEXT("dragons_hoard"), TEXT("The Dragon's Hoard"), TEXT("Ancient dragon treasure vault"),
			TEXT("Alagaesia, Minecraft Frontier"),
			{ TEXT("Stealth"), TEXT("Aggressive"), TEXT("Technical") }, 15000,
			TEXT("Invisibility potion or tunnel into the lair. The dragon sleeps light.") },
		{ TEXT("black_site"), TEXT("The Black Site"), TEXT("Secret NPD evidence vault"),
			TEXT("Mid-Wilshire, Aether City"),
			{ TEXT("Stealth"), TEXT("Social"), TEXT("Technical") }, 8000,
			TEXT("Impersonate officers or blackmail the vault manager. City-wide corruption on the line.") },
		{ TEXT("golden_goal"), TEXT("Golden Goal"), TEXT("Rig the Nexopolis Cup final"),
			TEXT("Grand Coliseum, Mid-Wilshire"),
			{ TEXT("Stealth"), TEXT("Technical"), TEXT("Aggressive") }, 20000,
			TEXT("Fix a player, hack the VAR, or kidnap the referee. The whole city's watching.") },
		{ TEXT("cure_protocol"), TEXT("Cure Protocol"), TEXT("Steal an experimental cure"),
			TEXT("Mercy Heights, Night City Expanse"),
			{ TEXT("Stealth"), TEXT("Social"), TEXT("Technical") }, 10000,
			TEXT("A pandemic cure is locked away for shareholders only. Break in or convince a doctor to help.") },
		{ TEXT("nexus_job"), TEXT("The Nexus Job"), TEXT("Rob the Nexus vault itself"),
			TEXT("The Nexus + all rings"),
			{ TEXT("Stealth"), TEXT("Aggressive"), TEXT("Technical"), TEXT("Social") }, 50000,
			TEXT("The ultimate heist. Requires max-level crew from every ring. One billion credits split among crew.") },
	};
	for (const FHeistDef& h : H) Heists.Add(h);
}

void UHeistSubsystem::SeedCrew()
{
	FCrewMemberDef C[] = {
		{ TEXT("Sparrow"), TEXT("Driver"), TEXT("getaway driver extraordinaire"), 15, 7 },
		{ TEXT("ZeroByte"), TEXT("Hacker"), TEXT("netrunner, Blackwall certified"), 18, 8 },
		{ TEXT("Brick"), TEXT("Gunslinger"), TEXT("heavy artillery, ex-NPD SWAT"), 20, 6 },
		{ TEXT("Kestrel"), TEXT("Brawler"), TEXT("close-quarters specialist, lost a dragon and wants revenge"), 14, 7 },
		{ TEXT("Lady Dusk"), TEXT("Socialite"), TEXT("high-society infiltrator, speaks seven ring dialects"), 16, 9 },
		{ TEXT("Ticker"), TEXT("Safecracker"), TEXT("clockwork genius, twice defected from the Keepers"), 18, 8 },
		{ TEXT("Wraith"), TEXT("Brawler"), TEXT("silent takedown specialist"), 17, 9 },
		{ TEXT("Overclock"), TEXT("Hacker"), TEXT("rigs vehicles, drones, and VAR systems"), 15, 7 },
	};
	for (const FCrewMemberDef& c : C) Crew.Add(c);
}

bool UHeistSubsystem::StartHeist(int32 HeistIdx, const FString& Approach)
{
	if (HeistIdx < 0 || HeistIdx >= Heists.Num()) return false;
	ActiveHeist = Heists[HeistIdx];
	if (!ActiveHeist.Approaches.Contains(Approach)) return false;
	ActiveApproach = Approach;
	Phase = EHeistPhase::Planning;
	bActive = true;
	HeistProgress = 0;
	HeistMaxProgress = 4;
	HeistMistakes = 0;
	HiredIndices.Reset();
	if (UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->RecordEvent(FString::Printf(TEXT("Heist started: %s (%s approach)"), *ActiveHeist.Title, *ActiveApproach));
	}
	return true;
}

void UHeistSubsystem::AdvancePhase()
{
	if (!bActive || Phase >= EHeistPhase::Complete) return;
	Phase = (EHeistPhase)((int32)Phase + 1);
	if (Phase == EHeistPhase::Prep)
	{
		HeistMaxProgress = 3;
		HeistProgress = 0;
	}
	else if (Phase == EHeistPhase::Execution)
	{
		HeistMaxProgress = 4;
		HeistProgress = 0;
	}
	else if (Phase == EHeistPhase::Getaway)
	{
		HeistMaxProgress = 2;
		HeistProgress = 0;
	}
	else if (Phase == EHeistPhase::Complete)
	{
		int32 Payout = ResolvePayout();
		if (UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		{
			Sess->AddCurrency(ECurrency::Credits, Payout);
			Sess->RecordEvent(FString::Printf(TEXT("Heist complete: %s. Payout: %d credits after crew cuts."),
				*ActiveHeist.Title, Payout));
			Sess->ShowMessage(FString::Printf(TEXT("HEIST COMPLETE: %s! +%d credits (after crew cuts)"),
				*ActiveHeist.Title, Payout));
			if (auto* Arc = GetGameInstance()->GetSubsystem<UStoryArcSubsystem>()) Arc->CompleteMission(TEXT("heist_complete"));
		}
	}
}

bool UHeistSubsystem::HireCrew(int32 CrewIdx)
{
	if (CrewIdx < 0 || CrewIdx >= Crew.Num() || HiredIndices.Contains(CrewIdx)) return false;
	HiredIndices.Add(CrewIdx);
	if (UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->ShowMessage(FString::Printf(TEXT("Hired: %s (%s) for %d%% cut."),
			*Crew[CrewIdx].Name, *Crew[CrewIdx].Specialization, Crew[CrewIdx].CutPercent));
	}
	return true;
}

bool UHeistSubsystem::PerformHeistAction(const FString& SkillId, const FString& ActionDesc)
{
	if (!bActive || Phase == EHeistPhase::Planning || Phase >= EHeistPhase::Complete) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;

	int32 SkillLvl = Sess->GetSkillLevel(FName(*SkillId));
	int32 CrewBonus = 0;
	for (int32 Idx : HiredIndices)
	{
		// crew skill adds to the check
		CrewBonus += Crew[Idx].SkillLevel / 2;
	}
	int32 Total = SkillLvl + CrewBonus;
	bool bSuccess = FMath::RandRange(0, Total + 10) >= 10;

	if (bSuccess)
	{
		HeistProgress++;
		Sess->AddSkillXP(FName(*SkillId), 20);
		Sess->ShowMessage(FString::Printf(TEXT("Heist action: %s — SUCCESS."), *ActionDesc));
		if (HeistProgress >= HeistMaxProgress) AdvancePhase();
	}
	else
	{
		HeistMistakes++;
		Sess->ShowMessage(FString::Printf(TEXT("Heist action: %s — FAILED. The mark is spooked."), *ActionDesc));
		if (HeistMistakes >= 3)
		{
			Sess->ShowMessage(TEXT("HEIST FAILED. Too many mistakes. The operation is blown."));
			Sess->AddInfamy(10);
			Sess->WantedLevel = FMath::Min(5, Sess->WantedLevel + 1);
			bActive = false;
		}
	}
	return bSuccess;
}

int32 UHeistSubsystem::ResolvePayout()
{
	int32 Gross = ActiveHeist.BasePayout;
	if (HeistMistakes == 0) Gross = Gross * 3 / 2; // flawless bonus
	int32 CrewCut = 0;
	for (int32 Idx : HiredIndices) CrewCut += Crew[Idx].CutPercent;
	int32 CutAmt = Gross * CrewCut / 100;
	return Gross - CutAmt;
}
