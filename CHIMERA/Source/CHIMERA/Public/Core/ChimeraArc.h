// GDD 8 / 12 — Mission Arc System. All quests chain into the final Convergence.
// GDD 14.12 — Character Roster. GTA V-style multi-character swapping.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraArc.generated.h"

// ======================================================================
// Story Chapter Progression
// ======================================================================
UENUM(BlueprintType)
enum class EStoryChapter : uint8
{
	Prologue,           // The Crash, character creation, Nexus introduction
	Act1_Convergence,   // Establish yourself: reach level 5, visit 3 rings, complete 1 quest
	Act2_Fractures,     // Complete ring-specific fracture missions (Aether, Mercy, MidWilshire, etc)
	Act3_Alliance,      // Complete Hunger Games, rebellion, heist — build your coalition
	Act4_Endgame,       // All rings visited, dungeon floor 100+, rebellion won or lost
	Finale_Choice,      // The Convergence: Separation, Integration, Transcendence, or Control
	Complete,           // Post-game — everything unlocked, world reflects your choice
	COUNT
};

// ======================================================================
// Playable Character Roster (GTA V-style swapping)
// ======================================================================
// Slot 0: YOUR created character (always available)
// Slot 1-4: Story characters unlocked through chapter progression
//   Katniss Everdeen (Act I, Panem) — the Mockingjay, bow, survival
//   Tris Prior (Act II, Chicago) — Dauntless, divergent, combat
//   Meredith Grey (Act III, Seattle) — surgeon, leader, empathy
//   John Nolan (Act IV, Mid-Wilshire) — cop, mentor, negotiation
// Like GTA V: L key swa
USTRUCT()
struct FMStoryCharacter
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString Title;
	UPROPERTY() FString Ring;     // where they start
	UPROPERTY() FString Story;    // their personal motivation
	UPROPERTY() int32 UnlockChapter;  // which EStoryChapter unlocks them (as int)
	UPROPERTY() bool bUnlocked = false;
};

struct FRosterMember
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() FString LifePath;
	UPROPERTY() int32 Level = 1;
	UPROPERTY() FString Ring;
	UPROPERTY() bool bActive = false;
	UPROPERTY() float SecondsPlayed = 0.f;
	UPROPERTY() bool bIsStoryChar = false; // true for Katniss/Tris/Meredith/Nolan
};

UCLASS()
class CHIMERA_API UStoryArcSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// === Story Chapter Progression ===
	EStoryChapter GetChapter() const { return Chapter; }
	FString GetChapterName() const;
	FString GetChapterObjective() const;
	const TArray<FString>& GetCompletedMissions() const { return CompletedMissions; }

	// Check and update chapter — called after completing major events
	void CheckChapterProgress();
	void AdvanceToNextChapter();
	bool CompleteMission(const FString& MissionId);

	// Finale — what ending did you choose?
	UFUNCTION() EFinalChoice GetEnding() const { return Ending; }
	void SetEnding(EFinalChoice End) { Ending = End; Chapter = EStoryChapter::Complete; }
	bool HasFinishedStory() const { return Chapter >= EStoryChapter::Complete; }

	// === Character Roster (GTA V style) ===
	const TArray<FRosterMember>& GetRoster() const { return Roster; }
	const TArray<FMStoryCharacter>& GetStoryCharacters() const { return StoryChars; }
	int32 GetActiveSlot() const { return ActiveSlot; }

	// Create your main character (slot 0, always available)
	bool CreateMainCharacter(const FString& Name, const FString& LifePath);
	// Check for story character unlocks on chapter advancement
	void CheckStoryUnlocks();
	// Swap to a different character
	bool SwitchCharacter(int32 Slot);
	// Get the currently active character
	FRosterMember* GetActiveCharacter();
	const FRosterMember* GetActiveCharacter() const;

	// AI controls inactive characters
	void TickInactiveCharacters(float DeltaTime);

private:
	void SetupStoryCharacters();
	TArray<FMStoryCharacter> StoryChars; // immutable — the 4 GTA-style story chars
	EStoryChapter Chapter = EStoryChapter::Prologue;
	EFinalChoice Ending = EFinalChoice::COUNT;
	TArray<FString> CompletedMissions;
	TArray<FRosterMember> Roster;
	int32 ActiveSlot = -1;
	float SwapCooldown = 0.f;
};
