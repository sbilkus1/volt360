// GDD 13.7 — Dating & Romance subsystem. Relationship stages, gifts, dates, milestones.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraDating.generated.h"

USTRUCT(BlueprintType)
struct FMDateEntry
{
	GENERATED_BODY()
	UPROPERTY() FString NpcName;
	UPROPERTY() int32 Stage = 0;     // 0=stranger,1=friend,2=attraction,3=dating,4=committed,5=married
	UPROPERTY() int32 Affection = 0;  // 0..100
	UPROPERTY() int32 Dates = 0;
	UPROPERTY() int32 Gifts = 0;
	UPROPERTY() FString LastActivity;
};

UCLASS()
class CHIMERA_API UDatingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const TArray<FMDateEntry>& GetPartners() const { return Partners; }
	const FMDateEntry* GetPartner(const FString& NpcName) const;
	FMDateEntry* GetPartnerMutable(const FString& NpcName);

	FString GetRelationStageName(int32 Stage) const;
	bool AdvanceStage(const FString& NpcName);
	bool GiveGift(const FString& NpcName, const FString& Gift);
	bool GoOnDate(const FString& NpcName, const FString& Activity);
	FString Breakup(const FString& NpcName);

private:
	TArray<FMDateEntry> Partners;
};
