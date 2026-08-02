#include "Core/ChimeraHungerGames.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

// Names for tributes per district.
static const TCHAR* MaleNames[] = { TEXT("Marvel"), TEXT("Cato"), TEXT("Beetee"), TEXT("Finnick"), TEXT("Porter"), TEXT("Dalton"),
	TEXT("Blight"), TEXT("Woovey"), TEXT("Than"), TEXT("Thresh"), TEXT("Rye"), TEXT("Peeta") };
static const TCHAR* FemaleNames[] = { TEXT("Glimmer"), TEXT("Clove"), TEXT("Wiress"), TEXT("Annie"), TEXT("Foxface"),
	TEXT("Morphling"), TEXT("Johanna"), TEXT("Cecelia"), TEXT("Seeder"), TEXT("Rue"), TEXT("Katniss"), TEXT("Sloan") };
static const TCHAR* InitialWeapons[] = { TEXT("sword"), TEXT("knife"), TEXT("spear"), TEXT("trident"), TEXT("bow"),
	TEXT("axe"), TEXT("wire trap"), TEXT("club"), TEXT("sickle"), TEXT("slingshot"), TEXT("bare hands"), TEXT("throwing stars") };

void UHungerGamesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UHungerGamesSubsystem::Deinitialize()
{
	if (ArenaTicker.IsValid()) FTSTicker::GetCoreTicker().RemoveTicker(ArenaTicker);
	Super::Deinitialize();
}

void UHungerGamesSubsystem::GenerateTributes()
{
	Tributes.Reset();
	FallenTributes.Reset();
	EventLog.Reset();
	VictorName.Empty();
	for (int32 d = 1; d <= 12; ++d)
	{
		FTribute M, F;
		M.Name = FString::Printf(TEXT("%s [D%dM]"), MaleNames[d - 1], d);
		M.District = d; M.bFemale = false; M.Combat = FMath::RandRange(2, 8);
		M.stealth = FMath::RandRange(2, 8); M.survival = FMath::RandRange(2, 8);
		M.bCareer = (d == 1 || d == 2 || d == 4);
		M.Weapon = InitialWeapons[FMath::RandRange(0, 11)];
		F.Name = FString::Printf(TEXT("%s [D%dF]"), FemaleNames[d - 1], d);
		F.District = d; F.bFemale = true; F.Combat = FMath::RandRange(2, 8);
		F.stealth = FMath::RandRange(2, 8); F.survival = FMath::RandRange(2, 8);
		F.bCareer = (d == 1 || d == 2 || d == 4);
		F.Weapon = InitialWeapons[FMath::RandRange(0, 11)];
		Tributes.Add(M); Tributes.Add(F);
	}
}

void UHungerGamesSubsystem::StartReaping()
{
	if (Phase != EArenaPhase::Idle) return;
	GenerateTributes();
	Phase = EArenaPhase::Reaping;
	ArenaDay = 0;
	EventLog.Add(TEXT("REAPING DAY: 24 tributes chosen. May the odds be ever in their favour."));
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(TEXT("THE REAPING: 24 tributes. 12 districts. One victor."));
}

void UHungerGamesSubsystem::AdvanceToTraining()
{
	if (Phase != EArenaPhase::Reaping && Phase != EArenaPhase::Idle) return;
	Phase = EArenaPhase::Training;
	EventLog.Add(TEXT("TRAINING: 3 days to impress sponsors and sharpen skills."));
	for (FTribute& T : Tributes) { T.Combat = FMath::Min(10, T.Combat + FMath::RandRange(0, 2)); }
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(TEXT("Training begins. Visit the Training Centre to sponsor a tribute or train."));
}

