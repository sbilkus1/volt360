// GDD 12 / 14 — Grand Unified Rebellion: the Hunger Games + Divergent + all CHIMERA rings.
// Characters from every universe pick sides. The Capitol & Erudite form one alliance.
// The Mockingjay, the Divergent, and the dragon riders form the other.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "ChimeraRebellion.generated.h"

UENUM(BlueprintType)
enum class ERebelPhase : uint8 { Peace, Sparks, DistrictRising, Coalition, CapitolSiege, Victory, COUNT };

USTRUCT(BlueprintType)
struct CHIMERA_API FRebelFaction
{
	GENERATED_BODY()
	UPROPERTY() FString Name;       // faction name
	UPROPERTY() FString Side;       // "Mockingjay" or "Capitol"
	UPROPERTY() FString Leader;
	UPROPERTY() FString Ring;       // which ring they're from
	UPROPERTY() int32 Strength = 5; // 1..10
};

UCLASS()
class CHIMERA_API URebellionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Grand rebellion across all worlds.
	void IgniteRebellion();
	ERebelPhase GetPhase() const { return Phase; }
	const TArray<FRebelFaction>& GetFactions() const { return Factions; }
	const TArray<FString>& GetRebelLog() const { return RebelLog; }
	int32 GetProgress() const { return Progress; }
	int32 GetMaxProgress() const { return MaxProgress; }

	// Called by interactables: perform a rebellion action with a skill check.
	bool RebelAction(const FString& Skill, const FString& Description);

	const TArray<FString>& GetMockingjayAllies() const { return MockingjayAllies; }
	const TArray<FString>& GetCapitolAllies() const { return CapitolAllies; }

private:
	void SeedFactions();
	bool Tick(float DeltaTime);

	ERebelPhase Phase = ERebelPhase::Peace;
	TArray<FRebelFaction> Factions;
	TArray<FString> RebelLog;
	TArray<FString> MockingjayAllies;
	TArray<FString> CapitolAllies;
	int32 Progress = 0;
	int32 MaxProgress = 25;
	float TickTimer = 0.f;
	FTSTicker::FDelegateHandle Ticker;
};
