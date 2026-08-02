#include "Core/ChimeraArc.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UStoryArcSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetupStoryCharacters();
}

void UStoryArcSubsystem::SetupStoryCharacters()
{
	FMStoryCharacter C;

	C.Id = TEXT("katniss"); C.Name = TEXT("Katniss Everdeen");
	C.Title = TEXT("The Mockingjay — Victor of the 74th Hunger Games");
	C.Ring = TEXT("Panem"); C.UnlockChapter = (int32)EStoryChapter::Act2_Fractures;
	C.Story = TEXT("After the Crash, Katniss found herself in a world where the Capitol and Dauntless shared the same skyline. Her bow still sings. Her aim is still true. She fights for District 12 — and now, for every district across every ring.");
	StoryChars.Add(C);

	C.Id = TEXT("tris"); C.Name = TEXT("Tris Prior");
	C.Title = TEXT("The Divergent — Dauntless leader, system breaker");
	C.Ring = TEXT("Chicago"); C.UnlockChapter = (int32)EStoryChapter::Act3_Alliance;
	C.Story = TEXT("Tris survived the Erudite coup because the Crash happened mid-battle. She woke up in a merged Chicago where Gaffney Medical Center stands next to the Dauntless compound. Four has her back. Eric is still a problem. She's still Divergent. She's still fighting.");
	StoryChars.Add(C);

	C.Id = TEXT("meredith"); C.Name = TEXT("Meredith Grey");
	C.Title = TEXT("Chief of General Surgery — Grey-Sloan Memorial");
	C.Ring = TEXT("Seattle"); C.UnlockChapter = (int32)EStoryChapter::Act4_Endgame;
	C.Story = TEXT("When the Crash hit, Meredith was in OR 2. The patient on the table was from a reality she'd never seen — cybernetic implants, dragon-scale burns, and a collapsed lung from drowning in a voxel lake. She saved them. She's been saving impossible patients ever since. The sun is still her standard.");
	StoryChars.Add(C);

	C.Id = TEXT("nolan"); C.Name = TEXT("John Nolan");
	C.Title = TEXT("NPD Training Officer — the oldest rookie in Nexopolis");
	C.Ring = TEXT("Mid-Wilshire"); C.UnlockChapter = (int32)EStoryChapter::Act4_Endgame;
	C.Story = TEXT("Nolan was responding to a domestic disturbance when the sky cracked open. When he came to, his badge still on his chest, he found Mid-Wilshire merged with Panem's District 12. Criminals from eleven realities. One precinct. He's still learning every day. At 50, he's still the rookie.");
	StoryChars.Add(C);
}

void UStoryArcSubsystem::CheckStoryUnlocks()
{
	for (FMStoryCharacter& C : StoryChars)
	{
		if (!C.bUnlocked && (int32)Chapter >= C.UnlockChapter)
		{
			C.bUnlocked = true;
			// Add to roster as slot
			FRosterMember M;
			M.Name = C.Name; M.LifePath = C.Title; M.Level = FMath::RandRange(8, 15);
			M.Ring = C.Ring; M.bIsStoryChar = true;
			Roster.Add(M);
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (Sess)
			{
				Sess->ShowMessage(FString::Printf(TEXT("NEW CHARACTER UNLOCKED: %s — %s. Press L to switch. %s"), *C.Name, *C.Title, *C.Story));
				Sess->RecordEvent(FString::Printf(TEXT("Story character unlocked: %s"), *C.Name));
			}
		}
	}
}

// ======================================================================
// Chapter Progression
// ======================================================================
FString UStoryArcSubsystem::GetChapterName() const
{
	switch (Chapter)
	{
	case EStoryChapter::Prologue: return TEXT("Prologue — The Crash");
	case EStoryChapter::Act1_Convergence: return TEXT("Act I — Convergence");
	case EStoryChapter::Act2_Fractures: return TEXT("Act II — Fractures");
	case EStoryChapter::Act3_Alliance: return TEXT("Act III — Alliance");
	case EStoryChapter::Act4_Endgame: return TEXT("Act IV — Endgame");
	case EStoryChapter::Finale_Choice: return TEXT("Finale — The Choice");
	case EStoryChapter::Complete: return TEXT("Epilogue — Free Play");
	default: return TEXT("Unknown");
	}
}

FString UStoryArcSubsystem::GetChapterObjective() const
{
	switch (Chapter)
	{
	case EStoryChapter::Prologue: return TEXT("Create your character. Explore the Nexus. Talk to the Keeper.");
	case EStoryChapter::Act1_Convergence: return TEXT("Reach skill level 5. Visit 3 rings. Complete your first mission.");
	case EStoryChapter::Act2_Fractures: return TEXT("Complete fracture missions in Aether City, Mercy Heights, Mid-Wilshire, Neo-Kingdom, and Alagaesia. Each ring's reality is breaking.");
	case EStoryChapter::Act3_Alliance: return TEXT("Win a Hunger Games. Ignite the grand rebellion. Complete a heist. Build your coalition across all rings.");
	case EStoryChapter::Act4_Endgame: return TEXT("Reach floor 100 in any dungeon. Visit all 15 rings. The Convergence is approaching. Prepare for the final choice.");
	case EStoryChapter::Finale_Choice: return TEXT("Return to the Nexus. The Keepers await. Choose the fate of every reality.");
	case EStoryChapter::Complete: return TEXT("The world reflects your choice. All rings, all missions, all possibilities remain open. Free play forever.");
	default: return TEXT("Unknown objective.");
	}
}