void UHungerGamesSubsystem::StartArena()
{
	if (Phase != EArenaPhase::Training) return;
	Phase = EArenaPhase::Arena;
	ArenaDay = 1;
	// Bloodbath at the Cornucopia — 4-8 die immediately.
	int32 Bloodbath = FMath::RandRange(4, 8);
	for (int32 i = 0; i < Bloodbath && GetAliveCount() > 1; ++i)
	{
		TArray<int32> Alive;
		for (int32 j = 0; j < Tributes.Num(); ++j) if (Tributes[j].bAlive) Alive.Add(j);
		if (Alive.Num() == 0) break;
		FTribute& T = Tributes[Alive[FMath::RandRange(0, Alive.Num() - 1)]];
		if (T.bCareer) continue; // Careers survive the bloodbath
		T.bAlive = false; T.Hunger = 100;
		FallenTributes.Add(T.Name);
		EventLog.Add(FString::Printf(TEXT("BLOODBATH: %s fell at the Cornucopia."), *T.Name));
	}
	for (FTribute& T : Tributes) { if (T.bAlive) T.Hunger = FMath::RandRange(10, 30); }
	EventLog.Add(FString::Printf(TEXT("%d tributes remain. The Games begin."), GetAliveCount()));
	ArenaTimer = 0.f;
	ArenaTicker = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UHungerGamesSubsystem::ArenaTick), 1.5f);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(FString::Printf(TEXT("THE GAMES BEGIN. %d tributes alive after the bloodbath."), GetAliveCount()));
}

bool UHungerGamesSubsystem::ArenaTick(float DeltaTime)
{
	if (Phase != EArenaPhase::Arena) return false;
	ArenaTimer += DeltaTime;
	if (ArenaTimer >= 3.f) { ArenaTimer = 0.f; DoArenaEvent(); }
	return true;
}

void UHungerGamesSubsystem::DoArenaEvent()
{
	if (GetAliveCount() <= 1) { CrownVictor(); return; }
	// Every few events is a day-end cannon.
	int32 Evt = FMath::RandRange(0, 12);
	if (Evt <= 5) // Combat event
	{
		TArray<FTribute*> Alive;
		for (FTribute& T : Tributes) if (T.bAlive) Alive.Add(&T);
		if (Alive.Num() < 2) return;
		FTribute* A = Alive[FMath::RandRange(0, Alive.Num() - 1)];
		FTribute* B = Alive[FMath::RandRange(0, Alive.Num() - 1)];
		while (B == A && Alive.Num() > 1) B = Alive[FMath::RandRange(0, Alive.Num() - 1)];
		int32 ARoll = A->Combat * 10 + FMath::RandRange(0, 50);
		int32 BRoll = B->Combat * 10 + FMath::RandRange(0, 50);
		if (ARoll > BRoll) // A wins
		{
			if (FMath::RandRange(0, 3) == 0) { B->bAlive = false; A->Kills++; FallenTributes.Add(B->Name);
				EventLog.Add(FString::Printf(TEXT("%s killed %s with a %s. Cannon fires. %d remain."),
					*A->Name, *B->Name, *A->Weapon, GetAliveCount() - 1)); }
			else EventLog.Add(FString::Printf(TEXT("%s escaped from %s in the darkness."), *B->Name, *A->Name));
		}
		else { A->Health -= FMath::RandRange(10, 40); EventLog.Add(FString::Printf(TEXT("%s wounded %s."), *B->Name, *A->Name)); }
	}
	else if (Evt == 6) // Sponsor gift
	{
		TArray<FTribute*> Alive;
		for (FTribute& T : Tributes) if (T.bAlive && T.Hunger < 60) Alive.Add(&T);
		if (Alive.Num() > 0)
		{
			FTribute* T = Alive[FMath::RandRange(0, Alive.Num() - 1)];
			T->SponsorGifts++; T->Health = FMath::Min(100, T->Health + 20);
			EventLog.Add(FString::Printf(TEXT("SPONSOR GIFT: %s received medicine from an unknown sponsor."), *T->Name));
		}
	}
	else if (Evt == 7 || Evt == 8) // Hunger / thirst
	{
		for (FTribute& T : Tributes) { if (T.bAlive) T.Hunger += 8; }
		for (FTribute& T : Tributes)
		{
			if (T.bAlive && T.Hunger >= 100) { T.bAlive = false; FallenTributes.Add(T.Name);
				EventLog.Add(FString::Printf(TEXT("%s died of starvation. No cannon for the forgotten."), *T.Name)); }
			if (T.bAlive && T.Hunger > 50 && T.survival > 5) T.Hunger -= 10;
		}
	}
	else if (Evt == 9) // Arena hazard (fire, flood, muttation)
	{
		for (FTribute& T : Tributes) { if (T.bAlive && FMath::RandRange(0, 8 - T.survival) == 0) T.Health -= 30; }
		EventLog.Add(TEXT("ARENA HAZARD: A fire sweeps the arena. Tributes scramble for safety."));
	}
	else // Day-end summary
	{
		ArenaDay++;
		for (FTribute& T : Tributes) { if (T.bAlive) T.Hunger = FMath::Min(100, T.Hunger + 15); }
		EventLog.Add(FString::Printf(TEXT("== NIGHT %d: %d tributes alive. The anthem plays. The faces of the dead appear in the sky. =="),
			ArenaDay, GetAliveCount()));
	}
	// Check for death via health loss.
	for (FTribute& T : Tributes)
	{
		if (T.bAlive && T.Health <= 0) { T.bAlive = false; FallenTributes.Add(T.Name);
			EventLog.Add(FString::Printf(TEXT("CANNON: %s succumbed to their wounds."), *T.Name)); }
	}
	if (GetAliveCount() <= 1) CrownVictor();
}

