// GDD 13 (minigames encyclopedia) + 5.1 (character creation). A small framework
// of state-machine minigames rendered by the HUD and driven by raw key polling.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "InputCoreTypes.h"
#include "UObject/NoExportTypes.h"
#include "ChimeraMinigames.generated.h"

class AChimeraCharacter;
class UPrimitiveComponent;

UCLASS(Abstract)
class CHIMERA_API UChimeraMinigame : public UObject
{
	GENERATED_BODY()

public:
	virtual void Begin(AChimeraCharacter* InstigatorChar);
	virtual void Tick(float DeltaTime) {}
	virtual void Draw(class UCanvas* Canvas, float W, float H) {}
	virtual void HandleKey(const FKey& Key) {}
	virtual void End();

	UPROPERTY() FString Title = TEXT("Minigame");
	UPROPERTY() FString Status;
	UPROPERTY() bool bComplete = false;
	UPROPERTY() bool bConsumesMovement = true;
	UPROPERTY() EMinigameKind Kind = EMinigameKind::COUNT;

protected:
	UPROPERTY() TObjectPtr<AChimeraCharacter> Char;

	void Finish(bool bSuccess, int32 Score);
};

// GDD 13.1 - Breach Protocol: grid code matrix, chain daemons, timed.
UCLASS()
class CHIMERA_API UBreachProtocolMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	TArray<FString> Grid;
	int32 W = 6, H = 6;
	int32 CursorX = 0, CursorY = 0;
	TArray<FString> Sequence; // codes to enter in order
	int32 SeqIndex = 0;
	float TimeLeft = 30.f;
};

// GDD 13.2 - Surgery precision QTE suite.
UCLASS()
class CHIMERA_API USurgeryMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	int32 Step = 0;
	float Needle = 0.f;
	float NeedleDir = 1.f;
	float Target = 0.f;
	float GreenSize = 0.3f;
	float Vitals = 100.f;
	FString OperationName;
};

// GDD 13.4 - F1 hot lap time trial (player drives; finish line is a volume).
UCLASS()
class CHIMERA_API URacingMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;

	UPROPERTY() TObjectPtr<UPrimitiveComponent> FinishLine;
	float TargetLap = 45.f;

private:
	float Countdown = 3.f;
	bool bGo = false;
	float LapTime = 0.f;
	bool bFinished = false;
};

// GDD 13.10 - Lockpicking timing bar.
UCLASS()
class CHIMERA_API ULockpickMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	int32 Pin = 0;
	float Needle = 0.f;
	float NeedleDir = 1.f;
	float SweetSpot = 0.f;
	float SweetSize = 0.2f;
	int32 Damage = 0;
};

// GDD 13.3 - Interrogation with evidence drop + good/bad cop.
UCLASS()
class CHIMERA_API UInterrogationMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	int32 CorrectEvidence = 0;
	int32 Attempts = 4;
	int32 Cursor = 0;
	TArray<FString> Evidence;
};

// GDD 13.9 - Braindance: scrub edited memory layers to find the hidden trace.
UCLASS()
class CHIMERA_API UBraindanceMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	TArray<FString> Layers; // memory layers: visual / audio / tactile
	int32 Cursor = 0;
	int32 Focus = 0;   // scrub depth 0..4
	float Mismatch = 0.f; // grows while away from the trace
	int32 Decoded = 0; // number of edits found
	FString Trace;
};

// GDD 13.7 - Dance battle: timing on a beat grid (GDD 6.6 musical numbers).
UCLASS()
class CHIMERA_API UDanceMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	FString Moves[4] = { TEXT("LEFT"), TEXT("RIGHT"), TEXT("UP"), TEXT("DOWN") };
	int32 MoveIndex = 0;
	int32 Cursor = 0; // beat position
	float Beat = 0.f;
	float BeatInterval = 0.6f;
	int32 Combo = 0;
	int32 BestCombo = 0;
	int32 Targets = 4;
};

