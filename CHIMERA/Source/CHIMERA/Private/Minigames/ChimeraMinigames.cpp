#include "Minigames/ChimeraMinigames.h"
#include "Character/ChimeraCharacter.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraStory.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "CHIMERA.h"

// ---------------------------------------------------------------------------
// Base minigame.
// ---------------------------------------------------------------------------
void UChimeraMinigame::Begin(AChimeraCharacter* InstigatorChar)
{
	Char = InstigatorChar;
	bComplete = false;
	Status.Empty();
}

void UChimeraMinigame::End()
{
	bComplete = true;
}

void UChimeraMinigame::Finish(bool bSuccess, int32 Score)
{
	if (Char)
	{
		Char->HandleMinigameResult(Kind, bSuccess, Score);
	}
	End();
}

UChimeraMinigame* FChimeraMinigameFactory::Create(EMinigameKind Kind, UObject* Outer)
{
	switch (Kind)
	{
	case EMinigameKind::BreachProtocol: return NewObject<UBreachProtocolMinigame>(Outer);
	case EMinigameKind::Surgery: return NewObject<USurgeryMinigame>(Outer);
	case EMinigameKind::Racing: return NewObject<URacingMinigame>(Outer);
	case EMinigameKind::Lockpicking: return NewObject<ULockpickMinigame>(Outer);
	case EMinigameKind::Interrogation: return NewObject<UInterrogationMinigame>(Outer);
	case EMinigameKind::Braindance: return NewObject<UBraindanceMinigame>(Outer);
	case EMinigameKind::Dance: return NewObject<UDanceMinigame>(Outer);
	case EMinigameKind::FootballShootout: return NewObject<UFootballShootoutMinigame>(Outer);
	case EMinigameKind::Fishing: return NewObject<UFishingMinigame>(Outer);
	case EMinigameKind::Poker: return NewObject<UPokerMinigame>(Outer);
	case EMinigameKind::DroneRacing: return NewObject<UDroneRacingMinigame>(Outer);
	case EMinigameKind::AptitudeTest: return NewObject<UAptitudeTestMinigame>(Outer);
	case EMinigameKind::MagicDuel: return NewObject<UMagicDuelMinigame>(Outer);
	case EMinigameKind::Pickpocket: return NewObject<UPickpocketMinigame>(Outer);
	case EMinigameKind::SafeCrack: return NewObject<USafeCrackMinigame>(Outer);
	case EMinigameKind::Enchanting: return NewObject<UEnchantingMinigame>(Outer);
	case EMinigameKind::Cooking: return NewObject<UCookingMinigame>(Outer);
	case EMinigameKind::Blacksmithing: return NewObject<UBlacksmithingMinigame>(Outer);
	case EMinigameKind::Blackjack: return NewObject<UBlackjackMinigame>(Outer);
	case EMinigameKind::Roulette: return NewObject<URouletteMinigame>(Outer);
	case EMinigameKind::Slots: return NewObject<USlotsMinigame>(Outer);
	case EMinigameKind::Photography: return NewObject<UChimeraMinigame>(Outer);
	case EMinigameKind::CharacterCreation: return NewObject<UCharacterCreationFlow>(Outer);
	default: return nullptr;
	}
}

// ---------------------------------------------------------------------------
// GDD 13.1 - Breach Protocol.
// ---------------------------------------------------------------------------
void UBreachProtocolMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Breach Protocol");
	Kind = EMinigameKind::BreachProtocol;
	Grid.SetNum(W * H);
	for (int32 i = 0; i < Grid.Num(); ++i)
	{
		Grid[i] = FString::Printf(TEXT("%02X"), FMath::RandRange(0, 255));
	}
	int32 Len = FMath::RandRange(3, 5);
	for (int32 i = 0; i < Len; ++i)
	{
		Sequence.Add(Grid[FMath::RandRange(0, Grid.Num() - 1)]);
	}
	SeqIndex = 0;
	Status = TEXT("Walk the code matrix: move with WASD/arrows, Space to pick the next code.");
}

void UBreachProtocolMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	TimeLeft -= DeltaTime;
	if (TimeLeft <= 0.f)
	{
		Status = TEXT("Upload timed out. The daemon stayed dormant.");
		Finish(false, SeqIndex);
	}
}

void UBreachProtocolMinigame::Draw(UCanvas* Canvas, float SW, float SH)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(20, 20), FVector2D(2, 2), FLinearColor::Green);
	float Cell = 44.f;
	float Ox = (SW - W * Cell) / 2.f, Oy = 120.f;
	for (int32 y = 0; y < H; ++y)
	{
		for (int32 x = 0; x < W; ++x)
		{
			bool Selected = (x == CursorX && y == CursorY);
			Canvas->DrawRect(Selected ? FLinearColor(0.2f, 1.f, 0.2f) : FLinearColor(0.1f, 0.1f, 0.15f),
				Ox + x * Cell, Oy + y * Cell, Cell - 4, Cell - 4);
			Canvas->DrawText(GEngine->GetSmallFont(), Grid[y * W + x], FVector2D(Ox + x * Cell + 8, Oy + y * Cell + 10), FVector2D(1.2f, 1.2f), FLinearColor::White);
		}
	}
	// Required sequence.
	FString Need;
	for (int32 i = SeqIndex; i < Sequence.Num(); ++i) Need += Sequence[i] + (i < Sequence.Num() - 1 ? " -> " : TEXT(""));
	Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("Sequence: %s   Time: %.1f"), *Need, TimeLeft),
		FVector2D(20, 80), FVector2D(1.5f, 1.5f), FLinearColor::Yellow);
	Canvas->DrawText(GEngine->GetSmallFont(), Status, FVector2D(20, SH - 40), FVector2D(1.2f, 1.2f), FLinearColor::White);
}

void UBreachProtocolMinigame::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	if (Key == EKeys::W || Key == EKeys::Up) { CursorY = FMath::Max(0, CursorY - 1); return; }
	if (Key == EKeys::S || Key == EKeys::Down) { CursorY = FMath::Min(H - 1, CursorY + 1); return; }
	if (Key == EKeys::A || Key == EKeys::Left) { CursorX = FMath::Max(0, CursorX - 1); return; }
	if (Key == EKeys::D || Key == EKeys::Right) { CursorX = FMath::Min(W - 1, CursorX + 1); return; }
	if (Key == EKeys::SpaceBar || Key == EKeys::Enter)
	{
		const FString Code = Grid[CursorY * W + CursorX];
		if (Code == Sequence[SeqIndex])
		{
			SeqIndex++;
			Status = FString::Printf(TEXT("Daemon %d of %d uploaded."), SeqIndex, Sequence.Num());
			if (SeqIndex >= Sequence.Num())
			{
				Status = TEXT("Full breach! Daemons chained across the network.");
				Finish(true, SeqIndex);
			}
		}
		else
		{
			Status = TEXT("Bad code. Security notices you. +mistake");
			if (Char) Char->OnMinigameMistake();
			Finish(false, SeqIndex);
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.2 - Surgery.
// ---------------------------------------------------------------------------
void USurgeryMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Surgery");
	Kind = EMinigameKind::Surgery;
	static const TArray<FString> Ops = { TEXT("Appendectomy"), TEXT("Cyberpunk Rejection removal"), TEXT("Dragon Flu bronchoscopy"), TEXT("Magical Overload discharge") };
	OperationName = Ops[FMath::RandRange(0, Ops.Num() - 1)];
	Target = FMath::RandRange(3000, 7000) / 10000.f;
	GreenSize = 0.3f;
	Step = 0;
	Vitals = 100.f;
	Status = FString::Printf(TEXT("%s - %d cuts to go. Keep the marker in the green when it sweeps. Space to cut."), *OperationName, 5);
}

void USurgeryMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	Needle += DeltaTime * 0.8f * NeedleDir;
	if (Needle > 1.f) { Needle = 1.f; NeedleDir = -1.f; }
	if (Needle < 0.f) { Needle = 0.f; NeedleDir = 1.f; }
}

void USurgeryMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(20, 20), FVector2D(2, 2), FLinearColor::Red);
	Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("%s  |  Vitals %.0f%%  |  Cut %d/5"), *OperationName, Vitals, Step + 1),
		FVector2D(20, 60), FVector2D(1.5f, 1.5f), FLinearColor::White);
	// Sweeping needle + green zone (GDD 13.2 - scalpel pressure/timing).
	float BarX = 80, BarY = H * 0.6f, BarW = W - 160, BarH = 40;
	Canvas->DrawRect(FLinearColor(0.15f, 0.15f, 0.15f), BarX, BarY, BarW, BarH);
	Canvas->DrawRect(FLinearColor(0.2f, 0.8f, 0.2f), BarX + Target * BarW, BarY, GreenSize * BarW, BarH);
	Canvas->DrawRect(FLinearColor::Yellow, BarX + Needle * BarW - 3, BarY - 6, 6, BarH + 12);
	Canvas->DrawText(GEngine->GetSmallFont(), Status, FVector2D(20, H - 40), FVector2D(1.2f, 1.2f), FLinearColor::White);
}

void USurgeryMinigame::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	if (Key == EKeys::SpaceBar)
	{
		float Dist = FMath::Abs(Needle - Target);
		if (Dist < GreenSize)
		{
			Vitals = FMath::Min(100.f, Vitals + 15.f);
			Step++;
			Status = FString::Printf(TEXT("Clean cut. Vitals %.0f%%. %d remaining."), Vitals, 5 - Step);
			Target = FMath::FRandRange(0.1f, 0.9f);
			if (Step >= 5)
			{
				Status = TEXT("Patient saved. Great work, doctor.");
				Finish(true, (int32)Vitals);
			}
		}
		else
		{
			Vitals -= 20.f;
			Status = FString::Printf(TEXT("Too deep! Vitals %.0f%%. +mistake"), Vitals);
			if (Char) Char->OnMinigameMistake();
			if (Vitals <= 0.f)
			{
				Status = TEXT("The patient flatlines. The family is notified. -emotional");
				Finish(false, 0);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.4 - Racing hot lap.
// ---------------------------------------------------------------------------
void URacingMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Grand Coliseum - Hot Lap");
	Kind = EMinigameKind::Racing;
	bConsumesMovement = false;
	Countdown = 3.f;
	bGo = false;
	Status = TEXT("3... 2... 1...");
}

void URacingMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	if (!bGo)
	{
		Countdown -= DeltaTime;
		Status = Countdown > 0.f ? FString::Printf(TEXT("%.0f"), FMath::CeilToFloat(Countdown)) : TEXT("GO!");
		if (Countdown <= 0.f) bGo = true;
		return;
	}
	LapTime += DeltaTime;
	Status = FString::Printf(TEXT("Lap time: %.2fs   Target: %.1fs"), LapTime, TargetLap);
	if (FinishLine && Char && FinishLine->IsOverlappingActor(Char))
	{
		bool bWin = LapTime <= TargetLap;
		Status = bWin ? TEXT("Pole position! Fastest lap.") : TEXT("Off the pace. The factory team wants telemetry.");
		Finish(bWin, (int32)(LapTime * 100.f));
	}
}

void URacingMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(20, 20), FVector2D(2, 2), FLinearColor::Cyan);
	Canvas->DrawText(GEngine->GetSmallFont(), Status, FVector2D(20, 60), FVector2D(2, 2), FLinearColor::White);
	Canvas->DrawText(GEngine->GetSmallFont(), TEXT("Drive the marked circuit and cross the finish line. WASD + sprint."),
		FVector2D(20, H - 60), FVector2D(1.2f, 1.2f), FLinearColor::Yellow);
}

// ---------------------------------------------------------------------------
// GDD 13.10 - Lockpicking.
// ---------------------------------------------------------------------------
void ULockpickMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Lockpicking");
	Kind = EMinigameKind::Lockpicking;
	Pin = 0;
	Needle = 0.f;
	Damage = 0;
	SweetSpot = FMath::FRandRange(0.15f, 0.85f);
	SweetSize = 0.2f;
	Status = TEXT("Press Space to set the pin when the needle is in the green.");
}

void ULockpickMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	Needle += DeltaTime * 1.2f * NeedleDir;
	if (Needle > 1.f) { Needle = 1.f; NeedleDir = -1.f; }
	if (Needle < 0.f) { Needle = 0.f; NeedleDir = 1.f; }
}

void ULockpickMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(20, 20), FVector2D(2, 2), FLinearColor::Orange);
	float BarX = 100, BarY = H * 0.55f, BarW = W - 200, BarH = 36;
	Canvas->DrawRect(FLinearColor(0.15f, 0.15f, 0.15f), BarX, BarY, BarW, BarH);
	Canvas->DrawRect(FLinearColor(0.2f, 0.8f, 0.2f), BarX + (SweetSpot - SweetSize / 2) * BarW, BarY, SweetSize * BarW, BarH);
	Canvas->DrawRect(FLinearColor::Orange, BarX + Needle * BarW - 3, BarY - 6, 6, BarH + 12);
	Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("Pin %d/3   Damage %d/2"), Pin + 1, Damage),
		FVector2D(20, 60), FVector2D(1.5f, 1.5f), FLinearColor::White);
	Canvas->DrawText(GEngine->GetSmallFont(), Status, FVector2D(20, H - 40), FVector2D(1.2f, 1.2f), FLinearColor::White);
}

void ULockpickMinigame::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	if (Key == EKeys::SpaceBar)
	{
		float Dist = FMath::Abs(Needle - SweetSpot);
		if (Dist < SweetSize)
		{
			Pin++;
			Status = FString::Printf(TEXT("Pin set. %d/3."), Pin);
			if (Pin >= 3)
			{
				Status = TEXT("The lock clicks open.");
				Finish(true, 3);
			}
			else
			{
				SweetSpot = FMath::FRandRange(0.15f, 0.85f);
			}
		}
		else
		{
			Damage++;
			Status = FString::Printf(TEXT("Scratch! Damage %d/2. +mistake"), Damage);
			if (Char) Char->OnMinigameMistake();
			if (Damage >= 2)
			{
				Status = TEXT("The pick snaps in the lock.");
				Finish(false, 0);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.3 - Interrogation.
// ---------------------------------------------------------------------------
void UInterrogationMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Interrogation");
	Kind = EMinigameKind::Interrogation;
	CorrectEvidence = FMath::RandRange(0, 3);
	Evidence = { TEXT("Empty holster"), TEXT("Dragon scale"), TEXT("Medical chart"), TEXT("Data shard") };
	Cursor = 0;
	Status = TEXT("Suspect: \"I wasn't there. I have an alibi.\" Present evidence or push back.");
}

void UInterrogationMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(20, 20), FVector2D(2, 2), FLinearColor::Purple);
	Canvas->DrawText(GEngine->GetSmallFont(), Status, FVector2D(20, 60), FVector2D(1.4f, 1.4f), FLinearColor::White);
	Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("Attempts left: %d"), Attempts),
		FVector2D(20, 100), FVector2D(1.3f, 1.3f), FLinearColor::Yellow);
	float Y = 160;
	for (int32 i = 0; i < Evidence.Num(); ++i)
	{
		bool Sel = (i == Cursor);
		Canvas->DrawText(GEngine->GetSmallFont(),
			FString::Printf(TEXT("%s %s (E)"), Sel ? TEXT(">") : TEXT(" "), *Evidence[i]),
			FVector2D(60, Y), FVector2D(1.4f, 1.4f), Sel ? FLinearColor::Green : FLinearColor::White);
		Y += 40;
	}
	Canvas->DrawText(GEngine->GetSmallFont(), TEXT("(E) present   (R) bad cop   (F) good cop   (Esc) stall"),
		FVector2D(20, H - 40), FVector2D(1.1f, 1.1f), FLinearColor(0.7f, 0.7f, 0.7f));
}

void UInterrogationMinigame::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	if (Key == EKeys::W || Key == EKeys::Up) { Cursor = FMath::Max(0, Cursor - 1); return; }
	if (Key == EKeys::S || Key == EKeys::Down) { Cursor = FMath::Min(Evidence.Num() - 1, Cursor + 1); return; }
	if (Key == EKeys::E || Key == EKeys::Enter)
	{
		if (Cursor == CorrectEvidence)
		{
			Status = TEXT("They break. Confession on the record. Case closed.");
			Finish(true, 3);
		}
		else
		{
			Attempts--;
			Status = FString::Printf(TEXT("Wrong evidence. They clam up. Attempts left: %d. +mistake"), Attempts);
			if (Char) Char->OnMinigameMistake();
			if (Attempts <= 0) Finish(false, 0);
		}
	}
	else if (Key == EKeys::R)
	{
		Attempts--;
		Status = FString::Printf(TEXT("Bad cop: pressure. They flinch but lawyering up. Attempts left: %d. +mistake"), Attempts);
		if (Char) Char->OnMinigameMistake();
		if (Attempts <= 0) Finish(false, 0);
	}
	else if (Key == EKeys::F)
	{
		Status = TEXT("Good cop: empathy. Rapport up. +Social XP");
		if (Char) Char->AddSkillXPByName(TEXT("Soc_Empathy"), 20);
	}
}

// ---------------------------------------------------------------------------
// GDD 13.9 - Braindance: scrub memory layers to find hidden edits.
// ---------------------------------------------------------------------------
void UBraindanceMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("BRAINDANCE EDITOR");
	Kind = EMinigameKind::Braindance;
	Layers = { TEXT("Visual - city street, neutral"), TEXT("Audio - distant sirens"),
		TEXT("Tactile - leather seat"), TEXT("Visual - a man, blurred"), TEXT("Audio - a name whispered") };
	Trace = TEXT("The watermark is hidden in visual layer 2 of 5.");
	Status = TEXT("Scrub each layer (A/D) and focus (W/S). The edited memory glitches where the trace was cut.");
}

void UBraindanceMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	if (Mismatch > 0.f) Mismatch = FMath::Max(0.f, Mismatch - DeltaTime * 2.f);
}

void UBraindanceMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(40, 20), FVector2D(2.5f, 2.5f), FLinearColor::Cyan);
	float Y = 80;
	for (int32 i = 0; i < Layers.Num(); ++i)
	{
		bool Sel = (i == Cursor);
		FString Line = FString::Printf(TEXT("%s [%s] %s"), Sel ? TEXT(">") : TEXT(" "),
			(i == Focus) ? TEXT("focused") : TEXT("       "), *Layers[i]);
		Canvas->DrawText(GEngine->GetSmallFont(), Line, FVector2D(80, Y), FVector2D(1.3f, 1.3f),
			Sel ? FLinearColor::Green : FLinearColor::White);
		Y += 30;
	}
	Canvas->DrawText(GEngine->GetSmallFont(),
		FString::Printf(TEXT("Scrub depth: %d/4   Mismatch: %.0f%%   Edits decoded: %d/%d"), Focus, Mismatch * 100.f, Decoded, 3),
		FVector2D(80, Y + 20), FVector2D(1.3f, 1.3f), Mismatch > 0.5f ? FLinearColor::Red : FLinearColor::Yellow);
	Canvas->DrawText(GEngine->GetSmallFont(),
		FString::Printf(TEXT("Hint: %s"), *Trace),
		FVector2D(80, Y + 60), FVector2D(1.1f, 1.1f), FLinearColor(0.7f, 0.7f, 0.7f));
	Canvas->DrawText(GEngine->GetSmallFont(), TEXT("A/D layer   W/S scrub   Space decode focused layer   (Esc) exit"),
		FVector2D(40, H - 40), FVector2D(1.2f, 1.2f), FLinearColor(0.7f, 0.7f, 0.7f));
}

void UBraindanceMinigame::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	if (Key == EKeys::A || Key == EKeys::Left) { Cursor = FMath::Max(0, Cursor - 1); return; }
	if (Key == EKeys::D || Key == EKeys::Right) { Cursor = FMath::Min(Layers.Num() - 1, Cursor + 1); return; }
	if (Key == EKeys::W || Key == EKeys::Up) { Focus = FMath::Max(0, Focus - 1); return; }
	if (Key == EKeys::S || Key == EKeys::Down) { Focus = FMath::Min(4, Focus + 1); return; }
	if (Key == EKeys::Space)
	{
		// The hidden edit sits at visual layer index 2, scrub depth 2.
		if (Cursor == 2 && Focus == 2)
		{
			Decoded++;
			Status = FString::Printf(TEXT("Edit decoded (%d/3). The watermark sharpens."), Decoded);
			if (Decoded >= 3) { Finish(true, Decoded * 25); }
		}
		else
		{
			Mismatch = 1.f;
			Status = TEXT("Nothing there. The edit eludes you. +mistake");
			if (Char) Char->OnMinigameMistake();
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.7 - Dance battle: keep the beat grid.
// ---------------------------------------------------------------------------
void UDanceMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("DANCE BATTLE");
	Kind = EMinigameKind::Dance;
	MoveIndex = 0;
	Cursor = 0;
	Beat = 0.f;
	Combo = 0;
	BestCombo = 0;
	Targets = 4;
	Status = TEXT("Match the moves on the beat. Press the arrow for the move shown. Don't miss the beat.");
}

void UDanceMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	Beat += DeltaTime;
	if (Beat >= BeatInterval)
	{
		Beat = 0.f;
		// A missed move breaks the combo.
		if (Combo > 0)
		{
			Status = FString::Printf(TEXT("Missed the beat. Combo reset. Best: %d"), BestCombo);
			Combo = 0;
			if (Char) Char->OnMinigameMistake();
		}
	}
}

void UDanceMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(40, 20), FVector2D(2.5f, 2.5f), FLinearColor::Cyan);
	Canvas->DrawText(GEngine->GetSmallFont(),
		FString::Printf(TEXT("Next move:  %s"), *Moves[MoveIndex]),
		FVector2D(80, 90), FVector2D(2.0f, 2.0f), FLinearColor::Yellow);
	Canvas->DrawText(GEngine->GetSmallFont(),
		FString::Printf(TEXT("Beat: %.2f / %.2f   Combo: %d   Best: %d   Moves done: %d/%d"),
			Beat, BeatInterval, Combo, BestCombo, MoveIndex, Targets),
		FVector2D(80, 140), FVector2D(1.3f, 1.3f), FLinearColor::White);
	Canvas->DrawText(GEngine->GetSmallFont(), TEXT("Arrow keys on the beat   (Esc) exit"),
		FVector2D(40, H - 40), FVector2D(1.2f, 1.2f), FLinearColor(0.7f, 0.7f, 0.7f));
}

void UDanceMinigame::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	FString Move = TEXT("NONE");
	if (Key == EKeys::Left) Move = TEXT("LEFT");
	else if (Key == EKeys::Right) Move = TEXT("RIGHT");
	else if (Key == EKeys::Up) Move = TEXT("UP");
	else if (Key == EKeys::Down) Move = TEXT("DOWN");
	if (Move == TEXT("NONE")) return;
	// Only register on the beat window.
	if (Beat < BeatInterval * 0.25f || Beat > BeatInterval * 0.85f)
	{
		Status = TEXT("Off-beat. +mistake");
		Combo = 0;
		if (Char) Char->OnMinigameMistake();
		return;
	}
	if (Move == Moves[MoveIndex])
	{
		Combo++;
		BestCombo = FMath::Max(BestCombo, Combo);
		MoveIndex++;
		Beat = 0.f;
		Status = FString::Printf(TEXT("Clean! Combo %d"), Combo);
		if (MoveIndex >= Targets) { Finish(true, BestCombo * 10); }
	}
	else
	{
		Combo = 0;
		Status = TEXT("Wrong move. +mistake");
		if (Char) Char->OnMinigameMistake();
	}
}

// ---------------------------------------------------------------------------
// GDD 13.8 - Football shootout: aim, charge, keeper dives late.
// ---------------------------------------------------------------------------
void UFootballShootoutMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("PENALTY SHOOTOUT");
	Kind = EMinigameKind::FootballShootout;
	Aim = 0;
	Keeper = 0;
	Power = 0.f;
	bCharging = false;
	Kicks = 0;
	Scored = 0;
	TotalKicks = 3;
	Status = TEXT("Aim with A/D, hold E to charge power, press Space to shoot. The keeper picks a corner at the last second.");
}

void UFootballShootoutMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	if (bCharging)
	{
		Power = FMath::Min(1.f, Power + DeltaTime);
	}
}

void UFootballShootoutMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	Canvas->DrawText(GEngine->GetSmallFont(), Title, FVector2D(40, 20), FVector2D(2.5f, 2.5f), FLinearColor::Cyan);
	Canvas->DrawText(GEngine->GetSmallFont(),
		FString::Printf(TEXT("Aim: %s   Keeper reads: %s"), *Corners[Aim], *Corners[Keeper]),
		FVector2D(80, 90), FVector2D(1.5f, 1.5f), FLinearColor::White);
	Canvas->DrawText(GEngine->GetSmallFont(),
		FString::Printf(TEXT("Power: %d%%   Kicks: %d/%d   Scored: %d"),
			(int32)(Power * 100.f), Kicks + 1, TotalKicks, Scored),
		FVector2D(80, 130), FVector2D(1.3f, 1.3f), FLinearColor::Yellow);
	Canvas->DrawText(GEngine->GetSmallFont(), TEXT("A/D aim   E charge   Space shoot   (Esc) exit"),
		FVector2D(40, H - 40), FVector2D(1.2f, 1.2f), FLinearColor(0.7f, 0.7f, 0.7f));
}

void UFootballShootoutMinigame::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	if (Key == EKeys::A || Key == EKeys::Left) { Aim = (Aim + 3) % 4; return; }
	if (Key == EKeys::D || Key == EKeys::Right) { Aim = (Aim + 1) % 4; return; }
	if (Key == EKeys::E)
	{
		bCharging = true;
		Power = 0.f;
		return;
	}
	if (Key == EKeys::Space)
	{
		// Release: keeper picks a corner (biased to read your aim unless undercharged).
		Keeper = FMath::RandRange(0, 3);
		if (Power < 0.35f)
		{
			// Weak shot: keeper saves unless wrong corner.
			if (Keeper == Aim) { Status = TEXT("Too weak. The keeper collects it."); }
			else { Status = TEXT("Underhit but placed. Goal!"); Scored++; }
		}
		else
		{
			// Full power: keeper guesses, aim wins unless both wrong-ish.
			if (Keeper == Aim) { Status = TEXT("The keeper reads you. Saved!"); }
			else { Status = TEXT("Top corner. The net bulges. GOAL!"); Scored++; }
		}
		Kicks++;
		Power = 0.f;
		bCharging = false;
		if (Kicks >= TotalKicks)
		{
			int32 Score = Scored * 15;
			Finish(Scored >= 2, Score);
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.10 - Fishing minigame.
// ---------------------------------------------------------------------------
void UFishingMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Fishing");
	Kind = EMinigameKind::Fishing;
	Phase = 0;
	CastPower = 0.f;
	WaitTimer = 4.f + FMath::FRandRange(1.f, 6.f);
	Tension = 0.f;
	Progress = 0.f;
	Status = TEXT("Hold Space to build power, release to cast.");
}

void UFishingMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	if (Phase == 0 && bCharging)
	{
		CastPower = FMath::Min(CastPower + DeltaTime * 0.7f, 1.f);
	}
	else if (Phase == 1)
	{
		WaitTimer -= DeltaTime;
		if (WaitTimer <= 0.f)
		{
			Phase = 2;
			HookTimer = 0.8f;
			Status = TEXT("BITE! Press Space now to hook the fish!");
		}
	}
	else if (Phase == 2)
	{
		HookTimer -= DeltaTime;
		if (HookTimer <= 0.f)
		{
			Status = TEXT("The fish got away. Too slow.");
			Finish(false, 0);
		}
	}
	else if (Phase == 3)
	{
		if (bReeling)
		{
			Tension += DeltaTime * 55.f;
			Progress += DeltaTime * 18.f;
		}
		else
		{
			Tension -= DeltaTime * 35.f;
		}
		Tension = FMath::Clamp(Tension, 0.f, 100.f);
		Progress = FMath::Clamp(Progress, 0.f, 100.f);
		if (Tension >= 100.f)
		{
			Status = TEXT("Line snapped! The fish escapes.");
			Finish(false, 0);
			return;
		}
		if (Progress >= 100.f)
		{
			int32 Score = 50 + FMath::RandRange(0, 50);
			Status = FString::Printf(TEXT("Caught! +%d points."), Score);
			Finish(true, Score);
		}
	}
}

void UFishingMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	const float LH = 22.f;
	float Y = H * 0.3f;
	// Title
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 60, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += LH * 2;
	if (Phase == 0)
	{
		Canvas->DrawText(F, TEXT("Cast Phase"), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::White);
		Canvas->DrawText(F, FString::Printf(TEXT("Power: %d%%"), FMath::RoundToInt(CastPower * 100.f)), FVector2D(40, Y + LH), FVector2D(1.1f, 1.1f), FLinearColor::Green);
	}
	else if (Phase == 1)
	{
		Canvas->DrawText(F, FString::Printf(TEXT("Waiting... %.1fs"), WaitTimer), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::White);
	}
	else if (Phase == 2)
	{
		Canvas->DrawText(F, FString::Printf(TEXT("HOOK! React! (%.1fs)"), HookTimer), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Red);
	}
	else if (Phase == 3)
	{
		Canvas->DrawText(F, FString::Printf(TEXT("Reel! Tension: %.0f%%   Progress: %.0f%%"), Tension, Progress), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::Yellow);
		// Tension bar
		float bx = 40, by = Y + LH, bw = 400, bh = 20;
		Canvas->DrawRect(FLinearColor(0.2f, 0.2f, 0.2f), bx, by, bw, bh);
		Canvas->DrawRect(FLinearColor(Tension > 80 ? 1.f : 0.8f, Tension > 80 ? 0.3f : 0.3f, 0.2f), bx, by, bw * Tension / 100.f, bh);
		// Progress bar
		Canvas->DrawRect(FLinearColor(0.2f, 0.2f, 0.2f), bx, by + 30, bw, bh);
		Canvas->DrawRect(FLinearColor(0.2f, 0.8f, 0.4f), bx, by + 30, bw * Progress / 100.f, bh);
	}
	Canvas->DrawText(F, Status, FVector2D(40, H * 0.6f), FVector2D(1.0f, 1.0f), FLinearColor(0.9f, 0.9f, 0.7f));
}

void UFishingMinigame::HandleKey(const FKey& Key)
{
	if (Key != EKeys::SpaceBar) return;
	if (Phase == 0)
	{
		if (bCharging)
		{
			// Release cast: power built -> launch.
			bCharging = false;
			Status = FString::Printf(TEXT("Cast at %d%% power. Waiting for a bite..."), FMath::RoundToInt(CastPower * 100.f));
			Phase = 1;
		}
		else
		{
			bCharging = true;
			Status = TEXT("Building power... Space again to cast.");
		}
	}
	else if (Phase == 2)
	{
		bReeling = true;
		Phase = 3;
		Status = TEXT("Hooked! Space to reel, Space again to ease tension.");
	}
	else if (Phase == 3)
	{
		bReeling = !bReeling;
	}
}

// ---------------------------------------------------------------------------
// GDD 13.9 - Poker (Texas Hold'em, 1 AI opponent).
// ---------------------------------------------------------------------------
static int32 CardValue(int32 C) { return C % 13; }
static int32 CardSuit(int32 C) { return C / 13; }
static FString CardStr(int32 C)
{
	static const TCHAR* Suits[] = { TEXT("\u2660"), TEXT("\u2661"), TEXT("\u2662"), TEXT("\u2663") };
	static const TCHAR* Ranks[] = { TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8"),
		TEXT("9"), TEXT("10"), TEXT("J"), TEXT("Q"), TEXT("K"), TEXT("A") };
	return FString::Printf(TEXT("%s%s"), Ranks[CardValue(C)], Suits[CardSuit(C)]);
}

static int32 HandRank(int32 C1, int32 C2, const TArray<int32>& Comm)
{
	// Simplified: highest card + bonus for pairs. Full hand eval is too much for prototype.
	int32 Hi = FMath::Max(CardValue(C1), CardValue(C2));
	int32 Pair = (CardValue(C1) == CardValue(C2)) ? 20 : 0;
	for (int32 c : Comm) { Hi = FMath::Max(Hi, CardValue(c)); if (CardValue(c) == CardValue(C1) || CardValue(c) == CardValue(C2)) Pair += 15; }
	return Hi + Pair;
}

void UPokerMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Texas Hold'em Poker");
	Kind = EMinigameKind::Poker;
	PlayerChips = 500;
	AIChips = 500;
	Deal();
}

void UPokerMinigame::Deal()
{
	Deck.SetNum(52);
	for (int32 i = 0; i < 52; ++i) Deck[i] = i;
	for (int32 i = 51; i > 0; --i) { int32 j = FMath::RandRange(0, i); Deck.Swap(i, j); }
	PlayerCard1 = Deck[0]; PlayerCard2 = Deck[1];
	AICard1 = Deck[2];    AICard2 = Deck[3];
	Community.Reset();
	Pot = 0;
	PlayerBet = 0;
	AIBet = 0;
	Phase = 0;
	Cursor = 0;
	bPlayerFolded = false;
	ResultMsg.Empty();
	Status = TEXT("Pre-flop dealt. Bet (up/down WS, Space confirm), or Fold (F).");
}

void UPokerMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	float Y = H * 0.25f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 60, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += 30;
	// Community cards
	FString Comm;
	for (int32 c : Community) Comm += CardStr(c) + TEXT("  ");
	Canvas->DrawText(F, FString::Printf(TEXT("Community: %s"), *Comm), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::White); Y += 25;
	// Player cards
	Canvas->DrawText(F, FString::Printf(TEXT("Your hand: %s  %s"), *CardStr(PlayerCard1), *CardStr(PlayerCard2)),
		FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Green); Y += 25;
	// AI cards (hidden unless showdown)
	Canvas->DrawText(F, Phase >= 4 ? FString::Printf(TEXT("AI hand: %s  %s"), *CardStr(AICard1), *CardStr(AICard2)) : TEXT("AI hand: ??  ??"),
		FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Red); Y += 25;
	// Chips & pot
	Canvas->DrawText(F, FString::Printf(TEXT("Pot: %d   You: %d chips   AI: %d chips"), Pot, PlayerChips, AIChips),
		FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::Yellow); Y += 30;

	if (Phase < 4 && !bPlayerFolded)
	{
		const FString Options[] = { TEXT("Bet 50"), TEXT("Fold"), TEXT("Call/Check") };
		for (int32 i = 0; i < 3; ++i)
		{
			Canvas->DrawText(F, FString::Printf(TEXT("%c %s"), Cursor == i ? TEXT('>') : TEXT(' '), *Options[i]),
				FVector2D(40, Y), FVector2D(1.1f, 1.1f), Cursor == i ? FLinearColor::Green : FLinearColor::White);
			Y += 22;
		}
	}
	if (!ResultMsg.IsEmpty())
	{
		Canvas->DrawText(F, ResultMsg, FVector2D(40, H * 0.7f), FVector2D(1.3f, 1.3f), FLinearColor(1.f, 0.85f, 0.3f));
	}
}

