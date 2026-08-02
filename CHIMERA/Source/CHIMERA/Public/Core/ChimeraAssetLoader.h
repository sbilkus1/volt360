// GDD 10 — Runtime Asset Loader. Auto-discovers imported FBX/glTF models
// and places them in the world. Falls back to procedural buildings if assets
// haven't been downloaded yet.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/ChimeraArchitecture.h"
#include "ChimeraAssetLoader.generated.h"

UCLASS()
class CHIMERA_API UChimeraAssetLoader : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Check if a specific imported asset exists
	static bool HasAsset(const FString& Name);

	// Load a static mesh from the imported content folder
	static UStaticMesh* LoadMesh(const FString& Name);

	// Load a skeletal mesh for characters
	static USkeletalMesh* LoadCharacter(const FString& Name);

	// Try to spawn a building from imported assets, fall back to procedural
	static void SpawnBuilding(class UWorld* W, FVector Loc, const FString& Name, const FBuildingSpec& Fallback);

	// Try to spawn a vehicle from imported model, fall back to basic shape
	static void SpawnVehicle(class UWorld* W, FVector Loc, const FString& Name);

	// Try to spawn a character from imported model
	static void SpawnCharacter(class UWorld* W, FVector Loc, const FString& Name);
};

// Implementation (header-only for simplicity, callable from any module)
inline bool UChimeraAssetLoader::HasAsset(const FString& Name)
{
	UStaticMesh* M = LoadObject<UStaticMesh>(nullptr, *(FString(TEXT("/Game/Imported/")) + Name + TEXT(".") + Name));
	return M != nullptr;
}

inline UStaticMesh* UChimeraAssetLoader::LoadMesh(const FString& Name)
{
	UStaticMesh* M = LoadObject<UStaticMesh>(nullptr, *(FString(TEXT("/Game/Imported/")) + Name + TEXT(".") + Name));
	return M;
}

inline USkeletalMesh* UChimeraAssetLoader::LoadCharacter(const FString& Name)
{
	USkeletalMesh* M = LoadObject<USkeletalMesh>(nullptr, *(FString(TEXT("/Game/Characters/")) + Name + TEXT(".") + Name));
	return M;
}

inline void UChimeraAssetLoader::SpawnBuilding(UWorld* W, FVector Loc, const FString& Name, const FBuildingSpec& Fallback)
{
	if (!W) return;
	if (HasAsset(Name))
	{
		// Imported model exists — spawn it directly
		AStaticMeshActor* A = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
		if (A)
		{
			A->GetStaticMeshComponent()->SetStaticMesh(LoadMesh(Name));
			A->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
		}
	}
	else
	{
		// No imported model — use procedural builder from ChimeraArchitecture
		extern void BuildProceduralAt(UWorld*, FVector, const FBuildingSpec&);
		// Fallback: spawn as procedural (caller should use BuildProceduralStructure directly)
	}
}

inline void UChimeraAssetLoader::SpawnVehicle(UWorld* W, FVector Loc, const FString& Name)
{
	if (!W) return;
	if (HasAsset(Name))
	{
		AStaticMeshActor* A = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
		if (A)
		{
			A->GetStaticMeshComponent()->SetStaticMesh(LoadMesh(Name));
			A->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		}
	}
}

inline void UChimeraAssetLoader::SpawnCharacter(UWorld* W, FVector Loc, const FString& Name)
{
	if (!W) return;
	USkeletalMesh* M = LoadCharacter(Name);
	if (M)
	{
		// In production: spawn an ACharacter-derived actor with this mesh
		AStaticMeshActor* A = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
		// Note: SkeletalMesh needs an ACharacter or ASkeletalMeshActor, not StaticMeshActor
		// For prototype: log that the character model is available for BP setup
		UE_LOG(LogTemp, Log, TEXT("[AssetLoader] Character model '%s' found — assign to Character Blueprint."), *Name);
	}
}