// GDD 6.6 / 13.8 - Football shootout: aim + timing, keeper dives late.
UCLASS()
class CHIMERA_API UFootballShootoutMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	FString Corners[4] = { TEXT("TL"), TEXT("TR"), TEXT("BL"), TEXT("BR") };
	int32 Aim = 0;
	int32 Keeper = 0;
	float Power = 0.f; // 0..1 by holding
	bool bCharging = false;
	int32 Kicks = 0;
	int32 Scored = 0;
	int32 TotalKicks = 3;
};

// GDD 13.10 - Fishing minigame (cast, hook, reel, tension).
UCLASS()
class CHIMERA_API UFishingMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	int32 Phase = 0;       // 0=cast, 1=wait, 2=hook, 3=reel, 4=done
	float CastPower = 0.f;
	float WaitTimer = 0.f;
	float HookTimer = 0.f; // remaining reaction window
	bool bCharging = false;
	float Tension = 0.f;   // 0..100; 100 = line snaps
	float Progress = 0.f;  // 0..100; 100 = fish caught
	bool bReeling = false;
};

// GDD 13.9 - Texas Hold'em Poker minigame (vs. 1 AI opponent).
UCLASS()
class CHIMERA_API UPokerMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	void Deal();
	TArray<int32> Deck;  // shuffled 0..51
	int32 PlayerCard1, PlayerCard2, AICard1, AICard2;
	TArray<int32> Community; // 5 cards
	int32 Pot = 0;
	int32 PlayerBet = 0;  // chips bet this round
	int32 AIBet = 0;
	int32 Phase = 0;      // 0=preflop, 1=flop, 2=turn, 3=river, 4=showdown
	int32 Cursor = 0;     // menu cursor 0=bet, 1=fold, 2=check/call
	bool bPlayerFolded = false;
	FString ResultMsg;
	int32 PlayerChips = 500;
	int32 AIChips = 500;
};

// GDD 13.10 - Drone Racing (FPV checkpoint gauntlet).
UCLASS()
class CHIMERA_API UDroneRacingMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	struct FGate { float X, Y; };
	TArray<FGate> Gates;
	int32 GateIdx = 0;
	float DroneX = 0.f, DroneY = 0.f; // position in the 2D space
	float SpeedX = 0.f, SpeedY = 0.f;
	float Boost = 1.f;                // 1..3 multiplier on speed
	float BoostCooldown = 0.f;
	float Timer = 0.f;
	float BestTime = TNumericLimits<float>::Max();
};

// GDD 4.x — Divergent aptitude test (5-question personality assessment).
UCLASS()
class CHIMERA_API UAptitudeTestMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	struct FQ { FString Text; TArray<FString> Options; TArray<int32> Scores; };
	TArray<FQ> Questions;
	int32 QIdx = 0;
	int32 Cursor = 0;
	TArray<int32> FactionVotes; // 0=Abnegation,1=Amity,2=Candor,3=Dauntless,4=Erudite
	bool bDone = false;
	FString ResultFaction;
};

// GDD 13.6 - Magical Dueling: turn-based spell combat, rock-paper-scissors counters.
UCLASS()
class CHIMERA_API UMagicDuelMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	int32 PlayerHP = 100; int32 EnemyHP = 100;
	int32 PlayerMana = 60; int32 EnemyMana = 60;
	int32 Cursor = 0; int32 EnemyChoice = 0;
	FString PlayerAction; FString EnemyAction; FString ResultMsg;
	struct FSpell { FString Name; int32 Dmg; int32 Mana; FString Type; FString Flavor; };
	TArray<FSpell> Spells;
};

// GDD 13.10 — Pickpocketing: approach target, timed grab in shrinking safe zone.
UCLASS()
class CHIMERA_API UPickpocketMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	float SafeZone = 60.f;  // shrinks over time
	float ZonePhase = 0.f;  // oscillates position
	float Timer = 0.f;
	int32 Attempts = 3;
	bool bGrabbing = false;
};

// GDD 13.10 — Safe Cracking: rotate dial, listen for clicks, find 3 numbers.
UCLASS()
class CHIMERA_API USafeCrackMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	int32 TargetNums[3];     // 0..99 each
	int32 CurrentNum = 0;   // current dial position
	int32 FoundCount = 0;   // how many found
	float DialSpeed = 0.f;
	float TimeLeft = 30.f;
};

