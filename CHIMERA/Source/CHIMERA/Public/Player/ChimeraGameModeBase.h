// GDD 3 (session bootstrap), 1 (world). Game mode: pawn/controller/HUD wiring and
// spawns the procedurally-built world (Nexus hub + ring zones).
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChimeraGameModeBase.generated.h"

UCLASS()
class CHIMERA_API AChimeraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChimeraGameModeBase();
	virtual void BeginPlay() override;

	// Guarantee a safe spawn at the Nexus hub regardless of template PlayerStarts.
	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;
};