void UPokerMinigame::HandleKey(const FKey& Key)
{
	if (Phase >= 4 || bPlayerFolded)
	{
		if (Key == EKeys::SpaceBar) { Deal(); Phase = 0; }
		else if (Key == EKeys::Escape) Finish(true, 20);
		return;
	}
	if (Key == EKeys::W || Key == EKeys::Up) { Cursor = (Cursor + 2) % 3; }
	else if (Key == EKeys::S || Key == EKeys::Down) { Cursor = (Cursor + 1) % 3; }
	else if (Key == EKeys::SpaceBar || Key == EKeys::Enter)
	{
		// AI plays simultaneously.
		int32 AIAction = FMath::RandRange(0, 2);
		bool bAIFold = false;

		if (Cursor == 0) // bet
		{
			int32 Amt = 50;
			PlayerBet = Amt;
			PlayerChips -= Amt;
			Pot += Amt;
			if (AIAction == 1 || AIChips < Amt) { bAIFold = true; }
			else { AIBet = Amt; AIChips -= Amt; Pot += Amt; }
		}
		else if (Cursor == 1) // fold
		{
			bPlayerFolded = true;
			ResultMsg = TEXT("You folded. AI wins the pot.");
			Phase = 4;
			return;
		}
		else // call/check
		{
			PlayerChips -= PlayerBet;
			Pot += PlayerBet;
			AIChips -= AIBet;
			Pot += AIBet;
			if (AIAction == 1) bAIFold = true;
		}

		if (bAIFold)
		{
			ResultMsg = TEXT("AI folds! You win the pot.");
			Phase = 4;
			return;
		}

		Phase++;
		if (Phase == 1) { Community.Add(Deck[4]); Community.Add(Deck[5]); Community.Add(Deck[6]); Status = TEXT("Flop dealt."); }
		else if (Phase == 2) { Community.Add(Deck[7]); Status = TEXT("Turn dealt."); }
		else if (Phase == 3) { Community.Add(Deck[8]); Status = TEXT("River dealt."); }
		else if (Phase >= 4)
		{
			int32 PlayerHand = HandRank(PlayerCard1, PlayerCard2, Community);
			int32 AIHand = HandRank(AICard1, AICard2, Community);
			if (PlayerHand > AIHand)
			{
				PlayerChips += Pot;
				ResultMsg = FString::Printf(TEXT("You win %d chips!"), Pot);
				Finish(true, 30);
			}
			else if (AIHand > PlayerHand)
			{
				AIChips += Pot;
				ResultMsg = TEXT("AI wins. Better luck next hand.");
				Finish(false, 10);
			}
			else
			{
				PlayerChips += Pot / 2;
				AIChips += Pot / 2;
				ResultMsg = TEXT("Split pot.");
				Finish(true, 15);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.10 - Drone Racing (FPV checkpoint gauntlet).
// ---------------------------------------------------------------------------
void UDroneRacingMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Drone Racing");
	Kind = EMinigameKind::DroneRacing;
	// Generate a track of 8 gates.
	Gates.Reset();
	float Angle = 0.f, R = 300.f;
	for (int32 i = 0; i < 8; ++i)
	{
		Angle = i * 45.f * 3.14159f / 180.f;
		FGate G;
		G.X = 400 + FMath::Cos(Angle) * R;
		G.Y = 300 + FMath::Sin(Angle) * R;
		Gates.Add(G);
	}
	GateIdx = 0;
	DroneX = 400;
	DroneY = 300;
	SpeedX = SpeedY = 0.f;
	Boost = 1.f;
	BoostCooldown = 0.f;
	Timer = 0.f;
	Status = TEXT("Navigate through the gates. WASD to fly, Space to boost.");
}

void UDroneRacingMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	Timer += DeltaTime;
	if (BoostCooldown > 0.f) { BoostCooldown -= DeltaTime; if (BoostCooldown <= 0.f) Boost = 1.f; }
	// Friction
	SpeedX *= 0.95f;
	SpeedY *= 0.95f;
	DroneX += SpeedX * DeltaTime * 200.f * Boost;
	DroneY += SpeedY * DeltaTime * 200.f * Boost;
	DroneX = FMath::Clamp(DroneX, 20.f, 780.f);
	DroneY = FMath::Clamp(DroneY, 20.f, 580.f);
	// Gate check
	if (GateIdx < Gates.Num())
	{
		FGate& G = Gates[GateIdx];
		float Dist = FMath::Sqrt(FMath::Square(DroneX - G.X) + FMath::Square(DroneY - G.Y));
		if (Dist < 30.f)
		{
			GateIdx++;
			Status = FString::Printf(TEXT("Gate %d/%d cleared!"), GateIdx, Gates.Num());
			if (GateIdx >= Gates.Num())
			{
				if (Timer < BestTime) BestTime = Timer;
				int32 Score = FMath::Max(10, 60 - FMath::RoundToInt(Timer));
				Status = FString::Printf(TEXT("Track complete! Time: %.1fs. Best: %.1fs. Score: %d"),
					Timer, BestTime, Score);
				Finish(true, Score);
			}
		}
	}
}

void UDroneRacingMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, *Title, FVector2D(40, H * 0.35f), FVector2D(1.5f, 1.5f), FLinearColor::Cyan);
	Canvas->DrawText(F, FString::Printf(TEXT("Time: %.1fs"), Timer), FVector2D(40, H * 0.35f + 25), FVector2D(1.2f, 1.2f), FLinearColor::White);
	// Draw mini-map style 800x600 track
	float MX = W / 2 - 400, MY = H * 0.42f, MW = 800, MH = 600;
	Canvas->DrawRect(FLinearColor(0.05f, 0.05f, 0.1f), MX, MY, MW, MH);
	// Draw gates
	for (int32 i = 0; i < Gates.Num(); ++i)
	{
		FLinearColor C = (i == GateIdx) ? FLinearColor::Green : FLinearColor(0.4f, 0.4f, 0.4f);
		Canvas->DrawRect(C, MX + Gates[i].X - 12, MY + Gates[i].Y - 12, 24, 24);
		Canvas->DrawText(F, FString::Printf(TEXT("%d"), i + 1),
			FVector2D(MX + Gates[i].X - 5, MY + Gates[i].Y - 20), FVector2D(0.8f, 0.8f), FLinearColor::White);
	}
	// Draw drone
	Canvas->DrawRect(FLinearColor(0.2f, 1.f, 0.8f), MX + DroneX - 8, MY + DroneY - 8, 16, 16);
	Canvas->DrawText(F, Status, FVector2D(40, MY + MH + 10), FVector2D(1.1f, 1.1f), FLinearColor(0.9f, 0.9f, 0.7f));
	Canvas->DrawText(F, TEXT("WASD: fly   Space: boost"), FVector2D(40, MY + MH + 32), FVector2D(0.9f, 0.9f), FLinearColor(0.6f, 0.6f, 0.6f));
}

void UDroneRacingMinigame::HandleKey(const FKey& Key)
{
	if (Key == EKeys::W) { SpeedY -= 50.f; }
	else if (Key == EKeys::S) { SpeedY += 50.f; }
	else if (Key == EKeys::A) { SpeedX -= 50.f; }
	else if (Key == EKeys::D) { SpeedX += 50.f; }
	else if (Key == EKeys::SpaceBar && BoostCooldown <= 0.f) { Boost = 3.f; BoostCooldown = 0.5f; }
}

// ---------------------------------------------------------------------------
// GDD 4.x — Divergent aptitude test (5-question faction personality test).
// ---------------------------------------------------------------------------
void UAptitudeTestMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Aptitude Test — Choose Your Faction");
	Kind = EMinigameKind::AptitudeTest;
	FactionVotes.SetNum(5); for (int32 i = 0; i < 5; ++i) FactionVotes[i] = 0;
	QIdx = 0; Cursor = 0; bDone = false;
	Questions = {
		{ TEXT("A stranger drops their wallet. Do you:"), { TEXT("Return it without a word (Abnegation)"), TEXT("Return it and explain why honesty matters (Candor)"), TEXT("Check the ID first — assess who they are (Erudite)"), TEXT("Pocket it — survival of the fittest (Dauntless)"), TEXT("Buy them a meal with it — kindness above all (Amity)") },
			{ 2, 0, 0, 0, 0 }, { 0, 0, 2, 0, 0 }, { 0, 0, 0, 0, 2 }, { 0, 2, 0, 0, 0 }, { 0, 0, 0, 0, 2 } },
		{ TEXT("You're in a burning building. Do you:"), { TEXT("Run toward the flames to save others (Dauntless)"), TEXT("Calmly direct people to the exits (Erudite)"), TEXT("Hold someone's hand and guide them out gently (Amity)"), TEXT("Tell the truth — the fire started because you left a candle (Candor)"), TEXT("Serve yourself last — make sure everyone else is out (Abnegation)") },
			{ 2, 0, 0, 0, 0 }, { 0, 0, 2, 0, 0 }, { 0, 0, 0, 2, 0 }, { 0, 0, 0, 0, 2 }, { 1, 0, 0, 0, 0 } },
		{ TEXT("A friend confesses a crime. Do you:"), { TEXT("Turn them in — the law matters more than friendship (Candor)"), TEXT("Help them hide — loyalty to your faction above all (Dauntless)"), TEXT("Study the law to find a loophole (Erudite)"), TEXT("Forgive them and never speak of it (Amity)"), TEXT("Take the blame yourself — sacrifice is strength (Abnegation)") },
			{ 0, 2, 0, 0, 0 }, { 2, 0, 0, 0, 0 }, { 0, 0, 2, 0, 0 }, { 0, 0, 0, 2, 0 }, { 1, 0, 0, 0, 0 } },
		{ TEXT("You're assigned a group project. Do you:"), { TEXT("Lead with logic and a detailed plan (Erudite)"), TEXT("Let everyone speak and ensure harmony (Amity)"), TEXT("Just take charge — someone has to (Dauntless)"), TEXT("Do the most work without being asked (Abnegation)"), TEXT("Demand clear roles and honest critiques (Candor)") },
			{ 0, 0, 2, 0, 0 }, { 0, 0, 0, 2, 0 }, { 0, 2, 0, 0, 0 }, { 1, 0, 0, 0, 0 }, { 0, 2, 0, 0, 0 } },
		{ TEXT("What do you fear most?"), { TEXT("Being forgotten — I need to matter (Dauntless)"), TEXT("Being wrong — I need to understand (Erudite)"), TEXT("Betrayal — I need honesty (Candor)"), TEXT("Conflict — I need peace (Amity)"), TEXT("Being selfish — I need to serve (Abnegation)") },
			{ 2, 0, 0, 0, 0 }, { 0, 0, 2, 0, 0 }, { 0, 2, 0, 0, 0 }, { 0, 0, 0, 2, 0 }, { 1, 0, 0, 0, 0 } },
	};
	Status = TEXT("Use W/S to choose, Space to confirm. Your faction awaits.");
}

void UAptitudeTestMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	float Y = H * 0.25f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 100, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += 40;
	if (bDone)
	{
		Canvas->DrawText(F, FString::Printf(TEXT("Your faction: %s"), *ResultFaction), FVector2D(40, Y), FVector2D(1.8f, 1.8f), FLinearColor::Green); Y += 50;
		Canvas->DrawText(F, TEXT("This aptitude result is permanent. Your faction colours are set."), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::White);
		Canvas->DrawText(F, TEXT("Space to confirm and leave."), FVector2D(40, Y + 30), FVector2D(1.0f, 1.0f), FLinearColor(0.6f, 0.6f, 0.6f));
		return;
	}
	if (QIdx < Questions.Num())
	{
		auto& Q = Questions[QIdx];
		Canvas->DrawText(F, FString::Printf(TEXT("Q%d: %s"), QIdx + 1, *Q.Text), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::White); Y += 30;
		for (int32 i = 0; i < Q.Options.Num(); ++i)
		{
			Canvas->DrawText(F, FString::Printf(TEXT("%c %s"), Cursor == i ? TEXT('>') : TEXT(' '), *Q.Options[i]),
				FVector2D(40, Y), FVector2D(1.0f, 1.0f), Cursor == i ? FLinearColor::Green : FLinearColor::White);
			Y += 22;
		}
		Canvas->DrawText(F, Status, FVector2D(40, H * 0.6f), FVector2D(1.0f, 1.0f), FLinearColor(0.9f, 0.9f, 0.7f));
	}
}