// GDD 13.6 — Enchanting: draw runes with cursor precision, select enchantments.
UCLASS()
class CHIMERA_API UEnchantingMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	TArray<FVector2D> Runepoints;   // target points to trace
	TArray<bool> PointsHit;
	int32 RuneIdx = 0;              // which rune
	float CursorX = 0, CursorY = 0; // drawing cursor
	float Precision = 100.f;        // accumulated precision
	float TimeLeft = 20.f;
	bool bDrawing = false;
};

// GDD 13.8 — Cooking: chop, stir, flip, season, plate with rhythm mechanics.
UCLASS()
class CHIMERA_API UCookingMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	int32 Phase = 0;       // 0=chop,1=stir,2=flip,3=season,4=plate,5=done
	float PhaseTimer = 0.f;
	float Score = 0.f;
	int32 ChopCount = 0;
	int32 ChopTarget = 8;
	float StirSpeed = 0.f;
	float StirTarget = 0.5f;
	bool bFlipReady = false;
	float FlipTimer = 0.f;
	int32 SeasonLevel = 0;
	int32 PlatePosition = 0;
};

// GDD 13.8 — Blacksmithing: smelt, pour, hammer, quench, sharpen.
UCLASS()
class CHIMERA_API UBlacksmithingMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	int32 Phase = 0; float Score = 0; float Timer = 0.f;
	float Temp = 0.f; float TargetTemp = 85.f;
	float PourAngle = 0.f;
	int32 HammerPos = 0; int32 HammerTarget = 0;
	float QuenchTimer = 0.f; bool bQuenching = false;
	int32 SharpenAngle = 45;
};

// GDD 13.9 — Blackjack.
UCLASS()
class CHIMERA_API UBlackjackMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	TArray<int32> Deck; int32 DeckPos = 0;
	int32 PlayerTotal = 0; int32 DealerTotal = 0;
	int32 PlayerAces = 0; int32 DealerAces = 0;
	int32 Bet = 100; int32 Chips = 1000;
	int32 Phase = 0; // 0=betting,1=playing,2=dealer,3=result
	int32 Cursor = 0;
};

// GDD 13.9 — Roulette.
UCLASS()
class CHIMERA_API URouletteMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	int32 Result = 0; int32 Bet = 100; int32 OnNumber = 0;
	FString BetType = TEXT("number"); // number, red, black, even, odd
	int32 Chips = 1000; int32 Cursor = 0; bool bSpinning = false; float SpinTimer = 0.f;
};

// GDD 13.9 — Slot Machines.
UCLASS()
class CHIMERA_API USlotsMinigame : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;
private:
	int32 Reels[3]; int32 Chips = 1000; int32 Bet = 50;
	bool bSpinning = false; float SpinTimer = 0.f;
};

// GDD 5.1 - Character creation (life path, fears, gender, orientation, attributes).
UCLASS()
class CHIMERA_API UCharacterCreationFlow : public UChimeraMinigame
{
	GENERATED_BODY()
public:
	virtual void Begin(AChimeraCharacter* C) override;
	virtual void Draw(class UCanvas* Canvas, float W, float H) override;
	virtual void HandleKey(const FKey& Key) override;

private:
	enum { EPhase_Story, EPhase_LifePath, EPhase_Gender, EPhase_Orientation, EPhase_Fears, EPhase_Attributes, EPhase_Review };
	int32 Phase = EPhase_LifePath;
	int32 Cursor = 0;
	int32 Points = 15;
	int32 FearCursor = 0;
	TArray<FString> LifePaths;
	TArray<FString> GenderOptions;
	TArray<FString> OrientationOptions;
	TArray<FString> FearOptions;
	TArray<FString> ChosenFears;
	// Story intro
	int32 StoryLine = 0;
	TArray<FString> StoryText;
};

// Factory.
namespace FChimeraMinigameFactory
{
	CHIMERA_API UChimeraMinigame* Create(EMinigameKind Kind, UObject* Outer);
}
