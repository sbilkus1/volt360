#include "Core/ChimeraJustice.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraArc.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UJusticeSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	State.Phase = ECourtPhase::Free;
	State.WantedStars = 0;
}

void UJusticeSystem::Surrender()
{
	if (State.WantedStars == 0) return;
	State.bSurrendered = true;
	State.Phase = ECourtPhase::Arrested;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess)
	{
		Sess->WantedLevel = 0;
		Sess->ShowMessage(TEXT("You raise your hands. 'I surrender.' The officer cuffs you. This will look better in court."));
		Sess->RecordEvent(TEXT("Arrested: surrendered peacefully."));
	}
	BookInmate();
}

void UJusticeSystem::ResistArrest()
{
	State.bSurrendered = false;
	State.Phase = ECourtPhase::Arrested;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess)
	{
		Sess->WantedLevel = 0;
		Sess->AddInfamy(10);
		Sess->ShowMessage(TEXT("The police tackle you to the ground. 'Stop resisting!' This won't help your case."));
		Sess->RecordEvent(TEXT("Arrested: resisted. Additional charges filed."));
	}
	BookInmate();
}

void UJusticeSystem::BookInmate()
{
	State.Phase = ECourtPhase::Booked;
	State.DaysUntilTrial = FMath::RandRange(1, 5);  // 1-5 days until court
	State.SentenceDays = 0;
	State.BailAmount = State.WantedStars * 5000 + (State.bSurrendered ? 0 : 10000);

	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess)
	{
		Sess->AddCurrency(ECurrency::Credits, -1000); // booking fee
		Sess->ShowMessage(FString::Printf(TEXT("BOOKED. Mugshot taken. Holding cell. Trial in %d days. Bail: %d credits."), State.DaysUntilTrial, State.BailAmount));
	}
}

bool UJusticeSystem::PostBail()
{
	if (State.Phase != ECourtPhase::Booked && State.Phase != ECourtPhase::AwaitingTrial) return false;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < State.BailAmount) return false;
	Sess->AddCurrency(ECurrency::Credits, -State.BailAmount);
	State.Phase = ECourtPhase::FreeOnBail;
	Sess->ShowMessage(TEXT("Bail posted. You're out until the trial. Don't skip town."));
	return true;
}

void UJusticeSystem::AdvanceTrial()
{
	if (State.DaysUntilTrial > 0) State.DaysUntilTrial--;
	if (State.DaysUntilTrial <= 0 && State.Phase == ECourtPhase::Booked)
		State.Phase = ECourtPhase::AwaitingTrial;
}

void UJusticeSystem::GoToCourt()
{
	if (State.Phase != ECourtPhase::AwaitingTrial) return;

	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return;

	// Player's legal skill affects outcome
	int32 LegalSkill = Sess->GetSkillLevel(FName("Pol_Investigation")) + Sess->GetSkillLevel(FName("Biz_Negotiation"));
	bool bGoodLawyer = LegalSkill > 30;
	int32 WantedPenalty = State.WantedStars * 5;
	int32 GoodBehaviourBonus = State.bSurrendered ? 10 : 0;

	// Verdict calculation
	int32 Score = LegalSkill / 2 + GoodBehaviourBonus - WantedPenalty + FMath::RandRange(-5, 10);
	bool bGuilty = Score < 15;

	State.Phase = ECourtPhase::Verdict;
	FString Verdict;
	if (bGuilty)
	{
		State.SentenceDays = FMath::Max(5, WantedPenalty * 2 - GoodBehaviourBonus + FMath::RandRange(0, 10));
		State.Phase = ECourtPhase::Sentenced;
		State.SentenceServed = 0;
		State.PrisonRing = Sess ? ((Sess->CurrentRing == ERing::MidWilshire) ? TEXT("Mid-Wilshire") : TEXT("Panem")) : TEXT("Mid-Wilshire");
		Verdict = FString::Printf(TEXT("GUILTY. Sentence: %d days in %s Correctional. Bail denied."), State.SentenceDays, *State.PrisonRing);
		Sess->RecordEvent(FString::Printf(TEXT("Convicted: %d days for %d-star crimes."), State.SentenceDays, State.WantedStars));
	}
	else
	{
		State.Phase = ECourtPhase::Free;
		Verdict = TEXT("NOT GUILTY. The jury acquits you. You're free to go. The court reporter records your vindication.");
		Sess->RecordEvent(TEXT("Acquitted: not guilty on all charges."));
		Sess->AddFame(5);
	}

	Sess->ShowMessage(Verdict);
}

FString UJusticeSystem::GetVerdict() const
{
	switch (State.Phase)
	{
	case ECourtPhase::Verdict: return TEXT("Verdict reached.");
	case ECourtPhase::Sentenced: return FString::Printf(TEXT("Sentenced: %d days. %d served."), State.SentenceDays, State.SentenceServed);
	case ECourtPhase::Free: return TEXT("Free.");
	default: return TEXT("Pending.");
	}
}

int32 UJusticeSystem::GetSentence() const { return State.SentenceDays; }

void UJusticeSystem::AcceptPleaDeal(int32 ReducedDays)
{
	State.SentenceDays = FMath::Max(3, ReducedDays);
	State.Phase = ECourtPhase::Sentenced;
	State.PrisonRing = TEXT("Mid-Wilshire");
	State.SentenceServed = 0;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(FString::Printf(TEXT("Plea deal accepted: %d days. You avoid trial."), State.SentenceDays));
}

// === Prison Life ===
void UJusticeSystem::AssignJob(EPrisonJob Job)
{
	State.CurrentJob = Job;
	const TCHAR* JobNames[] = { TEXT("None"), TEXT("Kitchen"), TEXT("Laundry"), TEXT("Library"), TEXT("Workshop"), TEXT("Yard"), TEXT("Medical"), TEXT("Chapel") };
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(FString::Printf(TEXT("Prison job: %s. Every day worked reduces your sentence."), JobNames[(int32)Job]));
}

