#include "Core/ChimeraBounties.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraEconomy.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

// ---------------------------------------------------------------------------
// GDD 14.9 - Bounty Hunting & Mercenary System.
// ---------------------------------------------------------------------------
void UBountySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GenerateBounty();
	GenerateBounty();
	GenerateBounty();
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UBountySubsystem::Tick), 5.0f);
}

void UBountySubsystem::Deinitialize()
{
	if (TickerHandle.IsValid()) FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	Super::Deinitialize();
}

bool UBountySubsystem::Tick(float DeltaTime)
{
	// New contracts roll in from faction conflict (GDD 14.9).
	GenTimer += DeltaTime;
	if (GenTimer >= 45.f)
	{
		GenTimer = 0.f;
		GenerateBounty();
	}
	// Player bounty scales with infamy (GDD 14.9 / 14.8).
	RefreshPlayerBounty();
	HuntTimer += DeltaTime;
	if (HuntTimer >= 20.f)
	{
		HuntTimer = 0.f;
		if (UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		{
			FString Hunter, Ev;
			if (Sess->GetInfamy() >= 50 && MaybeHuntPlayer(Hunter, Ev))
			{
				Sess->ShowMessage(FString::Printf(TEXT("Bounty hunter %s is after you. %s"), *Hunter, *Ev));
			}
		}
	}
	return true;
}

void UBountySubsystem::RefreshPlayerBounty()
{
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return;
	int32 Target = Sess->GetInfamy() * 200;
	Sess->SetPlayerBounty(Target);
}

void UBountySubsystem::GenerateBounty()
{
	static const TArray<FBountyDef> Templates = {
		{ TEXT("Knuckles Malone"), TEXT("Petty thief"), TEXT("Mid-Wilshire"), 400, 200, TEXT("Stole a hyperloop vending machine. Alive preferred.") },
		{ TEXT("Sable Reyes"), TEXT("Rogue netrunner"), TEXT("Night City Expanse"), 800, 300, TEXT("Crashed the stock ticker for fun. Bring the wreckage.") },
		{ TEXT("The Iron Falcon"), TEXT("Supervillain"), TEXT("Aether City"), 2000, 500, TEXT("Ransoms civilian airships. No one knows the face.") },
		{ TEXT("Baron Vespucci"), TEXT("Noble poisoner"), TEXT("Neo-Kingdom"), 1200, 400, TEXT("Poisoned three lords. Wears a ruby signet.") },
		{ TEXT("Void Howler"), TEXT("Wild monster"), TEXT("Alagaesia Wilds"), 900, 0, TEXT("A wolf-like creature raiding farms. Dead is fine.") },
		{ TEXT("Corrupt Deputy Wexler"), TEXT("Dirty cop"), TEXT("Mid-Wilshire"), 1500, 600, TEXT("On the take from the Voodoo Boys. Evidence held.") },
		{ TEXT("Sabretooth Racing"), TEXT("Saboteur"), TEXT("Grand Coliseum"), 700, 250, TEXT("Flashed blue shells at rivals. Ban them.") }
	};
	if (ActiveBounties.Num() >= 8) return;
	FBountyDef B = Templates[FMath::RandRange(0, Templates.Num() - 1)];
	// Avoid duplicates already on the board.
	for (const FBountyDef& E : ActiveBounties) if (E.TargetName == B.TargetName) return;
	ActiveBounties.Add(B);
	Claimed.Add(false);
	UE_LOG(LogChimera, Log, TEXT("[Bounties] New contract: %s (%d credits)"), *B.TargetName, B.Reward);
}

int32 UBountySubsystem::AcceptNextBounty()
{
	for (int32 i = 0; i < ActiveBounties.Num(); ++i)
	{
		if (!Claimed[i])
		{
			AcceptedIndex = i;
			return i;
		}
	}
	return -1;
}

int32 UBountySubsystem::CompleteAcceptedBounty(bool bAlive)
{
	if (AcceptedIndex < 0 || AcceptedIndex >= ActiveBounties.Num()) return 0;
	FBountyDef& B = ActiveBounties[AcceptedIndex];
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	// Skill check: police/social for alive, combat for dead.
	int32 Check = bAlive ? Sess->GetSkillLevel(FName("Pol_Investigation")) : Sess->GetSkillLevel(FName("Combat_Blades"));
	if (Check > 0 && FMath::RandRange(0, Check + 10) >= 10)
	{
		int32 Reward = B.Reward + (bAlive ? B.AliveBonus : 0);
		Sess->AddCurrency(ECurrency::Credits, Reward);
		Claimed[AcceptedIndex] = true;
		if (bAlive)
		{
			Sess->AddRep(EFaction::NPD, 10);
			Sess->AddFame(5);
		}
		else
		{
			Sess->HeroVillainMeter = FMath::Clamp(Sess->HeroVillainMeter - 5, -100, 100);
		}
		Sess->RecordEvent(FString::Printf(TEXT("Bounty collected: %s (%s) +%d credits"), *B.TargetName,
			bAlive ? TEXT("alive") : TEXT("dead"), Reward));
		Sess->AddStat(TEXT("bounty_done"), 1);
		Sess->ShowMessage(FString::Printf(TEXT("Bounty complete: %s. +%d credits."), *B.TargetName, Reward));
		int32 Idx = AcceptedIndex;
		AcceptedIndex = -1;
		return Reward;
	}
	Sess->ShowMessage(FString::Printf(TEXT("The bounty on %s escaped. Sharpen your skills."), *B.TargetName));
	return 0;
}

bool UBountySubsystem::MaybeHuntPlayer(FString& OutHunterName, FString& OutEvent)
{
	static const TArray<FString> Hunters = { TEXT("Grim Vane"), TEXT("One-Eye Sanchez"), TEXT("The Debt Collector"), TEXT("Blade O'Connor") };
	if (FMath::RandRange(0, 100) >= 40) return false;
	OutHunterName = Hunters[FMath::RandRange(0, Hunters.Num() - 1)];
	OutEvent = TEXT("They track you by scent and credit history.");
	return true;
}
