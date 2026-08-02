// GDD 6.7 — Police / FBI investigation subsystem.
// Full procedural: open cases, collect evidence, interview witnesses, make arrests.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraPolice.generated.h"

UCLASS()
class CHIMERA_API UPoliceInvestigation : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Case management.
	const TArray<FPoliceCase>& GetCases() const { return Cases; }
	const FPoliceCase* GetActiveCase() const { return ActiveIdx >= 0 && ActiveIdx < Cases.Num() ? &Cases[ActiveIdx] : nullptr; }
	int32 GetActiveCaseIndex() const { return ActiveIdx; }

	// Open a new procedural case based on the player's location/ring.
	void OpenNewCase();
	// Collect a piece of evidence (from an interactable).
	bool CollectEvidence(const FString& Name, EEvidenceType Type, int32 Weight = 1);
	// Interview a witness / suspect (skill check).
	FString InterviewSuspect(const FString& SuspectName);
	// Attempt to make an arrest.
	bool MakeArrest();
	// Close the active case.
	void CloseCase(bool bArrested);

private:
	void SeedCasePool();
	TArray<FPoliceCase> Cases;
	TArray<FPoliceCase> CasePool;
	int32 ActiveIdx = -1;
};
