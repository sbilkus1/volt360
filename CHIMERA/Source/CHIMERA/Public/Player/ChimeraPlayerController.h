// GDD 3 (loops), 13 (minigame input), 14.7 (pause/legacy). Owns the active
// minigame and routes raw keys to it; manages UI overlays and the pause menu.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "ChimeraPlayerController.generated.h"

class UChimeraMinigame;

UCLASS()
class CHIMERA_API AChimeraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	void StartMinigame(class UChimeraMinigame* M);
	void EndMinigame();

	UPROPERTY() TObjectPtr<UChimeraMinigame> ActiveMinigame;

	// UI overlay flags consumed by AChimeraHUD.
	bool bShowMap = false;
	bool bShowMarket = false;
	bool bShowPause = false;

	void ToggleMap() { bShowMap = !bShowMap; }
	void ToggleMarket() { bShowMarket = !bShowMarket; }
	void TogglePause() { bShowPause = !bShowPause; }

private:
	void RouteMinigameInput(const TArray<FKey>& Keys);
	void HandleMenuInput();
	void HandleMarketInput();
	void MaybeStartCreation();
};