void UStoryArcSubsystem::CheckChapterProgress()
{
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return;

	switch (Chapter)
	{
	case EStoryChapter::Prologue:
		if (Sess->bCharacterCreated) AdvanceToNextChapter();
		break;

	case EStoryChapter::Act1_Convergence:
		{
			int32 HighSkill = 0, RingsVisited = 0;
			for (const auto& S : Sess->Skills) HighSkill = FMath::Max(HighSkill, S.Level);
			if (HighSkill >= 5 && CompletedMissions.Num() >= 1) AdvanceToNextChapter();
			// Also advance if enough game progress
			if (Sess->GetGameDay() >= 5 && CompletedMissions.Num() >= 3) AdvanceToNextChapter();
		}
		break;

	case EStoryChapter::Act2_Fractures:
		if (CompletedMissions.Contains(TEXT("fracture_aether")) && CompletedMissions.Contains(TEXT("fracture_mercy")) &&
			CompletedMissions.Contains(TEXT("fracture_midwilshire")) && CompletedMissions.Contains(TEXT("fracture_neokingdom")) &&
			CompletedMissions.Contains(TEXT("fracture_alagaesia")))
			AdvanceToNextChapter();
		// Progress after enough general missions
		if (CompletedMissions.Num() >= 15) AdvanceToNextChapter();
		break;

	case EStoryChapter::Act3_Alliance:
		if (CompletedMissions.Contains(TEXT("hunger_games_won")) && CompletedMissions.Contains(TEXT("rebellion_won")) &&
			CompletedMissions.Contains(TEXT("heist_complete")))
			AdvanceToNextChapter();
		if (CompletedMissions.Num() >= 25) AdvanceToNextChapter();
		break;

	case EStoryChapter::Act4_Endgame:
		{
			auto* DS = GetGameInstance()->GetSubsystem<UDungeonSubsystem>();
			if (DS)
			{
				bool bFloor100 = false;
				for (int32 i = 0; i < (int32)EDungeon::COUNT; ++i)
					if (DS->GetDeepestFloor((EDungeon)i) >= 100) { bFloor100 = true; break; }
				if (bFloor100 || CompletedMissions.Num() >= 35) AdvanceToNextChapter();
			}
		}
		break;

	case EStoryChapter::Finale_Choice:
		// Player must explicitly choose at the Nexus to trigger the ending
		// This is set via SetEnding() called from an interactable
		break;

	default: break;
	}
}

void UStoryArcSubsystem::AdvanceToNextChapter()
{
	if (Chapter >= EStoryChapter::Finale_Choice) return;
	Chapter = (EStoryChapter)((int32)Chapter + 1);
	CheckStoryUnlocks(); // GTA V style — new character may unlock
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess)
	{
		Sess->ShowMessage(FString::Printf(TEXT("%s — %s"), *GetChapterName(), *GetChapterObjective()));
		Sess->RecordEvent(FString::Printf(TEXT("Chapter advanced: %s"), *GetChapterName()));
	}
}

bool UStoryArcSubsystem::CompleteMission(const FString& MissionId)
{
	if (CompletedMissions.Contains(MissionId)) return false;
	CompletedMissions.Add(MissionId);
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->RecordEvent(FString::Printf(TEXT("Mission completed: %s"), *MissionId));
	CheckChapterProgress();
	return true;
}

// ======================================================================
// Character Roster (GTA V-style swapping)
// ======================================================================
bool UStoryArcSubsystem::CreateMainCharacter(const FString& Name, const FString& LifePath)
{
	if (Roster.Num() >= 5) return false;
	FRosterMember M;
	M.Name = Name; M.LifePath = LifePath; M.Level = 1;
	M.Ring = TEXT("Nexus"); M.bActive = true; M.bIsStoryChar = false;
	ActiveSlot = 0;
	Roster.Insert(M, 0); // always slot 0
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(FString::Printf(TEXT("%s the %s — your story begins. L to swap characters when others join the roster."), *Name, *LifePath));
	return true;
}

bool UStoryArcSubsystem::SwitchCharacter(int32 Slot)
{
	if (Slot < 0 || Slot >= Roster.Num() || Slot == ActiveSlot) return false;
	if (SwapCooldown > 0.f) return false;

	// Save current character state
	if (ActiveSlot >= 0)
	{
		Roster[ActiveSlot].bActive = false;
		auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (Sess) { Roster[ActiveSlot].Ring = StaticEnum<ERing>()->GetNameStringByValue((int64)Sess->CurrentRing); }
	}

	// Activate new character
	Roster[Slot].bActive = true;
	ActiveSlot = Slot;
	SwapCooldown = 5.f;

	auto* Sess2 = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess2)
	{
		Sess2->ShowMessage(FString::Printf(TEXT("Switched to %s the %s. %d hours played."), *Roster[Slot].Name, *Roster[Slot].LifePath, FMath::RoundToInt(Roster[Slot].SecondsPlayed / 3600.f)));
		// In production: trigger GTA V-style camera zoom-out, location change, zoom-in
	}
	return true;
}

FRosterMember* UStoryArcSubsystem::GetActiveCharacter()
{
	return (ActiveSlot >= 0 && ActiveSlot < Roster.Num()) ? &Roster[ActiveSlot] : nullptr;
}

const FRosterMember* UStoryArcSubsystem::GetActiveCharacter() const
{
	return (ActiveSlot >= 0 && ActiveSlot < Roster.Num()) ? &Roster[ActiveSlot] : nullptr;
}

void UStoryArcSubsystem::TickInactiveCharacters(float DeltaTime)
{
	for (int32 i = 0; i < Roster.Num(); ++i)
	{
		if (i == ActiveSlot) continue;
		Roster[i].SecondsPlayed += DeltaTime * 0.1f; // slower time for inactive chars
	}
	SwapCooldown = FMath::Max(0.f, SwapCooldown - DeltaTime);
}
