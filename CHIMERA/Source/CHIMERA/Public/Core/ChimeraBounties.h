// GDD 14.9 - Bounty Hunting & Mercenary System. The bounty board generates NPC
// targets from faction conflict; players with high infamy get bounties placed
// on their own heads.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "ChimeraBounties.generated.h"

USTRUCT(BlueprintType)
struct CHIMERA_API FBountyDef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) FString TargetName;
	UPROPERTY(EditAnywhere) FString Role;
	UPROPERTY(EditAnywhere) FString Ring;
	UPROPERTY(EditAnywhere) int32 Reward = 500;
	UPROPERTY(EditAnywhere) int32 AliveBonus = 250;
	UPROPERTY(EditAnywhere) FString Description;
};

UCLASS()
class CHIMERA_API UBountySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	const TArray<FBountyDef>& GetActiveBounties() const { return ActiveBounties; }
	int32 GetAcceptedIndex() const { return AcceptedIndex; }

	// GDD 14.9 - accept the next unclaimed bounty. Returns the accepted index or -1.
	int32 AcceptNextBounty();
	// Attempt to complete the accepted bounty. bAlive = capture alive (bonus).
	// Returns credits earned, or 0 on failure.
	int32 CompleteAcceptedBounty(bool bAlive);
	// Bounty hunters may find the player when infamy is high. Returns true if hunted.
	bool MaybeHuntPlayer(FString& OutHunterName, FString& OutEvent);

private:
	bool Tick(float DeltaTime);
	void RefreshPlayerBounty();
	void GenerateBounty();

	TArray<FBountyDef> ActiveBounties;
	TArray<bool> Claimed;         // per-bounty completion state
	int32 AcceptedIndex = -1;
	float GenTimer = 0.f;
	float HuntTimer = 0.f;
	FTSTicker::FDelegateHandle TickerHandle;
};
