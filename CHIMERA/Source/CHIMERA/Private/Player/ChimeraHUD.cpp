#include "Player/ChimeraHUD.h"
#include "Player/ChimeraPlayerController.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraRecords.h"
#include "Core/ChimeraArc.h"
#include "Core/ChimeraEconomy.h"
#include "Core/ChimeraWorldSim.h"
#include "Core/ChimeraQuests.h"
#include "Minigames/ChimeraMinigames.h"
#include "Character/ChimeraCharacter.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "CHIMERA.h"

void AChimeraHUD::DrawHUD()
{
	if (!Canvas) return;
	const float W = Canvas->SizeX;
	const float H = Canvas->SizeY;

	AChimeraPlayerController* PC = Cast<AChimeraPlayerController>(PlayerOwner);
	if (PC && PC->ActiveMinigame)
	{
		PC->ActiveMinigame->Draw(Canvas, W, H);
		return;
	}
	if (PC && PC->bShowPause) { DrawPause(W, H); return; }
	if (PC && PC->bShowMap) { DrawMap(W, H); return; }
	if (PC && PC->bShowMarket) { DrawMarket(W, H); return; }

	DrawMainHUD(W, H);
}

void AChimeraHUD::DrawMainHUD(float W, float H)
{
	UChimeraSessionSubsystem* Sess = GetWorld()->GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return;
	UFont* F = GEngine->GetSmallFont();

	// GDD 1.1 - current ring.
	const TArray<FRingDef>& Rings = FChimeraData::Rings();
	const FRingDef& Ring = Rings[(int32)Sess->CurrentRing];
	Canvas->DrawText(F, FString::Printf(TEXT("CHIMERA - %s  (%s)"), *Ring.Name, *Ring.Genre),
		FVector2D(20, 16), FVector2D(1.8f, 1.8f), FLinearColor::Cyan);

	// GDD 5.2 - top skills + level.
	FString SkillLine = TEXT("Skills: ");
	int32 Count = 0;
	TArray<FSkillState> Sorted = Sess->Skills;
	Sorted.Sort([](const FSkillState& A, const FSkillState& B) { return A.Level > B.Level; });
	for (int32 i = 0; i < Sorted.Num() && Count < 5; ++i)
	{
		if (Sorted[i].Level <= 1) continue;
		SkillLine += FString::Printf(TEXT("%s %d  "), *Sorted[i].Id.ToString(), Sorted[i].Level);
		Count++;
	}
	Canvas->DrawText(F, SkillLine, FVector2D(20, 44), FVector2D(1.1f, 1.1f), FLinearColor(0.8f, 0.9f, 1.f));

	// GDD 9.1 - currencies.
	const TArray<FString>& CurNames = FChimeraData::CurrencyNames();
	FString Money;
	for (int32 i = 0; i < CurNames.Num(); ++i)
	{
		Money += FString::Printf(TEXT("%s: %d   "), *CurNames[i], Sess->GetCurrency((ECurrency)i));
	}
	Canvas->DrawText(F, Money, FVector2D(20, 64), FVector2D(1.1f, 1.1f), FLinearColor::Yellow);

	// GDD 8 — story chapter + character roster.
	if (auto* Arc = GetGameInstance()->GetSubsystem<UStoryArcSubsystem>())
	{
		Canvas->DrawText(F, FString::Printf(TEXT("%s — %s"), *Arc->GetChapterName(), *Arc->GetChapterObjective()),
			FVector2D(W / 2 - 400, 16), FVector2D(0.85f, 0.85f),
			Arc->HasFinishedStory() ? FLinearColor(0.5f, 1.f, 0.5f) : FLinearColor(1.f, 0.9f, 0.5f));

		// Character roster: show all unlocked characters
		FString RostText = TEXT("[L] Roster: ");
		for (int32 i = 0; i < Arc->GetRoster().Num(); ++i)
		{
			RostText += (i == Arc->GetActiveSlot()) ? FString::Printf(TEXT("[%s] "), *Arc->GetRoster()[i].Name) : FString::Printf(TEXT("%s "), *Arc->GetRoster()[i].Name);
		}
		if (Arc->GetRoster().Num() == 1) RostText += TEXT(" (more unlock as you progress)");
		Canvas->DrawText(F, RostText, FVector2D(W / 2 - 350, 36), FVector2D(0.85f, 0.85f), FLinearColor(0.7f, 0.9f, 1.f));
	}

	// GDD 4.5 - wanted stars.
	FString Wanted;
	for (int32 i = 0; i < Sess->WantedLevel; ++i) Wanted += TEXT("*");
	Canvas->DrawText(F, FString::Printf(TEXT("Wanted: %s   Hero/Villain: %d   Governor: %s"),
		Wanted.IsEmpty() ? TEXT("0") : *Wanted, Sess->HeroVillainMeter, *Sess->Governor),
		FVector2D(20, 84), FVector2D(1.1f, 1.1f), FLinearColor::Red);

	// GDD 14.8 / 14.9 - fame, infamy, player bounty.
	Canvas->DrawText(F, FString::Printf(TEXT("Fame: %d   Infamy: %d   Bounty on you: %d"),
		Sess->GetFame(), Sess->GetInfamy(), Sess->GetPlayerBounty()),
		FVector2D(20, 96), FVector2D(1.1f, 1.1f), FLinearColor(1.f, 0.85f, 0.6f));

	// GDD 6.9 / 14.3 - dragon bond + companion.
	FString Dragon = Sess->bHasDragon ?
		FString::Printf(TEXT("%s bond %d/20 (%s)"), *Sess->GetDragonName(), Sess->GetDragonBond(),
			Sess->CanFlyDragon() ? TEXT("READY") : TEXT("growing")) : TEXT("No dragon");
	FString Pet = Sess->HasPet() ?
		FString::Printf(TEXT("%s the %s lvl %d bond %d"), *Sess->GetPetName(), *Sess->GetPetType(),
			Sess->GetPetLevel(), Sess->GetPetBond()) : TEXT("No companion");
	Canvas->DrawText(F, FString::Printf(TEXT("Dragon: %s   Companion: %s"), *Dragon, *Pet),
		FVector2D(20, 108), FVector2D(1.1f, 1.1f), FLinearColor(0.9f, 1.f, 0.85f));

	// GDD 1.3 - day/night clock + weather + inventory/property/insurance.
	FString Clock = FString::Printf(TEXT("Day %d  %02d:%02d  %s  Season: %s"),
		Sess->GetGameDay(), FMath::FloorToInt(Sess->GetHourOfDay()), FMath::FloorToInt(FMath::Frac(Sess->GetHourOfDay()) * 60.f),
		*Sess->GetWeather(), *Sess->GetSeason());
	Canvas->DrawText(F, Clock, FVector2D(20, 136), FVector2D(1.1f, 1.1f), FLinearColor(0.7f, 1.f, 0.9f));
	Canvas->DrawText(F, FString::Printf(TEXT("Inventory: %d types   Properties: %d   Insurance: %d   Income: %d/h"),
		Sess->Inventory.Num(), Sess->GetOwnedProperties().Num(), Sess->ActiveInsurances.Num(), Sess->GetPassiveIncomePerHour()),
		FVector2D(20, 156), FVector2D(1.1f, 1.1f), FLinearColor(0.9f, 0.85f, 1.f));

	// GDD 12 - active quest objective.
	if (!Sess->CurrentObjective.IsEmpty())
	{
		Canvas->DrawText(F, FString::Printf(TEXT("OBJECTIVE: %s"), *Sess->CurrentObjective),
			FVector2D(W / 2 - 400, H - 40), FVector2D(1.5f, 1.5f), FLinearColor::Green);
	}

	// GDD 14.5 - disaster banner.
	if (auto* Dis = GetWorld()->GetSubsystem<UDisasterSubsystem>())
	{
		if (Dis->bDisasterActive)
		{
			Canvas->DrawText(F, FString::Printf(TEXT("!! DISASTER: %s in %s - respond as hero/doctor/cop/racer/builder (%.0fs)"),
				*Dis->ActiveDisaster, *Rings[(int32)Dis->DisasterRing].Name, Dis->RemainingSeconds),
				FVector2D(W / 2 - 500, 20), FVector2D(1.5f, 1.5f), FLinearColor(1.f, 0.3f, 0.3f));
		}
	}

	// GDD 14.1 - nemeses.
	if (auto* Nem = GetGameInstance()->GetSubsystem<UNemesisSubsystem>())
	{
		if (Nem->Rank() > 0)
		{
			Canvas->DrawText(F, FString::Printf(TEXT("NEMESES: %d - they remember."), Nem->Rank()),
				FVector2D(W - 320, 40), FVector2D(1.3f, 1.3f), FLinearColor(1.f, 0.5f, 0.f));
		}
	}

	// GDD 9.2 - BAWSAQ mini ticker.
	if (auto* BQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>())
	{
		const TArray<FStockDef>& Stocks = BQ->GetStocks();
		if (Stocks.Num() > 0)
		{
			Canvas->DrawText(F, FString::Printf(TEXT("BAWSAQ: %s %.0f  %s %.0f  %s %.0f (Tab)"),
				*Stocks[0].Symbol, Stocks[0].Price, *Stocks[1].Symbol, Stocks[1].Price,
				*Stocks[6].Symbol, Stocks[6].Price),
				FVector2D(20, H - 24), FVector2D(1.1f, 1.1f), FLinearColor(0.6f, 1.f, 0.6f));
		}
	}

	// GDD 14.18 - news ticker.
	Canvas->DrawText(F, *Sess->GetLatestHeadline(),
		FVector2D(20, H - 60), FVector2D(1.0f, 1.0f), FLinearColor(1.f, 0.95f, 0.7f));

	// GDD 3.4 - persistent career milestone.
	FString Career;
	int32 Best = 0;
	for (const FSkillState& S : Sess->Skills) Best = FMath::Max(Best, S.Level);
	Career = Best >= 100 ? TEXT("NEXT: LEGENDARY RESET (skill 100)") :
		FString::Printf(TEXT("NEXT MILESTONE: reach level %d in any skill"), FMath::Min(100, (Best / 10 + 1) * 10));
	Canvas->DrawText(F, Career, FVector2D(20, H - 44), FVector2D(1.1f, 1.1f), FLinearColor::Magenta);

	// Interact prompt.
	if (AChimeraCharacter* C = Cast<AChimeraCharacter>(GetOwningPawn()))
	{
		if (C->Focus)
		{
			Canvas->DrawText(F, FString::Printf(TEXT("[E] %s"), *C->Focus->Prompt),
				FVector2D(W / 2 - 120, H / 2 + 120), FVector2D(1.4f, 1.4f), FLinearColor::White);
		}
	Canvas->DrawText(F, TEXT("E interact | F melee | R shoot | C stealth | Tab market | M map | P menu"),
		FVector2D(20, H - 24), FVector2D(0.9f, 0.9f), FLinearColor(0.6f, 0.6f, 0.6f));
	}

	// Camera mode indicator
	if (AChimeraCharacter* C = Cast<AChimeraCharacter>(GetOwningPawn()))
	{
		const TCHAR* CamNames[] = { TEXT("3rd Person"), TEXT("Over-Shoulder"), TEXT("Wide"), TEXT("FPS"), TEXT("Top Down"), TEXT("Drone"), TEXT("Cinematic") };
		Canvas->DrawText(F, FString::Printf(TEXT("[V] %s"), CamNames[(int32)C->CurrentCamera]),
			FVector2D(W / 2 - 50, 24), FVector2D(0.9f, 0.9f), FLinearColor(0.5f, 0.8f, 0.5f));
	}

	// GDD 14.11 - festival banner.
	if (Sess->bFestivalActive)
	{
		Canvas->DrawText(F, FString::Printf(TEXT("   %s! 1.5x XP for all skills today!"), *Sess->ActiveFestival),
			FVector2D(W / 2 - 220, 140), FVector2D(1.4f, 1.4f), FLinearColor(1.f, 0.85f, 0.3f));
	}

	// GDD 14.12 / 14.18 - achievements unlocked count.
	if (auto* Rec = GetGameInstance()->GetSubsystem<UHallOfRecords>())
	{
		int32 Unlocked = Rec->GetUnlocked().Num();
		int32 Total = Rec->GetAchievements().Num();
		Canvas->DrawText(F, FString::Printf(TEXT("Achievements: %d/%d"), Unlocked, Total),
			FVector2D(W - 200, 84), FVector2D(1.0f, 1.0f), FLinearColor(1.f, 0.9f, 0.5f));
	}

	// GDD 14.19.2 - NPC/NPC-adjacent world messages.
	if (Sess->LastMessageTime > 0.f)
	{
		Canvas->DrawText(F, *Sess->LastMessage,
			FVector2D(W / 2 - 420, H - 110), FVector2D(1.3f, 1.3f), FLinearColor(1.f, 1.f, 0.85f));
	}
}

