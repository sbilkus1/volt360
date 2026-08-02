// GDD 12.8 - Heist Missions. Multi-phase planning & execution with crew selection.
// Each heist has approach options, prep missions, and skill-checked execution.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraHeists.generated.h"

UENUM(BlueprintType)
enum class EHeistApproach : uint8
{
	Stealth,
	Aggressive,
	Technical,
	Social,
	COUNT
};

UENUM(BlueprintType)
enum class EHeistPhase : uint8
{
	Planning,
	Prep,
	Execution,
	Getaway,
	Complete
};

USTRUCT(BlueprintType)
struct CHIMERA_API FHeistDef
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FString Id;
	UPROPERTY(EditAnywhere) FString Title;
	UPROPERTY(EditAnywhere) FString Target;
	UPROPERTY(EditAnywhere) FString Rings;
	UPROPERTY(EditAnywhere) TArray<FString> Approaches;       // e.g. "Stealth", "Aggressive", "Technical"
	UPROPERTY(EditAnywhere) int32 BasePayout = 0;
	UPROPERTY(EditAnywhere) FString Flavor;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FCrewMemberDef
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FString Name;
	UPROPERTY(EditAnywhere) FString Role;                     // Driver, Hacker, Gunslinger, Brawler, Socialite, Safecracker
	UPROPERTY(EditAnywhere) FString Specialization;            // e.g. "getaway driver", "netrunner", "assassin"
	UPROPERTY(EditAnywhere) int32 CutPercent = 20;
	UPROPERTY(EditAnywhere) int32 SkillLevel = 5;             // 1..10
};

UCLASS()
class CHIMERA_API UHeistSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Heist management.
	const TArray<FHeistDef>& GetHeists() const { return Heists; }
	const FHeistDef* GetActiveHeist() const { return bActive ? &ActiveHeist : nullptr; }
	EHeistPhase GetPhase() const { return Phase; }
	const FString& GetApproach() const { return ActiveApproach; }
	int32 GetHeistProgress() const { return HeistProgress; }
	int32 GetHeistMaxProgress() const { return HeistMaxProgress; }

	// Start a heist: select heist by index and approach string.
	bool StartHeist(int32 HeistIdx, const FString& Approach);
	void AdvancePhase();

	// Crew selection.
	const TArray<FCrewMemberDef>& GetCrew() const { return Crew; }
	bool HireCrew(int32 CrewIdx);
	const TArray<int32>& GetHiredIndices() const { return HiredIndices; }

	// Execution - skill-checked actions that progress the heist.
	bool PerformHeistAction(const FString& SkillId, const FString& ActionDesc);

	// Payday - splits payout among hired crew.
	int32 ResolvePayout();

private:
	void SeedHeists();
	void SeedCrew();

	TArray<FHeistDef> Heists;
	TArray<FCrewMemberDef> Crew;
	TArray<int32> HiredIndices;
	FHeistDef ActiveHeist;
	FString ActiveApproach;
	EHeistPhase Phase = EHeistPhase::Planning;
	bool bActive = false;
	int32 HeistProgress = 0;
	int32 HeistMaxProgress = 4;
	int32 HeistMistakes = 0;
};
