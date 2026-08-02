// GDD 14.19 - Advanced NPC AI: cognition (needs/personality), memory tiers,
// schedules, and dialogue generation from personality + memory + relationship.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "GameFramework/Character.h"
#include "ChimeraNPC.generated.h"

class AChimeraCharacter;

UCLASS()
class CHIMERA_API ANPCObject : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCObject();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere) FString NpcName;
	UPROPERTY(EditAnywhere) FString Role; // doctor / cop / dragon / criminal / citizen
	UPROPERTY() FPersonality Personality;
	UPROPERTY() FNeedState Needs;
	UPROPERTY() TArray<FMemoryEntry> Memory; // long-term tier (GDD 14.19.1)
	UPROPERTY() FString DailySchedule = TEXT("wander the district");

	// GDD 14.19.2 - dialogue from state.
	FString GetGreeting(class AChimeraCharacter* Player);
	FString GetNameTag() const { return FString::Printf(TEXT("%s (%s)"), *NpcName, *Role); }
	void Remember(const FString& Text, bool bImportant = false);
	void OnPlayerWrongedByPlayer(const FString& How);
	void OnPlayerHelpedPlayer(const FString& How);

	// GDD 14.19.1 - needs tick in this method.
	void TickNeeds(float DeltaTime);

private:
	FVector WanderTarget;
	float WanderTimer = 0.f;
	float NeuralTimer = 0.f; // GDD 14.19 — neural AI inference interval
	TObjectPtr<class UStaticMeshComponent> BodyMesh;
};