void AChimeraHUD::DrawMinigame(float W, float H)
{
	AChimeraPlayerController* PC = Cast<AChimeraPlayerController>(PlayerOwner);
	if (PC && PC->ActiveMinigame) PC->ActiveMinigame->Draw(Canvas, W, H);
}

void AChimeraHUD::DrawMap(float W, float H)
{
	UChimeraSessionSubsystem* Sess = GetWorld()->GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return;
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, TEXT("NEXOPOLIS - Ring Transit System (M to close)"), FVector2D(40, 30), FVector2D(2.f, 2.f), FLinearColor::Cyan);
	const TArray<FRingDef>& Rings = FChimeraData::Rings();
	float Y = 100;
	for (int32 i = 0; i < Rings.Num(); ++i)
	{
		bool Here = (i == (int32)Sess->CurrentRing);
		Canvas->DrawText(F, FString::Printf(TEXT("%s %s - %s (%s)"),
			Here ? TEXT(">>") : TEXT("  "), *Rings[i].Name, *Rings[i].Genre, *Rings[i].Source),
			FVector2D(80, Y), FVector2D(1.3f, 1.3f), Here ? FLinearColor::Green : FLinearColor::White);
		Y += 30;
	}
	Canvas->DrawText(F, TEXT("Walk to a ring gate to fast-travel. Dragon flight unlocks via the egg."),
		FVector2D(80, Y + 20), FVector2D(1.1f, 1.1f), FLinearColor(0.7f, 0.7f, 0.7f));
}

