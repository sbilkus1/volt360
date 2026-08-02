// GDD 6.1/13/4 - the interactable. Every object in the world (terminals, beds,
// crates, drones, NPC chat) is one of these, so the whole district catalog maps
// onto a single interaction model.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "GameFramework/Actor.h"
#include "ChimeraInteractable.generated.h"

class AChimeraCharacter;
class ANPCObject;

UCLASS()
class CHIMERA_API AChimeraInteractable : public AActor
{
	GENERATED_BODY()

public:
	AChimeraInteractable();
	virtual void OnInteract(AChimeraCharacter* C);
	void TakeHit(AChimeraCharacter* C, float Dmg);

	UPROPERTY(EditAnywhere) FString Prompt = TEXT("interact");
	UPROPERTY(EditAnywhere) EMinigameKind Minigame = EMinigameKind::COUNT;
	UPROPERTY(EditAnywhere) FString Message;
	UPROPERTY(EditAnywhere) FName StatKey;
	UPROPERTY(EditAnywhere) bool bIsDrone = false;
	UPROPERTY(EditAnywhere) int32 DroneHealth = 100;
	UPROPERTY(EditAnywhere) TObjectPtr<ANPCObject> OwnerNpc;

	// Optional overlap volume (e.g., F1 finish line for the racing minigame).
	UPROPERTY(VisibleAnywhere) TObjectPtr<class UBoxComponent> Trigger;
	UPROPERTY(VisibleAnywhere) TObjectPtr<class UStaticMeshComponent> Mesh;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