void UAptitudeTestMinigame::HandleKey(const FKey& Key)
{
	if (bDone) { if (Key == EKeys::SpaceBar) Finish(true, 25); return; }
	if (Key == EKeys::W || Key == EKeys::Up) Cursor = (Cursor + 4) % 5;
	else if (Key == EKeys::S || Key == EKeys::Down) Cursor = (Cursor + 1) % 5;
	else if (Key == EKeys::SpaceBar || Key == EKeys::Enter)
	{
		auto& Q = Questions[QIdx];
		for (int32 i = 0; i < 5; ++i) FactionVotes[i] += (Q.Scores[Cursor * 5 + i] > 0) ? Q.Scores[Cursor * 5 + i] : 0;
		QIdx++;
		if (QIdx >= Questions.Num())
		{
			int32 Best = 0, BestI = 0;
			for (int32 i = 0; i < 5; ++i) if (FactionVotes[i] > Best) { Best = FactionVotes[i]; BestI = i; }
			FString Names[5] = { TEXT("Abnegation"), TEXT("Amity"), TEXT("Candor"), TEXT("Dauntless"), TEXT("Erudite") };
			ResultFaction = Names[BestI]; bDone = true;
			Status = TEXT("Test complete. Your faction result is shown above.");
			// If tied with second-highest at same level → DIVERGENT.
			for (int32 i = 0; i < 5; ++i) if (i != BestI && FactionVotes[i] == Best) { ResultFaction += TEXT(" – DIVERGENT"); break; }
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.6 - Magical Dueling minigame.
// ---------------------------------------------------------------------------
void UMagicDuelMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Magical Duel");
	Kind = EMinigameKind::MagicDuel;
	PlayerHP = 100; EnemyHP = 100; PlayerMana = 60; EnemyMana = 60;
	Cursor = 0; EnemyChoice = 2;
	Spells = {
		{ TEXT("Fireball"), 25, 15, TEXT("Attack"), TEXT("A roaring sphere of flame erupts from your palm.") },
		{ TEXT("Ice Shield"), 0, 15, TEXT("Shield"), TEXT("A crystalline wall of ice absorbs the incoming spell.") },
		{ TEXT("Lightning Bolt"), 30, 20, TEXT("Disrupt"), TEXT("A crackling arc of electricity interrupts the casting.") },
		{ TEXT("Heal"), -20, 20, TEXT("Shield"), TEXT("Warm golden light knits flesh and restores spirit.") },
		{ TEXT("Shadow Curse"), 20, 10, TEXT("Disrupt"), TEXT("Tendrils of darkness sap the opponent's concentration.") },
		{ TEXT("Arcane Missile"), 15, 10, TEXT("Attack"), TEXT("Three bolts of purple energy streak toward your foe.") }
	};
	Status = TEXT("W/S choose spell, Space to cast. Attack > Disrupt > Shield > Attack.");
}

void UMagicDuelMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	float Y = H * 0.2f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 60, Y), FVector2D(1.8f, 1.8f), FLinearColor::Cyan); Y += 35;
	// HP bars
	Canvas->DrawText(F, FString::Printf(TEXT("YOU: %d HP  |  %d Mana"), PlayerHP, PlayerMana), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Green); Y += 25;
	Canvas->DrawText(F, FString::Printf(TEXT("FOE: %d HP  |  %d Mana"), EnemyHP, EnemyMana), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Red); Y += 30;
	// Spell menu
	for (int32 i = 0; i < Spells.Num(); ++i)
	{
		FString Disabled = (PlayerMana < Spells[i].Mana) ? TEXT(" (no mana)") : TEXT("");
		Canvas->DrawText(F, FString::Printf(TEXT("%s %s [%s] - %d dmg, %d mana%s"),
			Cursor == i ? TEXT(">") : TEXT(" "), *Spells[i].Name, *Spells[i].Type, Spells[i].Dmg, Spells[i].Mana, *Disabled),
			FVector2D(40, Y), FVector2D(1.0f, 1.0f), Cursor == i ? FLinearColor::Green : FLinearColor::White);
		Y += 22;
	}
	// Result
	if (!ResultMsg.IsEmpty())
		Canvas->DrawText(F, *ResultMsg, FVector2D(40, H * 0.55f), FVector2D(1.1f, 1.1f), FLinearColor(1.f, 0.85f, 0.3f));
	Canvas->DrawText(F, *Status, FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void UMagicDuelMinigame::HandleKey(const FKey& Key)
{
	if (PlayerHP <= 0 || EnemyHP <= 0) return;
	if (Key == EKeys::W || Key == EKeys::Up) Cursor = (Cursor + Spells.Num() - 1) % Spells.Num();
	else if (Key == EKeys::S || Key == EKeys::Down) Cursor = (Cursor + 1) % Spells.Num();
	else if (Key == EKeys::SpaceBar || Key == EKeys::Enter)
	{
		FSpell& PS = Spells[Cursor];
		if (PlayerMana < PS.Mana) { Status = TEXT("Not enough mana!"); return; }
		PlayerMana -= PS.Mana;
		// Enemy AI: picks randomly with slight bias against player type
		int32 EPick = FMath::RandRange(0, Spells.Num() - 1);
		if (FMath::RandRange(0, 10) >= 5) EPick = (Cursor + 1) % Spells.Num(); // try to counter
		if (EnemyMana < Spells[EPick].Mana) { for (int32 i = 0; i < Spells.Num(); ++i) if (EnemyMana >= Spells[i].Mana) { EPick = i; break; } }
		FSpell& ES = Spells[EPick]; EnemyMana -= ES.Mana;
		PlayerAction = PS.Name; EnemyAction = ES.Name;
		// Counter mechanic: Attack beats Disrupt, Disrupt beats Shield, Shield beats Attack
		// Player damage to enemy
		if (PS.Dmg > 0 && PS.Type == TEXT("Attack")) { if (ES.Type == TEXT("Disrupt")) PS.Dmg = 0; EnemyHP -= PS.Dmg; }
		if (PS.Dmg > 0 && PS.Type == TEXT("Disrupt")) { if (ES.Type == TEXT("Shield")) PS.Dmg = 0; EnemyHP -= PS.Dmg; }
		if (PS.Dmg < 0) PlayerHP -= PS.Dmg; // heal (negative damage)
		// Enemy damage to player
		if (ES.Dmg > 0 && ES.Type == TEXT("Attack")) { if (PS.Type == TEXT("Disrupt")) ES.Dmg = 0; PlayerHP -= ES.Dmg; }
		if (ES.Dmg > 0 && ES.Type == TEXT("Disrupt")) { if (PS.Type == TEXT("Shield")) ES.Dmg = 0; PlayerHP -= ES.Dmg; }
		if (ES.Dmg < 0) EnemyHP -= ES.Dmg; // enemy heal
		ResultMsg = FString::Printf(TEXT("You cast %s. Foe casts %s."), *PS.Name, *ES.Name);
		EnemyHP = FMath::Clamp(EnemyHP, 0, 100); PlayerHP = FMath::Clamp(PlayerHP, 0, 100);
		// Regain mana
		PlayerMana = FMath::Min(60, PlayerMana + 8); EnemyMana = FMath::Min(60, EnemyMana + 8);
		if (PlayerHP <= 0) { Status = TEXT("DEFEATED. The duel is lost."); Finish(false, 10); }
		else if (EnemyHP <= 0) { Status = TEXT("VICTORY! You have mastered the arcane."); Finish(true, 40); }
	}
}

// ---------------------------------------------------------------------------
// GDD 13.10 — Pickpocketing minigame.
// ---------------------------------------------------------------------------
void UPickpocketMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Pickpocketing");
	Kind = EMinigameKind::Pickpocket;
	SafeZone = 55.f; ZonePhase = 0.f; Timer = 0.f; Attempts = 3; bGrabbing = false;
	Status = TEXT("Approach the target from behind. Space to grab. Shrinking safe zone — time it right.");
}

void UPickpocketMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	Timer += DeltaTime;
	SafeZone = 55.f - Timer * 3.f;
	SafeZone = FMath::Max(SafeZone, 8.f);
	ZonePhase += DeltaTime * 2.5f;
}

void UPickpocketMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 60, H * 0.3f), FVector2D(1.5f, 1.5f), FLinearColor::Cyan);
	// Draw circular indicator
	float CX = W / 2, CY = H * 0.5f, R = 140.f;
	for (int32 i = 0; i < 360; i += 5)
	{
		float rad = i * 3.14159f / 180.f;
		float x1 = CX + FMath::Cos(rad) * R, y1 = CY + FMath::Sin(rad) * R;
		float x2 = CX + FMath::Cos(rad) * (R - 3), y2 = CY + FMath::Sin(rad) * (R - 3);
		Canvas->DrawLine(x1, y1, x2, y2, FLinearColor(0.3f, 0.3f, 0.3f));
	}
	// Draw safe zone
	float zoneRad = R * 0.7f;
	float zoneX = CX + FMath::Cos(ZonePhase) * zoneRad * 0.3f;
	float zoneY = CY + FMath::Sin(ZonePhase) * zoneRad * 0.3f;
	for (int32 i = 0; i < 360; i += 3)
	{
		float rad = i * 3.14159f / 180.f;
		float x1 = zoneX + FMath::Cos(rad) * SafeZone, y1 = zoneY + FMath::Sin(rad) * SafeZone;
		float x2 = zoneX + FMath::Cos(rad) * (SafeZone - 2), y2 = zoneY + FMath::Sin(rad) * (SafeZone - 2);
		Canvas->DrawLine(x1, y1, x2, y2, SafeZone > 20 ? FLinearColor::Green : FLinearColor::Red);
	}
	Canvas->DrawText(F, FString::Printf(TEXT("Attempts: %d"), Attempts), FVector2D(40, H * 0.65f), FVector2D(1.1f, 1.1f), FLinearColor::White);
	Canvas->DrawText(F, *Status, FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void UPickpocketMinigame::HandleKey(const FKey& Key)
{
	if (Key == EKeys::SpaceBar && !bGrabbing)
	{
		bGrabbing = true;
		bool bSuccess = SafeZone > 20.f;
		if (bSuccess)
		{
			int32 Score = FMath::RoundToInt(SafeZone * 3);
			Finish(true, Score);
			Status = TEXT("Snatched! The mark never felt a thing.");
		}
		else
		{
			Attempts--;
			if (Attempts <= 0) { Status = TEXT("BUSTED. The target noticed. They're calling the guards."); Finish(false, 0); }
			else { Status = FString::Printf(TEXT("Missed! %d attempts left. Try again."), Attempts); SafeZone = 30.f; Timer = Timer * 0.5f; }
			bGrabbing = false;
		}
	}
}

// ---------------------------------------------------------------------------
// GDD 13.10 — Safe Cracking minigame.
// ---------------------------------------------------------------------------
void USafeCrackMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("Safe Cracking");
	Kind = EMinigameKind::SafeCrack;
	TargetNums[0] = FMath::RandRange(5, 95); TargetNums[1] = FMath::RandRange(5, 95); TargetNums[2] = FMath::RandRange(5, 95);
	CurrentNum = 0; FoundCount = 0; DialSpeed = 0.f; TimeLeft = 30.f;
	Status = FString::Printf(TEXT("Find 3 numbers. A/D rotate. Listen for the click at the right spot. Find #%d."), FoundCount + 1);
}

void USafeCrackMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	TimeLeft -= DeltaTime;
	CurrentNum += FMath::RoundToInt(DialSpeed * DeltaTime * 40.f);
	if (CurrentNum < 0) CurrentNum = 99;
	if (CurrentNum > 99) CurrentNum = 0;
	DialSpeed *= 0.92f;
	if (TimeLeft <= 0.f) { Status = TEXT("Time's up. The safe remains sealed."); Finish(false, 0); }
}

void USafeCrackMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 60, H * 0.3f), FVector2D(1.5f, 1.5f), FLinearColor::Cyan);
	// Draw dial
	float CX = W / 2, CY = H * 0.5f, R = 100.f;
	for (int32 i = 0; i < 100; i += 5)
	{
		float rad = (i * 3.6f - 90.f) * 3.14159f / 180.f;
		float x1 = CX + FMath::Cos(rad) * R, y1 = CY + FMath::Sin(rad) * R;
		float x2 = CX + FMath::Cos(rad) * (R - 12), y2 = CY + FMath::Sin(rad) * (R - 12);
		Canvas->DrawLine(x1, y1, x2, y2, FLinearColor(0.5f, 0.5f, 0.5f));
	}
	// Needle
	float needleRad = (CurrentNum * 3.6f - 90.f) * 3.14159f / 180.f;
	Canvas->DrawLine(CX, CY, CX + FMath::Cos(needleRad) * R * 0.85f, CY + FMath::Sin(needleRad) * R * 0.85f, FLinearColor::Red);
	Canvas->DrawText(F, FString::Printf(TEXT("%d"), CurrentNum), FVector2D(CX - 12, CY + 30), FVector2D(1.8f, 1.8f), FLinearColor::White);
	Canvas->DrawText(F, FString::Printf(TEXT("Found: %d/3  Time: %.1fs"), FoundCount, TimeLeft), FVector2D(40, H * 0.65f), FVector2D(1.1f, 1.1f), FLinearColor::Yellow);
	Canvas->DrawText(F, *Status, FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void USafeCrackMinigame::HandleKey(const FKey& Key)
{
	if (Key == EKeys::D || Key == EKeys::Right) DialSpeed += 0.5f;
	else if (Key == EKeys::A || Key == EKeys::Left) DialSpeed -= 0.5f;
	else if (Key == EKeys::SpaceBar)
	{
		int32 Dist = FMath::Abs(CurrentNum - TargetNums[FoundCount]);
		if (Dist <= 3) // within tolerance
		{
			FoundCount++;
			if (FoundCount >= 3) { Status = TEXT("CLUNK. The safe opens."); Finish(true, 35); }
			else Status = FString::Printf(TEXT("CLICK! Found #%d. Now find #%d."), FoundCount, FoundCount + 1);
		}
		else Status = TEXT("Nothing. No click. Keep searching.");
	}
}

// ---------------------------------------------------------------------------
// GDD 13.6 — Enchanting minigame (rune drawing).
// ---------------------------------------------------------------------------
void UEnchantingMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C); Title = TEXT("Enchanting"); Kind = EMinigameKind::Enchanting;
	RuneIdx = 0; Precision = 100.f; TimeLeft = 20.f; CursorX = 0; CursorY = 0; bDrawing = false;
	// Generate 3 runes, each with 6 points
	PointsHit.Init(false, 6); Runepoints.Reset();
	for (int32 i = 0; i < 6; ++i) Runepoints.Add(FVector2D(FMath::RandRange(50, 150), FMath::RandRange(-60, 60)));
	Status = TEXT("Trace the rune with WASD. Hit all 6 glowing points. 3 runes to enchant.");
}

void UEnchantingMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	TimeLeft -= DeltaTime;
	if (TimeLeft <= 0.f) { Status = TEXT("The rune fades. Enchantment failed."); Finish(false, 10); }
}

void UEnchantingMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 40, H * 0.25f), FVector2D(1.5f, 1.5f), FLinearColor::Cyan);
	float CX = W / 2, CY = H * 0.5f;
	// Draw target points
	for (int32 i = 0; i < Runepoints.Num(); ++i)
	{
		float px = CX + Runepoints[i].X, py = CY + Runepoints[i].Y;
		FLinearColor col = PointsHit[i] ? FLinearColor::Green : FLinearColor(1.f, 0.75f, 0.2f);
		Canvas->DrawRect(col, px - 6, py - 6, 12, 12);
		Canvas->DrawText(F, FString::Printf(TEXT("%d"), i + 1), FVector2D(px - 4, py - 20), FVector2D(0.7f, 0.7f), FLinearColor::White);
	}
	// Draw cursor
	Canvas->DrawRect(FLinearColor::Green, CX + CursorX - 4, CY + CursorY - 4, 8, 8);
	Canvas->DrawText(F, FString::Printf(TEXT("Rune %d/3  Precision: %.0f%%  Time: %.1fs"), RuneIdx + 1, Precision, TimeLeft),
		FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor::Yellow);
	Canvas->DrawText(F, Status, FVector2D(40, H * 0.75f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void UEnchantingMinigame::HandleKey(const FKey& Key)
{
	if (Key == EKeys::W || Key == EKeys::Up) CursorY -= 4.f;
	else if (Key == EKeys::S || Key == EKeys::Down) CursorY += 4.f;
	else if (Key == EKeys::A || Key == EKeys::Left) CursorX -= 4.f;
	else if (Key == EKeys::D || Key == EKeys::Right) CursorX += 4.f;
	// Check proximity to next unhit point
	for (int32 i = 0; i < Runepoints.Num(); ++i)
	{
		if (PointsHit[i]) continue;
		float dist = FMath::Sqrt(FMath::Square(CursorX - Runepoints[i].X) + FMath::Square(CursorY - Runepoints[i].Y));
		if (dist < 18.f)
		{
			PointsHit[i] = true;
			bool bAllHit = true; for (bool b : PointsHit) if (!b) bAllHit = false;
			if (bAllHit)
			{
				RuneIdx++;
				if (RuneIdx >= 3) { Status = TEXT("RUNES COMPLETE. The enchantment holds!"); Finish(true, FMath::RoundToInt(Precision)); }
				else { PointsHit.Init(false, 6); Runepoints.Reset(); for (int32 j = 0; j < 6; ++j) Runepoints.Add(FVector2D(FMath::RandRange(50, 150), FMath::RandRange(-60, 60))); Precision = 100.f; Status = FString::Printf(TEXT("Rune %d complete! Draw the next."), RuneIdx + 1); }
			}
			break;
		}
	}
	Precision -= 0.5f;
}

// ---------------------------------------------------------------------------
// GDD 13.8 — Cooking minigame (chop, stir, flip, season, plate).
// ---------------------------------------------------------------------------
void UCookingMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C); Title = TEXT("Cooking"); Kind = EMinigameKind::Cooking;
	Phase = 0; PhaseTimer = 0.f; Score = 0.f; ChopCount = 0; ChopTarget = 8;
	StirSpeed = 0.f; StirTarget = 0.5f; bFlipReady = false; FlipTimer = 0.f; SeasonLevel = 0; PlatePosition = 0;
	Status = TEXT("Phase 1/5: CHOP! Press Space repeatedly to dice the ingredients.");
}

void UCookingMinigame::Tick(float DeltaTime)
{
	if (bComplete) return;
	PhaseTimer += DeltaTime;
	if (Phase == 0) { if (PhaseTimer > 10.f) { Phase = 1; PhaseTimer = 0.f; Status = TEXT("Phase 2/5: STIR! Tap A and D to maintain rhythm speed."); } }
	else if (Phase == 1) { if (PhaseTimer > 8.f) { Phase = 2; PhaseTimer = 0.f; bFlipReady = true; Status = TEXT("Phase 3/5: FLIP! Press Space when the indicator says GO."); } }
	else if (Phase == 2 && bFlipReady) { FlipTimer += DeltaTime; if (FlipTimer > 1.5f) { bFlipReady = false; Phase = 3; Status = TEXT("Phase 4/5: SEASON! Press W/S to adjust seasoning level."); } }
	else if (Phase == 3) { if (PhaseTimer > 6.f) { Phase = 4; Status = TEXT("Phase 5/5: PLATE! Use A/D to position the food on the plate, Space to serve."); } }
}

void UCookingMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont(); float Y = H * 0.3f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 30, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += 35;
	const TCHAR* PhaseNames[] = { TEXT("CHOP"), TEXT("STIR"), TEXT("FLIP"), TEXT("SEASON"), TEXT("PLATE") };
	Canvas->DrawText(F, FString::Printf(TEXT("Phase: %s  Score: %.0f"), PhaseNames[Phase], Score), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::White); Y += 30;
	if (Phase == 0) { Canvas->DrawText(F, FString::Printf(TEXT("Chops: %d/%d"), ChopCount, ChopTarget), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Green); }
	else if (Phase == 1) { Canvas->DrawText(F, FString::Printf(TEXT("Stir speed: %.2f (target ~%.2f)"), StirSpeed, StirTarget), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::Yellow); }
	else if (Phase == 2 && bFlipReady) { Canvas->DrawText(F, FString::Printf(TEXT("FLIP NOW! (Space)"), FMath::RoundToInt(FlipTimer * 100)), FVector2D(40, Y), FVector2D(1.5f, 1.5f), FLinearColor::Red); }
	else if (Phase == 3) { Canvas->DrawText(F, FString::Printf(TEXT("Seasoning: %d (target: 5)"), SeasonLevel), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::Yellow); }
	else if (Phase == 4) { Canvas->DrawText(F, FString::Printf(TEXT("Plating position: %d (target: 5)"), PlatePosition), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::Yellow); }
	Canvas->DrawText(F, Status, FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void UCookingMinigame::HandleKey(const FKey& Key)
{
	if (Phase == 0 && Key == EKeys::SpaceBar) { ChopCount++; Score += 8; if (ChopCount >= ChopTarget) { Phase = 1; PhaseTimer = 0.f; Status = TEXT("Phase 2/5: STIR!"); } }
	else if (Phase == 1)
	{
		if (Key == EKeys::A) StirSpeed -= 0.1f;
		else if (Key == EKeys::D) StirSpeed += 0.1f;
		if (FMath::Abs(StirSpeed - StirTarget) < 0.15f) Score += 2;
	}
	else if (Phase == 2 && bFlipReady && Key == EKeys::SpaceBar) { Score += (FlipTimer < 0.3f) ? 30 : (FlipTimer < 0.7f) ? 20 : 10; bFlipReady = false; Phase = 3; }
	else if (Phase == 3) { if (Key == EKeys::W) SeasonLevel = FMath::Min(10, SeasonLevel + 1); else if (Key == EKeys::S) SeasonLevel = FMath::Max(0, SeasonLevel - 1); }
	else if (Phase == 4)
	{
		if (Key == EKeys::A) PlatePosition = FMath::Max(0, PlatePosition - 1);
		else if (Key == EKeys::D) PlatePosition = FMath::Min(10, PlatePosition + 1);
		else if (Key == EKeys::SpaceBar) { Score += FMath::Abs(PlatePosition - 5) < 2 ? 25 : 10; Phase = 5; Status = FString::Printf(TEXT("DISH COMPLETE! Score: %.0f"), Score); Finish(true, FMath::RoundToInt(Score)); }
	}
}

static int32 CalcBJTotal(int32 T, int32 A) { while (T > 21 && A > 0) { T -= 10; A--; } return T; }
static int32 CardVal(int32 C) { return FMath::Min(C % 13 + 1, 10); }

// ---------------------------------------------------------------------------
// GDD 13.8 — Blacksmithing.
// ---------------------------------------------------------------------------
void UBlacksmithingMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C); Title = TEXT("Blacksmithing"); Kind = EMinigameKind::Blacksmithing;
	Phase = 0; Score = 0; Timer = 0.f; Temp = 20.f; TargetTemp = 85.f; PourAngle = 90.f; HammerPos = 0;
	HammerTarget = 3; QuenchTimer = 0.f; bQuenching = false; SharpenAngle = 45;
	Status = TEXT("Phase 1/5: SMELT. Tap Space to pump the bellows. Keep temp at 85.");
}

void UBlacksmithingMinigame::Tick(float DeltaTime)
{
	if (bComplete) return; Timer += DeltaTime; Temp -= DeltaTime * 4.f;
	if (Phase == 1) PourAngle += DeltaTime * 8.f;
	if (Phase == 3 && bQuenching) { QuenchTimer += DeltaTime; if (QuenchTimer > 2.5f) { Score -= 20; Status = TEXT("CRACK! Quenched too long. The blade cracked."); Phase = 4; Timer = 0.f; bQuenching = false; } }
}

void UBlacksmithingMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont(); float Y = H * 0.25f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 50, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += 35;
	if (Phase == 0) { Canvas->DrawText(F, FString::Printf(TEXT("Furnace temp: %.0f / 85"), Temp), FVector2D(40, Y), FVector2D(1.2f, 1.2f), Temp > 80 ? FLinearColor::Green : FLinearColor::Red); }
	else if (Phase == 1) { Canvas->DrawText(F, FString::Printf(TEXT("Pour angle: %.0f (target: 90)"), PourAngle), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Yellow); }
	else if (Phase == 2) { Canvas->DrawText(F, FString::Printf(TEXT("Hammer spot: %d (target: %d)"), HammerPos, HammerTarget), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Yellow); }
	else if (Phase == 3) { Canvas->DrawText(F, FString::Printf(TEXT("QUENCH! Time: %.1fs (tap Space to lift)"), QuenchTimer), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Red); }
	else if (Phase == 4) { Canvas->DrawText(F, FString::Printf(TEXT("Sharpen angle: %d (tap A/D, lock at 45)"), SharpenAngle), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Yellow); }
	Canvas->DrawText(F, FString::Printf(TEXT("Score: %.0f"), Score), FVector2D(W / 2 - 40, H * 0.55f), FVector2D(1.3f, 1.3f), FLinearColor::White);
	Canvas->DrawText(F, Status, FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void UBlacksmithingMinigame::HandleKey(const FKey& Key)
{
	if (Phase == 0 && Key == EKeys::SpaceBar) { Temp += 6.f; if (Temp >= 80.f && Temp <= 90.f) Score += 3; if (Timer > 8.f) { Phase = 1; Timer = 0.f; Status = TEXT("Phase 2/5: POUR. Hold A/D to adjust pour angle to 90."); } }
	else if (Phase == 1) { if (Key == EKeys::A) PourAngle -= 4.f; else if (Key == EKeys::D) PourAngle += 4.f; if (FMath::Abs(PourAngle - 90) < 5) Score += 2; if (Timer > 7.f) { Phase = 2; Timer = 0.f; Status = TEXT("Phase 3/5: HAMMER. Tap A/D to move hammer to the target spot."); } }
	else if (Phase == 2) { if (Key == EKeys::A) HammerPos = FMath::Max(0, HammerPos - 1); else if (Key == EKeys::D) HammerPos = FMath::Min(10, HammerPos + 1); else if (Key == EKeys::SpaceBar) { if (HammerPos == HammerTarget) { Score += 15; HammerTarget = FMath::RandRange(0, 10); } else Score -= 5; if (Timer > 8.f) { Phase = 3; Timer = 0.f; bQuenching = true; Status = TEXT("Phase 4/5: QUENCH. Tap Space to lift from oil."); } } }
	else if (Phase == 3 && bQuenching && Key == EKeys::SpaceBar) { bQuenching = false; if (QuenchTimer > 1.f && QuenchTimer < 2.f) Score += 30; Phase = 4; Timer = 0.f; Status = TEXT("Phase 5/5: SHARPEN. A/D to set angle to 45, Space to finish."); }
	else if (Phase == 4) { if (Key == EKeys::A) SharpenAngle = FMath::Max(0, SharpenAngle - 2); else if (Key == EKeys::D) SharpenAngle = FMath::Min(90, SharpenAngle + 2); else if (Key == EKeys::SpaceBar) { if (FMath::Abs(SharpenAngle - 45) < 5) Score += 20; Finish(true, FMath::RoundToInt(Score)); } }
}

// ---------------------------------------------------------------------------
// GDD 13.9 — Blackjack.
// ---------------------------------------------------------------------------
void UBlackjackMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C); Title = TEXT("Blackjack"); Kind = EMinigameKind::Blackjack;
	Chips = 1000; Bet = 100; Phase = 0; Cursor = 0;
	Status = TEXT("W/S change bet, Space to deal.");
}

void UBlackjackMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont(); float Y = H * 0.25f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 40, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += 35;
	Canvas->DrawText(F, FString::Printf(TEXT("Chips: %d  Bet: %d"), Chips, Bet), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Yellow); Y += 25;
	if (Phase == 0) { Canvas->DrawText(F, TEXT("W/S adjust bet. Space to deal."), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::White); }
	else {
		Canvas->DrawText(F, FString::Printf(TEXT("Your hand: %d"), PlayerTotal), FVector2D(40, Y), FVector2D(1.3f, 1.3f), FLinearColor::Green); Y += 25;
		Canvas->DrawText(F, FString::Printf(TEXT("Dealer: %d"), Phase == 2 ? DealerTotal : DealerTotal > 0 ? (int32)DealerTotal : 0), FVector2D(40, Y), FVector2D(1.3f, 1.3f), FLinearColor::Red); Y += 30;
		if (Phase == 1) {
			Canvas->DrawText(F, Cursor == 0 ? TEXT("> HIT") : TEXT("  Hit"), FVector2D(40, Y), FVector2D(1.2f, 1.2f), Cursor == 0 ? FLinearColor::Green : FLinearColor::White);
			Canvas->DrawText(F, Cursor == 1 ? TEXT("> STAND") : TEXT("  Stand"), FVector2D(40, Y + 22), FVector2D(1.2f, 1.2f), Cursor == 1 ? FLinearColor::Green : FLinearColor::White);
		}
		else if (Phase == 2) Canvas->DrawText(F, TEXT("Dealer drawing..."), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::Yellow);
		else if (Phase == 3) Canvas->DrawText(F, *Status, FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Green);
	}
}

void UBlackjackMinigame::HandleKey(const FKey& Key)
{
	if (Phase == 0)
	{
		if (Key == EKeys::W || Key == EKeys::Up) Bet = FMath::Min(1000, Bet + 100);
		else if (Key == EKeys::S || Key == EKeys::Down) Bet = FMath::Max(100, Bet - 100);
		else if (Key == EKeys::SpaceBar) { Phase = 1; Cursor = 0; PlayerTotal = CardVal(FMath::RandRange(0, 51)) + CardVal(FMath::RandRange(0, 51)); PlayerAces = 0; DealerTotal = CardVal(FMath::RandRange(0, 51)); DealerAces = 0; PlayerTotal = CalcBJTotal(PlayerTotal, 0); DealerTotal = CalcBJTotal(DealerTotal, 0); Status = TEXT("Choose: Hit or Stand."); }
	}
	else if (Phase == 1)
	{
		if (Key == EKeys::W || Key == EKeys::Up) Cursor = (Cursor + 1) % 2;
		else if (Key == EKeys::S || Key == EKeys::Down) Cursor = (Cursor + 1) % 2;
		else if (Key == EKeys::SpaceBar)
		{
			if (Cursor == 0) { PlayerTotal += CardVal(FMath::RandRange(0, 51)); PlayerTotal = CalcBJTotal(PlayerTotal, 0); if (PlayerTotal > 21) { Phase = 3; Chips -= Bet; Status = TEXT("BUST! You lose."); } }
			else { Phase = 2; Status = TEXT("Dealer draws..."); }
		}
	}
	else if (Phase == 2)
	{
		while (DealerTotal < 17) { DealerTotal += CardVal(FMath::RandRange(0, 51)); DealerTotal = CalcBJTotal(DealerTotal, 0); }
		Phase = 3;
		if (DealerTotal > 21 || PlayerTotal > DealerTotal) { Chips += Bet; Status = TEXT("YOU WIN!"); }
		else if (PlayerTotal == DealerTotal) Status = TEXT("PUSH. Bet returned.");
		else { Chips -= Bet; Status = TEXT("Dealer wins."); }
	}
	else if (Phase == 3 && Key == EKeys::SpaceBar) { Phase = 0; Finish(Chips > 500, 20); }
}

// ---------------------------------------------------------------------------
// GDD 13.9 — Roulette.
// ---------------------------------------------------------------------------
void URouletteMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C); Title = TEXT("Roulette"); Kind = EMinigameKind::Roulette;
	Chips = 1000; Bet = 100; Cursor = 0; OnNumber = 17; bSpinning = false;
	Status = TEXT("W/S choose bet type, A/D change number, Space spin.");
}

void URouletteMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont(); float Y = H * 0.25f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 40, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += 35;
	Canvas->DrawText(F, FString::Printf(TEXT("Chips: %d  Bet: %d"), Chips, Bet), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Yellow); Y += 25;
	if (bSpinning) { Canvas->DrawText(F, FString::Printf(TEXT("Ball lands on: %d"), Result), FVector2D(40, Y), FVector2D(1.5f, 1.5f), FLinearColor::Green); }
	else {
		Canvas->DrawText(F, FString::Printf(TEXT("Bet type: %s  Number: %d"), *BetType, OnNumber), FVector2D(40, Y), FVector2D(1.1f, 1.1f), FLinearColor::White);
		Canvas->DrawText(F, TEXT("W/S: change type  A/D: number  Space: spin"), FVector2D(40, Y + 30), FVector2D(1.0f, 1.0f), FLinearColor(0.6f, 0.6f, 0.6f));
	}
	Canvas->DrawText(F, Status, FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void URouletteMinigame::HandleKey(const FKey& Key)
{
	if (bSpinning) { if (Key == EKeys::SpaceBar) { bSpinning = false; Finish(Chips > 500, 15); } return; }
	if (Key == EKeys::W || Key == EKeys::Up) { static const TCHAR* Types[] = { TEXT("number"), TEXT("red"), TEXT("black"), TEXT("even"), TEXT("odd") }; Cursor = (Cursor + 4) % 5; BetType = Types[Cursor]; }
	else if (Key == EKeys::S || Key == EKeys::Down) { static const TCHAR* Types[] = { TEXT("number"), TEXT("red"), TEXT("black"), TEXT("even"), TEXT("odd") }; Cursor = (Cursor + 1) % 5; BetType = Types[Cursor]; }
	else if (Key == EKeys::A || Key == EKeys::Left) OnNumber = (OnNumber + 35) % 37;
	else if (Key == EKeys::D || Key == EKeys::Right) OnNumber = (OnNumber + 1) % 37;
	else if (Key == EKeys::SpaceBar)
	{
		Result = FMath::RandRange(0, 36); bSpinning = true;
		bool bRed = (Result % 2 == 1); bool bWon = false; int32 Mult = 0;
		if (BetType == TEXT("number") && OnNumber == Result) { bWon = true; Mult = 35; }
		else if (BetType == TEXT("red") && bRed) { bWon = true; Mult = 2; }
		else if (BetType == TEXT("black") && !bRed && Result != 0) { bWon = true; Mult = 2; }
		else if (BetType == TEXT("even") && Result % 2 == 0 && Result != 0) { bWon = true; Mult = 2; }
		else if (BetType == TEXT("odd") && Result % 2 == 1) { bWon = true; Mult = 2; }
		if (bWon) { Chips += Bet * Mult; Status = FString::Printf(TEXT("WIN! +%d chips."), Bet * Mult); }
		else { Chips -= Bet; Status = TEXT("Loss."); }
	}
}

// ---------------------------------------------------------------------------
// GDD 13.9 — Slot Machines.
// ---------------------------------------------------------------------------
void USlotsMinigame::Begin(AChimeraCharacter* C)
{
	Super::Begin(C); Title = TEXT("Slot Machine"); Kind = EMinigameKind::Slots;
	Chips = 1000; Bet = 50; for (int32 i = 0; i < 3; ++i) Reels[i] = 0;
	Status = TEXT("Space to spin. W/S adjust bet.");
}

void USlotsMinigame::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont(); float Y = H * 0.3f;
	Canvas->DrawText(F, *Title, FVector2D(W / 2 - 50, Y), FVector2D(1.5f, 1.5f), FLinearColor::Cyan); Y += 40;
	FLinearColor Colors[] = { FLinearColor::Red, FLinearColor::Green, FLinearColor::Blue, FLinearColor::Yellow, FLinearColor::Magenta };
	const TCHAR* Sym[] = { TEXT("7"), TEXT("BAR"), TEXT("CHERRY"), TEXT("BELL"), TEXT("LEMON") };
	Canvas->DrawText(F, FString::Printf(TEXT("[ %s | %s | %s ]"), Sym[Reels[0]], Sym[Reels[1]], Sym[Reels[2]]),
		FVector2D(W / 2 - 80, Y), FVector2D(2.0f, 2.0f), FLinearColor::White); Y += 50;
	Canvas->DrawText(F, FString::Printf(TEXT("Chips: %d  Bet: %d"), Chips, Bet), FVector2D(40, Y), FVector2D(1.2f, 1.2f), FLinearColor::Yellow);
	Canvas->DrawText(F, Status, FVector2D(40, H * 0.7f), FVector2D(1.0f, 1.0f), FLinearColor(0.8f, 0.8f, 0.8f));
}

void USlotsMinigame::HandleKey(const FKey& Key)
{
	if (bSpinning) return;
	if (Key == EKeys::W || Key == EKeys::Up) Bet = FMath::Min(500, Bet + 50);
	else if (Key == EKeys::S || Key == EKeys::Down) Bet = FMath::Max(50, Bet - 50);
	else if (Key == EKeys::SpaceBar)
	{
		Chips -= Bet; bSpinning = true;
		int32 Winnings = 0; bool bJackpot = false;
		// Spin individually for dramatic effect
		Reels[0] = FMath::RandRange(0, 4); Reels[1] = FMath::RandRange(0, 4); Reels[2] = FMath::RandRange(0, 4);
		if (Reels[0] == Reels[1] && Reels[1] == Reels[2]) { Winnings = Bet * 10; if (Reels[0] == 0) { Winnings = Bet * 50; bJackpot = true; } }
		else if (Reels[0] == Reels[1] || Reels[1] == Reels[2] || Reels[0] == Reels[2]) Winnings = Bet * 3;
		Ch chips += Winnings;
		Status = bJackpot ? TEXT("JACKPOT!!! 50x payout!") : Winnings > 0 ? FString::Printf(TEXT("Win! +%d chips."), Winnings) : TEXT("No match. Try again.");
		bSpinning = false;
	}
}

