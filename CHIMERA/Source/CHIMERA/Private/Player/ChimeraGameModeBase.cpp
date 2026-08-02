#include "Player/ChimeraGameModeBase.h"
#include "Player/ChimeraPlayerController.h"
#include "Player/ChimeraHUD.h"
#include "Character/ChimeraCharacter.h"
#include "World/ChimeraWorldBuilders.h"
#include "Engine/World.h"
#include "CHIMERA.h"
#include "GameFramework/Controller.h"

AChimeraGameModeBase::AChimeraGameModeBase()
{
	// GDD 3.5/10 - single unified world; co-op hooks live in the session subsystem.
	DefaultPawnClass = AChimeraCharacter::StaticClass();
	PlayerControllerClass = AChimeraPlayerController::StaticClass();
	HUDClass = AChimeraHUD::StaticClass();
}

APawn* AChimeraGameModeBase::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
	// The world is built far above the template map's terrain (see WorldBuilders), so
	// spawn on the Nexus pad rather than trusting a template PlayerStart.
	const FVector SpawnLoc = FVector(0.f, 0.f, kChimeraWorldAltitude + 200.f);
	FActorSpawnParameters Params;
	Params.Owner = NewPlayer;
	return GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLoc, FRotator::ZeroRotator, Params);
}

void AChimeraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// GDD 1/4 - build Nexopolis (all 9 rings in one persistent world).
	GetWorld()->SpawnActor<AChimeraWorld>(AChimeraWorld::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
}
