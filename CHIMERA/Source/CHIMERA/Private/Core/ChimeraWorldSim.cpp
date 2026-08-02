#include "Core/ChimeraWorldSim.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraEconomy.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "CHIMERA.h"

// ---------------------------------------------------------------------------
// GDD 14.1 - Nemesis System.
// ---------------------------------------------------------------------------
void UNemesisSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UNemesisSubsystem::Tick), 5.0f);
}

void UNemesisSubsystem::Deinitialize()
{
	if (TickerHandle.IsValid()) FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	Super::Deinitialize();
}

bool UNemesisSubsystem::Tick(float DeltaTime)
{
	// Evolution: nemeses climb rank and grow more dangerous over time (GDD 14.1).
	for (FNemesisInfo& N : Nemeses)
	{
		if (FMath::RandRange(0, 100) < 8) N.Rank++;
	}
	AmbushTimer += DeltaTime;
	if (AmbushTimer > 60.f && Nemeses.Num() > 0)
	{
		AmbushTimer = 0.f;
	}
	return true;
}

void UNemesisSubsystem::OnPlayerWronged(const FString& NpcName, const FString& How)
{
	FNemesisInfo* Existing = FindNemesis(NpcName);
	if (!Existing)
	{
		static const TArray<FString> Types = { TEXT("The Braggart"), TEXT("The Silent"), TEXT("The Strategist"), TEXT("The Fanatic"), TEXT("The Tragic") };
		FNemesisInfo N;
		N.Name = NpcName;
		N.PersonalityType = Types[FMath::RandRange(0, Types.Num() - 1)];
		N.Scars = How;
		N.Rank = 1;
		N.Encounters = 1;
		Nemeses.Add(N);
		if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		{
			Sess->RecordEvent(FString::Printf(TEXT("A NEMESIS RISES: %s (%s) - %s"), *NpcName, *N.PersonalityType, *How));
		}
	}
	else
	{
		Existing->Encounters++;
		Existing->Rank++;
	}
}

FNemesisInfo* UNemesisSubsystem::FindNemesis(const FString& NpcName)
{
	for (FNemesisInfo& N : Nemeses)
	{
		if (N.Name == NpcName) return &N;
	}
	return nullptr;
}

bool UNemesisSubsystem::NemesisAct(FString& OutName, FString& OutAct)
{
	if (Nemeses.Num() == 0) return false;
	const FNemesisInfo& N = Nemeses[FMath::RandRange(0, Nemeses.Num() - 1)];
	OutName = N.Name;
	// GDD 14.1 interactions: ambushes, taunts, hostage situations.
	static const TArray<FString> Acts = {
		TEXT("ambushes you mid-shift, remembering what you did"),
		TEXT("taunts you on the in-game news"),
		TEXT("kidnaps someone you care about and leaves a message"),
		TEXT("has upgraded since you last fought - harder now")
	};
	OutAct = Acts[FMath::RandRange(0, Acts.Num() - 1)];
	return true;
}

// ---------------------------------------------------------------------------
// GDD 14.4 - Faction warfare.
// ---------------------------------------------------------------------------
void UFactionWarfareSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Territory.SetNum((int32)EFaction::COUNT);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Territory = Sess->FactionTerritory;
	}
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UFactionWarfareSubsystem::Tick), 30.0f);
}

void UFactionWarfareSubsystem::Deinitialize()
{
	if (TickerHandle.IsValid()) FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	Super::Deinitialize();
}

bool UFactionWarfareSubsystem::Tick(float DeltaTime)
{
	// Territory drifts between factions; player rep tilts the balance (GDD 14.4).
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		WarTimer += DeltaTime;
		for (int32 i = 0; i < (int32)EFaction::COUNT; ++i)
		{
			int32 Delta = FMath::RandRange(-1, 1) + Sess->GetRep((EFaction)i) / 100;
			Territory[i] = FMath::Clamp(Territory[i] + Delta, 0, 100);
		}
		// Large-scale battles at peak tensions (GDD 14.4).
		if (WarTimer > 180.f)
		{
			WarTimer = 0.f;
			if (FMath::RandRange(0, 100) < 30)
			{
				bBattleInProgress = true;
				EFaction A = GetLeadingFaction();
				EFaction B = (EFaction)FMath::RandRange(0, (int32)EFaction::COUNT - 1);
				if (A != B)
				{
					BattleReport = FString::Printf(TEXT("%s vs %s border clash at the Faction Line"),
						*FChimeraData::FactionNames()[(int32)A], *FChimeraData::FactionNames()[(int32)B]);
					Sess->RecordEvent(BattleReport);
					if (auto* BQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>()) BQ->ApplyEvent(TEXT("crime_wave"));
				}
				bBattleInProgress = false;
			}
		}
		Sess->FactionTerritory = Territory;
	}
	return true;
}

