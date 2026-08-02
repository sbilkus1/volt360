#include "Player/ChimeraPlayerController.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraEconomy.h"
#include "Minigames/ChimeraMinigames.h"
#include "Character/ChimeraCharacter.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void AChimeraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// GDD 5.1 - open character creation on first boot.
	FTimerHandle Tmp;
	GetWorldTimerManager().SetTimer(Tmp, this, &AChimeraPlayerController::MaybeStartCreation, 0.2f, false);
}

void AChimeraPlayerController::MaybeStartCreation()
{
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess && !Sess->bCharacterCreated && GetPawn())
	{
		StartMinigame(FChimeraMinigameFactory::Create(EMinigameKind::CharacterCreation, this));
	}
}

void AChimeraPlayerController::StartMinigame(UChimeraMinigame* M)
{
	if (!M) return;
	if (ActiveMinigame)
	{
		ActiveMinigame->End();
	}
	ActiveMinigame = M;
	M->Begin(Cast<AChimeraCharacter>(GetPawn()));
	// Pause the world for focused minigames; PlayerTick still runs while paused so
	// key polling below keeps working. Racing keeps the world live.
	SetPause(M->bConsumesMovement);
}

void AChimeraPlayerController::EndMinigame()
{
	bool bWasCreation = ActiveMinigame && ActiveMinigame->Kind == EMinigameKind::CharacterCreation;
	if (ActiveMinigame)
	{
		ActiveMinigame->End();
		ActiveMinigame = nullptr;
		SetPause(false);
	}
	// If creation was cancelled (Esc), bring it back so the game always has a character.
	if (bWasCreation)
	{
		UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (Sess && !Sess->bCharacterCreated && GetPawn())
		{
			FTimerHandle Tmp;
			GetWorldTimerManager().SetTimer(Tmp, this, &AChimeraPlayerController::MaybeStartCreation, 0.3f, false);
		}
	}
}

void AChimeraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if (ActiveMinigame)
	{
		ActiveMinigame->Tick(DeltaTime);
		// GDD 13 - raw key polling keeps input trivial and remappable.
		static const TArray<FKey> Keys = {
			EKeys::W, EKeys::A, EKeys::S, EKeys::D,
			EKeys::Up, EKeys::Down, EKeys::Left, EKeys::Right,
			EKeys::SpaceBar, EKeys::Enter, EKeys::Escape,
			EKeys::E, EKeys::R, EKeys::F, EKeys::C
		};
		RouteMinigameInput(Keys);
		if (ActiveMinigame && ActiveMinigame->bComplete)
		{
			EndMinigame();
		}
		return;
	}

	HandleMenuInput();
	HandleMarketInput();
}

void AChimeraPlayerController::RouteMinigameInput(const TArray<FKey>& Keys)
{
	for (const FKey& K : Keys)
	{
		if (WasInputKeyJustPressed(K))
		{
			if (K == EKeys::Escape)
			{
				EndMinigame();
			}
			else if (ActiveMinigame)
			{
				ActiveMinigame->HandleKey(K);
			}
		}
	}
}

void AChimeraPlayerController::HandleMenuInput()
{
	if (WasInputKeyJustPressed(EKeys::Tab))
	{
		if (!bShowPause) ToggleMarket();
	}
	if (WasInputKeyJustPressed(EKeys::M))
	{
		if (!bShowPause) ToggleMap();
	}
	if (WasInputKeyJustPressed(EKeys::P) || (bShowPause && WasInputKeyJustPressed(EKeys::Escape)))
	{
		TogglePause();
		SetPause(bShowPause);
	}
	// GDD 3.5 - quick save/load.
	if (WasInputKeyJustPressed(EKeys::F5))
	{
		UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (Sess && Sess->SaveGame(TEXT("quicksave")))
		{
			Sess->ShowMessage(TEXT("Game saved."));
		}
	}
	if (WasInputKeyJustPressed(EKeys::F9))
	{
		UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (Sess && Sess->LoadGame(TEXT("quicksave")))
		{
			RestartLevel();
		}
	}
	if (bShowPause && WasInputKeyJustPressed(EKeys::R))
	{
		// GDD 3.6 - Legacy System: retire, heir inherits world state.
		UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (Sess)
		{
			Sess->RetireCharacter();
			Sess->StartHeir();
			bShowPause = false;
			SetPause(false);
			RestartLevel();
		}
	}
}

void AChimeraPlayerController::HandleMarketInput()
{
	if (!bShowMarket || bShowPause) return;
	UBAWSAQSubsystem* BAWSAQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>();
	if (!BAWSAQ) return;
	const TArray<FStockDef>& Stocks = BAWSAQ->GetStocks();
	static int32 Cursor = 0;
	if (WasInputKeyJustPressed(EKeys::W)) Cursor = FMath::Max(0, Cursor - 1);
	if (WasInputKeyJustPressed(EKeys::S)) Cursor = FMath::Min(Stocks.Num() - 1, Cursor + 1);
	if (WasInputKeyJustPressed(EKeys::Enter)) BAWSAQ->BuyStock(Cursor, 1);
	if (WasInputKeyJustPressed(EKeys::Tab)) ToggleMarket();
}
