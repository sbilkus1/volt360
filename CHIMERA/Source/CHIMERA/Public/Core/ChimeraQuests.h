// GDD 8 (narrative) + 12 (missions & quests). Quest state machine with mission
// ratings and a procedural radiant generator, plus cross-ring objective chains.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraQuests.generated.h"

UCLASS()
class CHIMERA_API UQuestSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// GDD 12.1 - mission tagging.
	void StartQuest(const FName& Id);
	void GenerateRadiantQuest(); // GDD 12.6
	void ProgressObjective(const FName& StatKey, int32 Amt = 1);
	void OnMistake() { Mistakes++; }
	void TickQuest(float Delta) { if (bActive) QuestTimer += Delta; }

	const TArray<FQuestDef>& GetAvailableQuests() const { return AvailableQuests; }
	const FQuestDef* GetActive() const { return bActive ? &Active : nullptr; }
	int32 GetObjectiveIndex() const { return ObjectiveIndex; }

	// GDD 12.9 — Mission Rating System.
	FString GetRating() const;
	float GetQuestTime() const { return QuestTimer; }

private:
	void AdvanceObjective();
	void FinalizeQuest();

	TArray<FQuestDef> AvailableQuests;
	FQuestDef Active;
	int32 ObjectiveIndex = 0;
	int32 Mistakes = 0;
	float QuestTimer = 0.f;
	bool bActive = false;
};
