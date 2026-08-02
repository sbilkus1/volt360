// GDD 6.5 / 13.4 — Formula 1 track database: 24 real circuits + custom CHIMERA tracks.
#pragma once

#include "CoreMinimal.h"
#include "ChimeraTracks.generated.h"

USTRUCT(BlueprintType)
struct CHIMERA_API FChimeraTrack
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() FString Location;
	UPROPERTY() FString Country;
	UPROPERTY() float LapKm = 5.f;       // length per lap
	UPROPERTY() int32 Corners = 16;       // number of turns
	UPROPERTY() int32 LapCount = 3;       // default race length
	UPROPERTY() float Difficulty = 0.5f;  // 0..1 for AI performance
	UPROPERTY() bool bReal = true;        // real F1 circuit or custom CHIMERA
};

namespace FChimeraTrackData
{
	CHIMERA_API const TArray<FChimeraTrack>& RealTracks();
	CHIMERA_API const TArray<FChimeraTrack>& CustomTracks();
	CHIMERA_API const FChimeraTrack* FindTrack(const FString& Name);
}