void AChimeraHUD::DrawMarket(float W, float H)
{
	UChimeraSessionSubsystem* Sess = GetWorld()->GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	UBAWSAQSubsystem* BQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>();
	if (!Sess || !BQ) return;
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, TEXT("BAWSAQ 2.0 - every player action moves the market (Tab to close)"),
		FVector2D(40, 30), FVector2D(2.f, 2.f), FLinearColor(0.6f, 1.f, 0.6f));
	Canvas->DrawText(F, FString::Printf(TEXT("Credits: %d"), Sess->GetCurrency(ECurrency::Credits)),
		FVector2D(40, 70), FVector2D(1.3f, 1.3f), FLinearColor::Yellow);
	const TArray<FStockDef>& Stocks = BQ->GetStocks();
	float Y = 120;
	for (int32 i = 0; i < Stocks.Num(); ++i)
	{
		Canvas->DrawText(F, FString::Printf(TEXT("%-6s %-28s %.2f   (W/S select, Enter buy 1)"),
			*Stocks[i].Symbol, *Stocks[i].Name, Stocks[i].Price),
			FVector2D(60, Y), FVector2D(1.1f, 1.1f), FLinearColor::White);
		Y += 26;
	}
}

void AChimeraHUD::DrawPause(float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, TEXT("PAUSED - the world keeps simulating (GDD 6.10)"),
		FVector2D(W / 2 - 260, H / 2 - 120), FVector2D(2.f, 2.f), FLinearColor::White);
	Canvas->DrawText(F, TEXT("[Esc] Resume    [R] Retire & begin the Legacy (GDD 3.6)"),
		FVector2D(W / 2 - 260, H / 2 - 40), FVector2D(1.3f, 1.3f), FLinearColor::Yellow);
	Canvas->DrawText(F, TEXT("Your retired character becomes an NPC mentor. The heir inherits 20% of skills and the world state."),
		FVector2D(W / 2 - 360, H / 2 + 10), FVector2D(1.0f, 1.0f), FLinearColor(0.7f, 0.7f, 0.7f));
}

void AChimeraHUD::DrawBar(float X, float Y, float W, float H, float Pct, FLinearColor Color)
{
	Canvas->DrawRect(FLinearColor(0.1f, 0.1f, 0.1f), X, Y, W, H);
	Canvas->DrawRect(Color, X, Y, W * FMath::Clamp(Pct, 0.f, 1.f), H);
}