// ---------------------------------------------------------------------------
// GDD 5.1 - Character creation.
// ---------------------------------------------------------------------------
void UCharacterCreationFlow::Begin(AChimeraCharacter* C)
{
	Super::Begin(C);
	Title = TEXT("CHIMERA - Character Creation");
	Kind = EMinigameKind::CharacterCreation;
	LifePaths = { TEXT("Cop"), TEXT("Doctor"), TEXT("Superhero"), TEXT("Dragon Rider"),
		TEXT("Racer"), TEXT("Builder"), TEXT("Netrunner"), TEXT("Fixer"), TEXT("Intern"),
		TEXT("Tribute"), TEXT("Dauntless Initiate"), TEXT("Surgeon"), TEXT("Detective"),
		TEXT("Mage"), TEXT("Pilot"), TEXT("Chef"), TEXT("Musician"), TEXT("Athlete"),
		TEXT("Politician"), TEXT("Farmer"), TEXT("Blacksmith"), TEXT("Thief"),
		TEXT("Journalist"), TEXT("Scientist"), TEXT("Explorer"), TEXT("Nomad"), TEXT("Noble"), TEXT("Refugee") };
	GenderOptions = { TEXT("Male"), TEXT("Female"), TEXT("Non-Binary"), TEXT("Trans Man"), TEXT("Trans Woman"),
		TEXT("Genderfluid"), TEXT("Agender"), TEXT("Questioning") };
	OrientationOptions = { TEXT("Straight"), TEXT("Gay"), TEXT("Lesbian"), TEXT("Bisexual"),
		TEXT("Pansexual"), TEXT("Asexual"), TEXT("Demisexual"), TEXT("Aromantic") };
	FearOptions = { TEXT("Heights"), TEXT("Drowning"), TEXT("Fire"), TEXT("Betrayal"), TEXT("Abandonment"),
		TEXT("Failure"), TEXT("Spiders"), TEXT("Darkness"), TEXT("Enclosed spaces"), TEXT("Crowds"),
		TEXT("Being forgotten"), TEXT("Losing control"), TEXT("Needles"), TEXT("Thunder"),
		TEXT("Being wrong"), TEXT("Death of a loved one"), TEXT("Public humiliation"), TEXT("Losing your mind") };
	ChosenFears.Reset();
	Points = 15; Cursor = 0; FearCursor = 0;
	Phase = EPhase_Story; StoryLine = 0;
	StoryText = ChimeraStory::GetOpeningNarrative();
	Status = TEXT("Press Space to continue the story...");
}

void UCharacterCreationFlow::Draw(UCanvas* Canvas, float W, float H)
{
	UFont* F = GEngine->GetSmallFont();
	Canvas->DrawText(F, Title, FVector2D(40, 20), FVector2D(2.2f, 2.2f), FLinearColor::Cyan);
	float Y = 70;
	const TCHAR* PhaseNames[] = { TEXT("Life Path"), TEXT("Gender"), TEXT("Orientation"), TEXT("Fears"), TEXT("Attributes"), TEXT("Review") };
	Canvas->DrawText(F, FString::Printf(TEXT("Step %d/6: %s"), Phase + 1, PhaseNames[Phase]), FVector2D(40, 55), FVector2D(1.0f, 1.0f), FLinearColor::Yellow);

	if (Phase == EPhase_LifePath)
	{
		for (int32 i = 0; i < LifePaths.Num(); ++i)
		{
			bool Sel = (i == Cursor);
			Canvas->DrawText(F, FString::Printf(TEXT("%s %s"), Sel ? TEXT(">") : TEXT(" "), *LifePaths[i]),
				FVector2D(80, Y), FVector2D(1.4f, 1.4f), Sel ? FLinearColor::Green : FLinearColor::White);
			Y += 26;
		}
		Canvas->DrawText(F, TEXT("A: random   Space: confirm"), FVector2D(80, Y + 10), FVector2D(1.1f, 1.1f), FLinearColor(0.7f, 0.7f, 0.7f));
	}
	else if (Phase == EPhase_Gender)
	{
		for (int32 i = 0; i < GenderOptions.Num(); ++i)
		{
			bool Sel = (i == Cursor);
			Canvas->DrawText(F, FString::Printf(TEXT("%s %s"), Sel ? TEXT(">") : TEXT("  "), *GenderOptions[i]),
				FVector2D(80, Y), FVector2D(1.4f, 1.4f), Sel ? FLinearColor::Green : FLinearColor::White);
			Y += 28;
		}
	}
	else if (Phase == EPhase_Orientation)
	{
		for (int32 i = 0; i < OrientationOptions.Num(); ++i)
		{
			bool Sel = (i == Cursor);
			Canvas->DrawText(F, FString::Printf(TEXT("%s %s"), Sel ? TEXT(">") : TEXT("  "), *OrientationOptions[i]),
				FVector2D(80, Y), FVector2D(1.4f, 1.4f), Sel ? FLinearColor::Green : FLinearColor::White);
			Y += 28;
		}
	}
	else if (Phase == EPhase_Fears)
	{
		if (ChosenFears.Num() < 3)
		{
			for (int32 i = 0; i < FearOptions.Num(); ++i)
			{
				bool Sel = (i == Cursor);
				FString Mark = ChosenFears.Contains(FearOptions[i]) ? TEXT("[X]") : TEXT("[ ]");
				Canvas->DrawText(F, FString::Printf(TEXT("%s %s %s"), Sel ? TEXT(">") : TEXT(" "), *Mark, *FearOptions[i]),
					FVector2D(80, Y), FVector2D(1.3f, 1.3f), Sel ? FLinearColor::Green : ChosenFears.Contains(FearOptions[i]) ? FLinearColor::Red : FLinearColor::White);
				Y += 24;
			}
			Canvas->DrawText(F, FString::Printf(TEXT("Choose 3 fears: %d/3 selected. Space to select, Enter when done."), ChosenFears.Num()),
				FVector2D(80, Y + 10), FVector2D(1.0f, 1.0f), FLinearColor(0.7f, 0.7f, 0.7f));
		}
	}
	else if (Phase == EPhase_Attributes)
	{
		static const TArray<FString> AttrNames = { TEXT("Body"), TEXT("Reflexes"), TEXT("Intelligence"), TEXT("Technical"), TEXT("Cool"), TEXT("Magic"), TEXT("Social") };
		float X = W * 0.55f; Y = 80;
		for (int32 i = 0; i < AttrNames.Num(); ++i)
		{
			Canvas->DrawText(F, FString::Printf(TEXT("%s: %d"), *AttrNames[i], Char ? Char->GetAttribute((EAttribute)i) : 5),
				FVector2D(X, Y), FVector2D(1.3f, 1.3f), i == Cursor ? FLinearColor::Green : FLinearColor::White);
			Y += 34;
		}
		Canvas->DrawText(F, FString::Printf(TEXT("Points: %d"), Points), FVector2D(X, Y + 10), FVector2D(1.5f, 1.5f), FLinearColor::Yellow);
		Canvas->DrawText(F, TEXT("A/D: spend points   W/S: select attribute   Space: confirm"), FVector2D(80, H - 40), FVector2D(1.1f, 1.1f), FLinearColor(0.7f, 0.7f, 0.7f));
	}
	else if (Phase == EPhase_Review)
	{
		Canvas->DrawText(F, FString::Printf(TEXT("Life Path: %s"), *LifePaths[Cursor]), FVector2D(80, Y), FVector2D(1.3f, 1.3f), FLinearColor::Green); Y += 30;
		Canvas->DrawText(F, TEXT("Review your choices. Space to finalise. Esc to restart."), FVector2D(80, Y + 10), FVector2D(1.1f, 1.1f), FLinearColor::White);
	}
	Canvas->DrawText(F, Status, FVector2D(40, H - 60), FVector2D(1.0f, 1.0f), FLinearColor(0.9f, 0.9f, 0.7f));
}

void UCharacterCreationFlow::HandleKey(const FKey& Key)
{
	if (bComplete) return;
	if (Phase == EPhase_LifePath)
	{
		if (Key == EKeys::W || Key == EKeys::Up) { Cursor = (Cursor + LifePaths.Num() - 1) % LifePaths.Num(); }
		else if (Key == EKeys::S || Key == EKeys::Down) { Cursor = (Cursor + 1) % LifePaths.Num(); }
		else if (Key == EKeys::A) { Cursor = FMath::RandRange(0, LifePaths.Num() - 1); } // random
		else if (Key == EKeys::SpaceBar || Key == EKeys::Enter) { Phase = EPhase_Gender; Cursor = 0; Status = TEXT("Phase 2/6: Choose your gender identity."); }
	}
	else if (Phase == EPhase_Gender)
	{
		if (Key == EKeys::W || Key == EKeys::Up) { Cursor = (Cursor + GenderOptions.Num() - 1) % GenderOptions.Num(); }
		else if (Key == EKeys::S || Key == EKeys::Down) { Cursor = (Cursor + 1) % GenderOptions.Num(); }
		else if (Key == EKeys::SpaceBar || Key == EKeys::Enter)
		{
			if (Char && Char->GetSession()) Char->GetSession()->SetGender((EGender)Cursor);
			Phase = EPhase_Orientation; Cursor = 0; Status = TEXT("Phase 3/6: Choose your orientation.");
		}
	}
	else if (Phase == EPhase_Orientation)
	{
		if (Key == EKeys::W || Key == EKeys::Up) { Cursor = (Cursor + OrientationOptions.Num() - 1) % OrientationOptions.Num(); }
		else if (Key == EKeys::S || Key == EKeys::Down) { Cursor = (Cursor + 1) % OrientationOptions.Num(); }
		else if (Key == EKeys::SpaceBar || Key == EKeys::Enter)
		{
			if (Char && Char->GetSession()) Char->GetSession()->SetOrientation((EOrientation)Cursor);
			Phase = EPhase_Fears; Cursor = 0; Status = TEXT("Phase 4/6: Choose 3 fears. Space to select, Enter when done.");
		}
	}
	else if (Phase == EPhase_Fears)
	{
		if (Key == EKeys::W || Key == EKeys::Up) { Cursor = (Cursor + FearOptions.Num() - 1) % FearOptions.Num(); }
		else if (Key == EKeys::S || Key == EKeys::Down) { Cursor = (Cursor + 1) % FearOptions.Num(); }
		else if (Key == EKeys::SpaceBar)
		{
			FString Fear = FearOptions[Cursor];
			if (ChosenFears.Contains(Fear)) ChosenFears.Remove(Fear);
			else if (ChosenFears.Num() < 3) ChosenFears.Add(Fear);
		}
		else if (Key == EKeys::Enter && ChosenFears.Num() >= 3)
		{
			if (Char && Char->GetSession())
			{
				Char->GetSession()->PlayerFears = ChosenFears;
				Char->GetSession()->RecordEvent(FString::Printf(TEXT("Fears chosen: %s, %s, %s"), *ChosenFears[0], *ChosenFears[1], *ChosenFears[2]));
			}
			Phase = EPhase_Attributes; Cursor = 0; Status = TEXT("Phase 5/6: Distribute attribute points (W/S select, A/D adjust, Space to finish).");
		}
	}
	else if (Phase == EPhase_Attributes)
	{
		if (Key == EKeys::W || Key == EKeys::Up) { Cursor = (Cursor + 6) % 7; }
		else if (Key == EKeys::S || Key == EKeys::Down) { Cursor = (Cursor + 1) % 7; }
		else if (Key == EKeys::A || Key == EKeys::Left) { if (Char && Points > 0) { Char->AddAttribute((EAttribute)Cursor, -1); Points++; } }
		else if (Key == EKeys::D || Key == EKeys::Right) { if (Char && Points > 0 && Char->GetAttribute((EAttribute)Cursor) < 100) { Char->AddAttribute((EAttribute)Cursor, 1); Points--; } }
		else if (Key == EKeys::SpaceBar || Key == EKeys::Enter) { Phase = EPhase_Review; Status = TEXT("Phase 6/6: Review and finalise. Space to confirm, Esc to restart."); }
	}
	else if (Phase == EPhase_Review)
	{
		if (Key == EKeys::SpaceBar || Key == EKeys::Enter)
		{
			if (Char) Char->FinishCharacterCreation(LifePaths[Cursor]);
			Finish(true, Points);
		}
		else if (Key == EKeys::Escape) { Begin(Char); } // restart
	}
}