void UJusticeSystem::WorkShift()
{
	if (State.CurrentJob == EPrisonJob::None || State.Phase != ECourtPhase::Sentenced) return;
	State.JobDaysWorked++;
	State.GoodBehaviourDays++;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess)
	{
		Sess->AddCurrency(ECurrency::Credits, 50); // prison wages
		Sess->AddSkillXP(FName("Soc_Empathy"), 5);
	}
}

void UJusticeSystem::VisitInmate()
{
	if (State.Phase != ECourtPhase::Sentenced) return;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(TEXT("A visitor comes to see you. They brought a book and news from outside. Morale improves."));
	State.GoodBehaviourDays++;
}

// === Prison Escape ===
void UJusticeSystem::PlanEscape(EEscapeMethod Method)
{
	State.EscapeMethod = Method;
	State.EscapeProgress = 0.f;
	const TCHAR* Methods[] = { TEXT("digging a tunnel"), TEXT("starting a riot"), TEXT("distracting the guard"), TEXT("forging ID papers"), TEXT("smuggling a key"), TEXT("signaling a helicopter"), TEXT("swapping with a visitor") };
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(FString::Printf(TEXT("Escape plan: %s. Work on it each day."), Methods[(int32)Method]));
}

void UJusticeSystem::WorkOnEscape()
{
	State.EscapeProgress += FMath::RandRange(5.f, 15.f);
	State.GoodBehaviourDays = FMath::Max(0, State.GoodBehaviourDays - 2); // escape planning hurts good behaviour
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess && State.EscapeProgress >= 50.f)
		Sess->ShowMessage(FString::Printf(TEXT("Escape progress: %.0f%%. Nearly ready."), State.EscapeProgress));
}

bool UJusticeSystem::AttemptEscape()
{
	if (State.EscapeProgress < 80.f) return false;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	int32 Skill = Sess->GetSkillLevel(FName("Combat_Stealth")) + Sess->GetSkillLevel(FName("Tech_Engineering"));
	bool bSuccess = FMath::RandRange(0, Skill + FMath::RoundToInt(State.EscapeProgress)) >= 60;

	if (bSuccess)
	{
		State.Phase = ECourtPhase::Escaped;
		State.WantedStars = 3; // you're a fugitive now
		Sess->WantedLevel = 3;
		Sess->ShowMessage(TEXT("YOU ESCAPED. The sirens blare. Guards shout. You're over the wall. But now you're a fugitive."));
		Sess->AddInfamy(20);
		return true;
	}

	Sess->ShowMessage(TEXT("ESCAPE FAILED. Solitary confinement for 5 days. Your sentence just got longer."));
	State.SentenceDays += 5;
	State.EscapeProgress = 0.f;
	return false;
}

// === Prison Riots ===
void UJusticeSystem::FomentRiot()
{
	State.RiotReadiness += FMath::RandRange(10, 25);
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(FString::Printf(TEXT("You whisper in the yard. Tensions rise. Riot readiness: %d%%."), State.RiotReadiness));
}

bool UJusticeSystem::CheckForRiot()
{
	if (State.RiotReadiness >= 100 && FMath::RandRange(0, 100) >= 40)
	{
		State.RiotReadiness = 0;
		return true;
	}
	if (FMath::RandRange(0, 200) < State.RiotReadiness / 2)
	{
		State.RiotReadiness = 0;
		return true; // spontaneous riot
	}
	return false;
}

FString UJusticeSystem::RiotOutcome()
{
	int32 Choice = FMath::RandRange(0, 3);
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	switch (Choice)
	{
	case 0:
		if (Sess) Sess->AddFame(3);
		ReduceSentence(5);
		return TEXT("RIOT: You protected the infirmary. Guards notice. Sentence reduced by 5 days for bravery.");
	case 1:
		State.EscapeProgress += 40.f;
		return TEXT("RIOT: In the chaos, you make significant progress on your escape plan. The guards are distracted.");
	case 2:
		State.SentenceDays += 10;
		if (Sess) Sess->AddInfamy(5);
		return TEXT("RIOT: You got caught fighting a guard. 10 extra days added. Worth it? Maybe.");
	default:
		State.GoodBehaviourDays += 3;
		return TEXT("RIOT: You stayed in your cell. The warden notices your restraint. Good behaviour noted.");
	}
}

// === Sentence Reduction & Release ===
void UJusticeSystem::ReduceSentence(int32 Days)
{
	State.SentenceDays = FMath::Max(0, State.SentenceDays - Days);
}

void UJusticeSystem::GoodBehaviour()
{
	if (State.Phase != ECourtPhase::Sentenced) return;
	State.GoodBehaviourDays++;
	State.SentenceServed++;
	if (State.SentenceServed >= GetRemainingSentence())
		ReleaseFromPrison();
}

void UJusticeSystem::ReleaseFromPrison()
{
	State.Phase = ECourtPhase::Free;
	State.WantedStars = 0;
	State.SentenceDays = 0;
	State.SentenceServed = 0;
	State.CurrentJob = EPrisonJob::None;
	State.EscapeProgress = 0.f;
	State.RiotReadiness = 0;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess)
	{
		Sess->WantedLevel = 0;
		Sess->ShowMessage(FString::Printf(TEXT("RELEASED. After %d days, you walk free. %d days off for good behaviour. The sun feels different."), State.JobDaysWorked + State.GoodBehaviourDays, State.GoodBehaviourDays));
		Sess->RecordEvent(TEXT("Released from prison."));
	}
}
