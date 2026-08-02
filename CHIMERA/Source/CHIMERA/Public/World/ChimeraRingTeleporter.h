// GDD 1.2 - Ring Transit System. Overlap a gate to fast-travel between rings;
// every ring is physically connected, this is just the fast way.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTypes.h"
#include "GameFramework/Actor.h"
#include "ChimeraRingTeleporter.generated.h"

UCLASS()
class CHIMERA_API ARingTeleporter : public AActor
{
	GENERATED_BODY()

public:
	ARingTeleporter();
	UPROPERTY(EditAnywhere) ERing Destination = ERing::AetherCity;
	UPROPERTY(EditAnywhere) FString Label;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY() TObjectPtr<class UBoxComponent> Box;
	UPROPERTY() TObjectPtr<class UStaticMeshComponent> Gate;
};
