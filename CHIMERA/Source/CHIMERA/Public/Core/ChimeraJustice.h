// GDD 6.7 — Criminal Justice System. Arrests, courts, prisons, escapes, riots.
// Unlike GTA V: police arrest you. You get a trial. You can escape prison.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraJustice.generated.h"

UENUM() enum class EPrisonJob : uint8 { None, Kitchen, Laundry, Library, Workshop, YardDuty, Medical, Chapel, COUNT };
UENUM() enum class ECourtPhase : uint8 { Free, Arrested, Booked, AwaitingTrial, InCourt, Verdict, Sentenced, FreeOnBail, Escaped, COUNT };
UENUM() enum class EEscapeMethod : uint8 { Tunnel, Riot, GuardDistraction, FakeID, ContrabandKey, Helicopter, VisitorSwap, COUNT };

USTRUCT()
struct FJusticeState
{
	GENERATED_BODY()
	ECourtPhase Phase = ECourtPhase::Free;
	int32 WantedStars = 0;           // current wanted level (0-5)
	int32 DaysUntilTrial = 0;       // court date
	int32 SentenceDays = 0;         // how long in prison
	int32 SentenceServed = 0;       // days already served
	int32 BailAmount = 0;           // credits to post bail
	float EscapeProgress = 0.f;     // 0..100, digging/bribing progress
	EEscapeMethod EscapeMethod = EEscapeMethod::Tunnel;
	EPrisonJob CurrentJob = EPrisonJob::None;
	int32 JobDaysWorked = 0;
	int32 GoodBehaviourDays = 0;     // reduces sentence
	int32 RiotReadiness = 0;         // how close to a riot
	bool bHasBluesuit = true;        // prison uniform
	bool bSurrendered = false;       // surrendered peacefully (better for court)
	FString PrisonRing;              // which ring the prison is in
};

UCLASS()
class CHIMERA_API UJusticeSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// === Arrest & Booking ===
	FJusticeState& GetState() { return State; }
	bool IsFree() const { return State.Phase == ECourtPhase::Free || State.Phase == ECourtPhase::FreeOnBail; }
	bool IsArrested() const { return State.Phase >= ECourtPhase::Arrested && State.Phase < ECourtPhase::FreeOnBail; }
	bool IsInPrison() const { return State.Phase == ECourtPhase::Sentenced; }

	// Surrender to police (reduces sentence, avoids death)
	void Surrender();
	// Fight it out — if you lose, you get arrested with worse terms
	void ResistArrest();

	// Booking process (fingerprints, mugshot, holding cell)
	void BookInmate();

	// === Bail ===
	bool PostBail();

	// === Court System ===
	void AdvanceTrial();            // move clock toward trial date
	void GoToCourt();               // the trial itself
	FString GetVerdict() const;     // guilty/not guilty/plea deal
	int32 GetSentence() const;      // days in prison
	void AcceptPleaDeal(int32 ReducedDays);

	// === Prison Life ===
	void AssignJob(EPrisonJob Job);
	void WorkShift();               // one day of prison work
	void VisitInmate();             // NPC visits you (raises morale)

	// === Prison Escape ===
	void PlanEscape(EEscapeMethod Method);
	void WorkOnEscape();            // one day of escape progress
	bool AttemptEscape();           // final check — skill + luck

	// === Prison Riots ===
	void FomentRiot();              // stir up trouble
	bool CheckForRiot();            // chance of riot breaking out
	FString RiotOutcome();          // what you did during the riot

	// === Progressive Sentence Reduction ===
	void ReduceSentence(int32 Days);
	void GoodBehaviour();           // called daily if behaving well
	int32 GetRemainingSentence() const { return FMath::Max(0, State.SentenceDays - State.SentenceServed - State.GoodBehaviourDays); }

	// === Release ===
	void ReleaseFromPrison();

private:
	FJusticeState State;
	float PrisonTimer = 0.f;
};