EFaction UFactionWarfareSubsystem::GetLeadingFaction() const
{
	int32 Best = 0;
	for (int32 i = 1; i < Territory.Num(); ++i)
	{
		if (Territory[i] > Territory[Best]) Best = i;
	}
	return (EFaction)Best;
}

// ---------------------------------------------------------------------------
// GDD 14.5 - Disasters.
// ---------------------------------------------------------------------------
void UDisasterSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	InWorld.GetTimerManager().SetTimer(DisasterTimerHandle, this,
		&UDisasterSubsystem::TryTriggerDisaster, 90.0f, true);
}

void UDisasterSubsystem::ClockTick()
{
	if (!GetWorld()) return;
	if (bDisasterActive)
	{
		RemainingSeconds = FMath::Max(0.f, RemainingSeconds - 1.f);
		if (RemainingSeconds <= 0.f) EndDisaster();
	}
}

void UDisasterSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDisasterSubsystem::TryTriggerDisaster()
{
	if (bDisasterActive || !GetWorld()) return;
	if (FMath::RandRange(0, 100) >= 40) return;
	static const TArray<FString> Disasters = {
		TEXT("Kaiju Rampage"), TEXT("Plague Outbreak"), TEXT("Reality Storm"),
		TEXT("Reactor Meltdown"), TEXT("Zombie Outbreak"), TEXT("Cyberpsycho Wave")
	};
	ActiveDisaster = Disasters[FMath::RandRange(0, Disasters.Num() - 1)];
	DisasterRing = (ERing)FMath::RandRange(0, (int32)ERing::COUNT - 1);
	bDisasterActive = true;
	RemainingSeconds = 60.f;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->RecordEvent(FString::Printf(TEXT("DISASTER: %s hits %s - respond as hero/doctor/cop/racer/builder"),
			*ActiveDisaster, *FChimeraData::Rings()[(int32)DisasterRing].Name));
		if (auto* BQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>()) BQ->ApplyEvent(TEXT("disaster"));
		if (auto* M = GetGameInstance()->GetSubsystem<UManagementSubsystem>()) M->OnCrimeSpike();
	}
	GetWorld()->GetTimerManager().SetTimer(ClockTimerHandle, this, &UDisasterSubsystem::ClockTick, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(ClearTimerHandle, this, &UDisasterSubsystem::EndDisaster, 60.0f, false);
}

void UDisasterSubsystem::EndDisaster()
{
	if (!bDisasterActive) return;
	bDisasterActive = false;
	ActiveDisaster.Empty();
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->RecordEvent(TEXT("The disaster passes. The city begins to rebuild."));
	}
}

// ---------------------------------------------------------------------------
// GDD 14.7 - Politics & elections.
// ---------------------------------------------------------------------------
void UPoliticsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	Rival = TEXT("Vice-Governor Kray");
	InWorld.GetTimerManager().SetTimer(ElectionTimerHandle, this,
		&UPoliticsSubsystem::RunElection, 240.0f, true);
}

void UPoliticsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UPoliticsSubsystem::RunElection()
{
	if (!GetWorld()) return;
	EFaction Winner = (EFaction)FMath::RandRange(0, (int32)EFaction::COUNT - 1);
	Governor = FString::Printf(TEXT("%s slate"), *FChimeraData::FactionNames()[(int32)Winner]);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->Governor = Governor;
		Sess->AddRep(Winner, 10);
		Sess->RecordEvent(FString::Printf(TEXT("ELECTION DAY: the %s slate wins the governorship. Policy changes incoming."), *Governor));
	}
}
