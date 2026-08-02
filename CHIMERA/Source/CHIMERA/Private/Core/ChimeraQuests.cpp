#include "Core/ChimeraQuests.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UQuestSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// GDD 12.2 - Act 1 arc is available from the start; fractures unlock via story later.
	AvailableQuests = FChimeraData::Act1Quests();
}

void UQuestSystem::StartQuest(const FName& Id)
{
	if (bActive) return;
	for (const FQuestDef& Q : AvailableQuests)
	{
		if (Q.Id == Id)
		{
			Active = Q;
			ObjectiveIndex = 0;
			Mistakes = 0;
			bActive = true;
			if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
			{
				Sess->SetActiveQuest(Active);
			}
			return;
		}
	}
}

void UQuestSystem::GenerateRadiantQuest()
{
	// GDD 12.6 - procedural templates combined with world-state variables.
	static const TArray<FString> Templates = {
		TEXT("Eliminate %s at %s. Watch out for %s."),
		TEXT("Retrieve %s from %s. It is guarded by %s. Bonus if clean."),
		TEXT("Get %s from %s to %s. %s will try to stop you."),
		TEXT("%s happened at %s. Interview witnesses, find the truth.")
	};
	static const TArray<FString> Targets = { TEXT("the rogue drone"), TEXT("the cyber-creeper"), TEXT("the fixer"), TEXT("the corrupt officer") };
	static const TArray<FString> Items = { TEXT("the dragon scale"), TEXT("the hacked chip"), TEXT("the stolen vial"), TEXT("the brightsteel ingot") };
	static const TArray<FString> Complications = { TEXT("a reality fracture"), TEXT("a NEMESIS ambush"), TEXT("a kaiju in the area"), TEXT("a hostage") };

	const TArray<FRingDef>& Rings = FChimeraData::Rings();
	int32 RIdx = FMath::RandRange(1, Rings.Num() - 1); // skip Nexus
	int32 RIdx2 = FMath::RandRange(1, Rings.Num() - 1);
	int32 RIdx3 = FMath::RandRange(1, Rings.Num() - 1);

	FQuestDef Q;
	Q.bRadiant = true;
	Q.Id = FName(*FString::Printf(TEXT("Radiant_%d"), FMath::RandRange(1000, 999999)));
	Q.Ring = (ERing)RIdx;
	const FString A = Rings[RIdx].Name, B = Rings[RIdx2].Name, C = Rings[RIdx3].Name;

	int32 T = FMath::RandRange(0, Templates.Num() - 1);
	Q.Title = FString::Printf(TEXT("Radiant: %s"), *Targets[FMath::RandRange(0, Targets.Num() - 1)]);
	Q.Description = FString::Printf(*Templates[T],
		*Targets[FMath::RandRange(0, Targets.Num() - 1)], *A,
		*Complications[FMath::RandRange(0, Complications.Num() - 1)]);

	// Cross-ring objective chain (GDD 12.6 example: poisoned footballer).
	FQuestObjective O1; O1.Text = FString::Printf(TEXT("Investigate in %s"), *A); O1.StatKey = FName("visit_ring"); O1.Target = 1; Q.Objectives.Add(O1);
	FQuestObjective O2; O2.Text = FString::Printf(TEXT("Interrogate the suspect in %s"), *B); O2.StatKey = FName("interrogate"); O2.Target = 1; Q.Objectives.Add(O2);
	FQuestObjective O3; O3.Text = FString::Printf(TEXT("Test the evidence in %s lab"), *C); O3.StatKey = FName("craft"); O3.Target = 1; Q.Objectives.Add(O3);
	Q.RewardCredits = 100 + FMath::RandRange(0, 5) * 100;

	AvailableQuests.Add(Q);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->RecordEvent(FString::Printf(TEXT("New radiant quest: %s"), *Q.Title));
	}
	// Auto-start so the player always has a hook.
	StartQuest(Q.Id);
}

void UQuestSystem::ProgressObjective(const FName& StatKey, int32 Amt)
{
	if (!bActive) return;
	if (ObjectiveIndex >= Active.Objectives.Num()) return;
	FQuestObjective& Obj = Active.Objectives[ObjectiveIndex];
	if (Obj.StatKey == StatKey || Obj.StatKey == FName("ANY"))
	{
		Obj.Progress += Amt;
		if (Obj.Progress >= Obj.Target)
		{
			AdvanceObjective();
		}
	}
}

void UQuestSystem::AdvanceObjective()
{
	ObjectiveIndex++;
	if (ObjectiveIndex >= Active.Objectives.Num())
	{
		FinalizeQuest();
	}
	else
	{
		if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		{
			Sess->CurrentObjective = Active.Objectives[ObjectiveIndex].Text;
		}
	}
}

void UQuestSystem::FinalizeQuest()
{
	// GDD 12.9 - mission rating: S(6) down to F(0); mistakes + missed bonus cap it.
	Active.Rating = FMath::Clamp(6 - Mistakes, 1, 6);
	// GDD 12.9 - timer bonus: completing fast pushes rating up
	if (QuestTimer < 60.f) Active.Rating = FMath::Min(6, Active.Rating + 1);
	FName DoneId = Active.Id;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		// Restore the finalized quest so completion rewards use rating.
		Sess->ActiveQuest = Active;
		Sess->CompleteActiveQuest();
	}
	bActive = false;
	// GDD 12.2 - Act 1 chains: M1 -> M3 -> M4. The boss (M6) starts when you
	// investigate the fracture near the Aether gate.
	if (DoneId == FName("Q_A1_Contact")) StartQuest(FName("Q_A1_Keeper"));
	else if (DoneId == FName("Q_A1_Keeper")) StartQuest(FName("Q_A1_Fractures"));
}

FString UQuestSystem::GetRating() const
{
	switch (Active.Rating)
	{
	case 6: return TEXT("S-Rank — Masterpiece");
	case 5: return TEXT("A-Rank — Excellent");
	case 4: return TEXT("B-Rank — Good");
	case 3: return TEXT("C-Rank — Acceptable");
	case 2: return TEXT("D-Rank — Poor");
	default: return TEXT("F-Rank — Failure");
	}
}
