#include "Core/ChimeraPolice.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UPoliceInvestigation::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedCasePool();
}

void UPoliceInvestigation::SeedCasePool()
{
	FPoliceCase C;
	C.CaseID = TEXT("MW-001"); C.Title = TEXT("The Hyperloop Heist"); C.Victim = TEXT("Nexopolis Transit Authority");
	C.Leads = { TEXT("disgruntled engineer"), TEXT("rival transport corp"), TEXT("fixer with a grudge") }; C.RequiredEvidence = 4; C.Difficulty = 2; CasePool.Add(C);
	C.CaseID = TEXT("MW-002"); C.Title = TEXT("Missing Dragon Egg"); C.Victim = TEXT("Eragon Shadeslayer");
	C.Leads = { TEXT("Empire remnants"), TEXT("rogue Rider"), TEXT("black market collector") }; C.RequiredEvidence = 5; C.Difficulty = 4; CasePool.Add(C);
	C.CaseID = TEXT("MW-003"); C.Title = TEXT("Surgery Sabotage"); C.Victim = TEXT("Grey-Sloan Memorial");
	C.Leads = { TEXT("rival hospital admin"), TEXT("disgraced surgeon"), TEXT("insurance fraud ring") }; C.RequiredEvidence = 3; C.Difficulty = 2; CasePool.Add(C);
	C.CaseID = TEXT("MW-004"); C.Title = TEXT("Cyberblackout at Aether City"); C.Victim = TEXT("Aether City Power Grid");
	C.Leads = { TEXT("gang of netrunners"), TEXT("corporate espionage"), TEXT("kaiju sympathiser") }; C.RequiredEvidence = 4; C.Difficulty = 3; CasePool.Add(C);
	C.CaseID = TEXT("MW-005"); C.Title = TEXT("The Reaping Conspiracy"); C.Victim = TEXT("District 12");
	C.Leads = { TEXT("Capitol loyalist"), TEXT("rigged reaping ball"), TEXT("Peacekeeper collusion") }; C.RequiredEvidence = 5; C.Difficulty = 4; CasePool.Add(C);
	C.CaseID = TEXT("MW-006"); C.Title = TEXT("Dauntless Initiation Death"); C.Victim = TEXT("Edward (Dauntless initiate)");
	C.Leads = { TEXT("Peter"), TEXT("Eric — staged"), TEXT("Erudite serum overdose") }; C.RequiredEvidence = 4; C.Difficulty = 3; CasePool.Add(C);
	C.CaseID = TEXT("MW-007"); C.Title = TEXT("Voxel Counterfeiting Ring"); C.Victim = TEXT("Minecraft Frontier Bank");
	C.Leads = { TEXT("redstone hacker"), TEXT("disgruntled builder"), TEXT("interdimensional smuggler") }; C.RequiredEvidence = 3; C.Difficulty = 1; CasePool.Add(C);
	C.CaseID = TEXT("MW-008"); C.Title = TEXT("The Mockingjay Assassination Plot"); C.Victim = TEXT("Katniss Everdeen");
	C.Leads = { TEXT("Snow loyalist"), TEXT("President Coin"), TEXT("Career tribute vendetta") }; C.RequiredEvidence = 5; C.Difficulty = 5; CasePool.Add(C);
	C.CaseID = TEXT("MW-009"); C.Title = TEXT("F1 Race Fixing Scandal"); C.Victim = TEXT("Grand Coliseum Racing Federation");
	C.Leads = { TEXT("betting syndicate"), TEXT("bribed official"), TEXT("sabotaged car") }; C.RequiredEvidence = 4; C.Difficulty = 3; CasePool.Add(C);
	C.CaseID = TEXT("MW-010"); C.Title = TEXT("The Keeper's Betrayal"); C.Victim = TEXT("The Nexus");
	C.Leads = { TEXT("corrupted Keeper"), TEXT("reality fragment"), TEXT("Council conspiracy") }; C.RequiredEvidence = 5; C.Difficulty = 5; CasePool.Add(C);
}

