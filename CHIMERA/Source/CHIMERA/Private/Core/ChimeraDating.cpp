#include "Core/ChimeraDating.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"

void UDatingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

const FMDateEntry* UDatingSubsystem::GetPartner(const FString& NpcName) const
{
	for (const FMDateEntry& P : Partners) if (P.NpcName == NpcName) return &P;
	return nullptr;
}

FMDateEntry* UDatingSubsystem::GetPartnerMutable(const FString& NpcName)
{
	for (FMDateEntry& P : Partners) if (P.NpcName == NpcName) return &P;
	return nullptr;
}

FString UDatingSubsystem::GetRelationStageName(int32 Stage) const
{
	switch (Stage) { case 1: return TEXT("Friend"); case 2: return TEXT("Attraction"); case 3: return TEXT("Dating");
		case 4: return TEXT("Committed"); case 5: return TEXT("Married"); default: return TEXT("Stranger"); }
}

bool UDatingSubsystem::AdvanceStage(const FString& NpcName)
{
	FMDateEntry* P = GetPartnerMutable(NpcName);
	if (!P) { FMDateEntry E; E.NpcName = NpcName; E.Stage = 1; E.Affection = 10; Partners.Add(E);
		if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
			Sess->ShowMessage(FString::Printf(TEXT("%s has become your friend."), *NpcName)); return true; }
	if (P->Stage >= 5) return false;
	P->Stage++;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(FString::Printf(TEXT("Relationship with %s: %s"), *NpcName, *GetRelationStageName(P->Stage)));
	return true;
}

bool UDatingSubsystem::GiveGift(const FString& NpcName, const FString& Gift)
{
	FMDateEntry* P = GetPartnerMutable(NpcName);
	if (!P) { AdvanceStage(NpcName); P = GetPartnerMutable(NpcName); }
	P->Affection = FMath::Min(100, P->Affection + 10);
	P->Gifts++;
	if (P->Affection >= 80 && P->Stage < 3) AdvanceStage(NpcName);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(FString::Printf(TEXT("Gave %s to %s. Affection now %d."), *Gift, *NpcName, P->Affection));
	return true;
}

bool UDatingSubsystem::GoOnDate(const FString& NpcName, const FString& Activity)
{
	FMDateEntry* P = GetPartnerMutable(NpcName);
	if (!P || P->Stage < 2) return false;
	P->Affection = FMath::Min(100, P->Affection + 15);
	P->Dates++;
	P->LastActivity = Activity;
	if (P->Affection >= 90 && P->Stage < 4) AdvanceStage(NpcName);
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(FString::Printf(TEXT("Date: %s with %s. Affection %d."), *Activity, *NpcName, P->Affection));
	return true;
}

FString UDatingSubsystem::Breakup(const FString& NpcName)
{
	for (int32 i = 0; i < Partners.Num(); ++i)
	{
		if (Partners[i].NpcName == NpcName)
		{
			FString Msg = FString::Printf(TEXT("You and %s have parted ways after %d dates and %d gifts."), *NpcName, Partners[i].Dates, Partners[i].Gifts);
			if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>()) Sess->ShowMessage(Msg);
			Partners.RemoveAt(i);
			return Msg;
		}
	}
	return TEXT("No relationship to end.");
}