void UHungerGamesSubsystem::CrownVictor()
{
	if (ArenaTicker.IsValid()) { FTSTicker::GetCoreTicker().RemoveTicker(ArenaTicker); ArenaTicker.Reset(); }
	for (const FTribute& T : Tributes)
	{
		if (T.bAlive) { VictorName = T.Name; Phase = EArenaPhase::Victor; break; }
	}
	TotalGamesPlayed++;
	EventLog.Add(FString::Printf(TEXT("VICTOR: %s! The 74th Hunger Games have ended. May the odds be ever in your favour."),
		*VictorName));
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->ShowMessage(FString::Printf(TEXT("VICTOR CROWNED: %s wins the Hunger Games!"), *VictorName));
		Sess->RecordEvent(FString::Printf(TEXT("Hunger Games victor: %s (Games #%d)"), *VictorName, TotalGamesPlayed));
	}
}

bool UHungerGamesSubsystem::SponsorTribute(int32 Index)
{
	if (Phase != EArenaPhase::Arena || Index < 0 || Index >= Tributes.Num()) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < 500) return false;
	Sess->AddCurrency(ECurrency::Credits, -500);
	Tributes[Index].SponsorGifts++;
	Tributes[Index].Health = FMath::Min(100, Tributes[Index].Health + 25);
	Sess->ShowMessage(FString::Printf(TEXT("Sponsor gift sent to %s. +Health. The crowd applauds."), *Tributes[Index].Name));
	return true;
}

int32 UHungerGamesSubsystem::GetAliveCount() const
{
	int32 C = 0; for (const FTribute& T : Tributes) if (T.bAlive) C++; return C;
}

FTribute* UHungerGamesSubsystem::FindTribute(const FString& Name)
{
	for (FTribute& T : Tributes) if (T.Name == Name) return &T; return nullptr;
}

// === Rebellion War (Mockingjay) ===
bool UHungerGamesSubsystem::AdvanceWar(const FString& Action)
{
	if (!bWarActive) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	WarProgress++;
	WarLog.Add(Action);
	int32 Check = Sess->GetSkillLevel(FName("Combat_Brawling")) + Sess->GetSkillLevel(FName("Combat_Stealth"));
	if (FMath::RandRange(0, Check + 10) >= 10)
	{
		if (WarProgress >= WarMaxProgress)
		{
			WarPhase = EWarPhase::FreePanem;
			WarLog.Add(TEXT("SNOW IS DEAD. Panem is free. The Mockingjay's wings spread across the nation."));
			Sess->ShowMessage(TEXT("REBELLION VICTORY: Snow has fallen. Panem rises free."));
			Sess->RecordEvent(TEXT("The rebellion succeeded. President Snow executed. The Hunger Games are abolished."));
			return true;
		}
		if (WarProgress == 5) { WarPhase = EWarPhase::District2; Sess->ShowMessage(TEXT("District 2 falls. The Nut is cracked.")); }
		if (WarProgress == 10) { WarPhase = EWarPhase::CapitolAssault; Sess->ShowMessage(TEXT("The Capitol is breached. The Star Squad enters the City Circle.")); }
	}
	return true;
}