void UPoliceInvestigation::OpenNewCase()
{
	if (Cases.Num() > 0 && ActiveIdx >= 0 && Cases[ActiveIdx].Status != ECaseStatus::Solved) return;
	int32 Pick = FMath::RandRange(0, CasePool.Num() - 1);
	FPoliceCase C = CasePool[Pick];
	C.Status = ECaseStatus::Active;
	C.EvidenceCollected.Reset();
	ActiveIdx = Cases.Add(C);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->ShowMessage(FString::Printf(TEXT("NEW CASE: %s — %s. Collect evidence at the case board."), *C.CaseID, *C.Title));
		Sess->RecordEvent(FString::Printf(TEXT("Case opened: %s"), *C.Title));
	}
}

bool UPoliceInvestigation::CollectEvidence(const FString& Name, EEvidenceType Type, int32 Weight)
{
	if (ActiveIdx < 0 || ActiveIdx >= Cases.Num()) return false;
	FEvidence E; E.Name = Name; E.Type = Type; E.Weight = Weight; E.Description = TEXT("Collected by investigating officer.");
	Cases[ActiveIdx].EvidenceCollected.Add(E);
	if (Cases[ActiveIdx].EvidenceCollected.Num() >= Cases[ActiveIdx].RequiredEvidence)
		Cases[ActiveIdx].Status = ECaseStatus::Stalled; // ready for arrest
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(FString::Printf(TEXT("Evidence collected: %s (%d/%d)"), *Name,
			Cases[ActiveIdx].EvidenceCollected.Num(), Cases[ActiveIdx].RequiredEvidence));
	return true;
}

FString UPoliceInvestigation::InterviewSuspect(const FString& SuspectName)
{
	if (ActiveIdx < 0) return TEXT("No active case.");
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return TEXT("Err");
	FPoliceCase& C = Cases[ActiveIdx];
	int32 Skill = Sess->GetSkillLevel(FName("Pol_Interrogation"));
	bool bGotLead = FMath::RandRange(0, Skill + 10) >= 10;
	if (bGotLead)
	{
		FString Lead = C.Leads[FMath::RandRange(0, C.Leads.Num() - 1)];
		C.EvidenceCollected.Add({ FString::Printf(TEXT("Statement: %s"), *SuspectName), EEvidenceType::Witness, 1, Lead });
		Sess->AddSkillXP(FName("Pol_Interrogation"), 20);
		return FString::Printf(TEXT("%s cracks: '%s is involved. Follow the money.'"), *SuspectName, *Lead);
	}
	return FString::Printf(TEXT("%s: 'I don't know anything. I want my lawyer.'"), *SuspectName);
}

bool UPoliceInvestigation::MakeArrest()
{
	if (ActiveIdx < 0) return false;
	FPoliceCase& C = Cases[ActiveIdx];
	if (C.EvidenceCollected.Num() < C.RequiredEvidence) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	int32 Skill = Sess->GetSkillLevel(FName("Pol_Investigation")) + C.EvidenceCollected.Num() * 2;
	bool bSuccess = FMath::RandRange(0, Skill + C.Difficulty * 2) >= 10;
	if (bSuccess) { CloseCase(true); return true; }
	C.Status = ECaseStatus::Stalled;
	return false;
}

void UPoliceInvestigation::CloseCase(bool bArrested)
{
	if (ActiveIdx < 0) return;
	Cases[ActiveIdx].Status = bArrested ? ECaseStatus::Solved : ECaseStatus::Cold;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		Sess->ShowMessage(FString::Printf(TEXT("CASE %s: %s!"), bArrested ? TEXT("SOLVED") : TEXT("COLD"), *Cases[ActiveIdx].Title));
		Sess->AddStat(TEXT("arrest"), bArrested ? 1 : 0);
		Sess->AddFame(bArrested ? 5 : -2);
	}
}
