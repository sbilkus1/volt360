// GDD 3.4 (HUD motivators), 9 (BAWSAQ), 12.9 (mission ratings), 14.18 (chronicle).
// Canvas-based HUD: no assets required. Renders the main overlay, minigames,
// map, market, and pause/legacy menus.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ChimeraHUD.generated.h"

UCLASS()
class CHIMERA_API AChimeraHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	void DrawMainHUD(float W, float H);
	void DrawMinigame(float W, float H);
	void DrawMap(float W, float H);
	void DrawMarket(float W, float H);
	void DrawPause(float W, float H);
	void DrawBar(float X, float Y, float W, float H, float Pct, FLinearColor Color);
};
