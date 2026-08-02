// GDD 4 — Procedural building construction. Replaces Grey Box Zone with architecturally
// detailed structures using UE5 Starter Content + Quixel Megascans materials.
#include "World/ChimeraWorldBuilders.h"
#include "Core/ChimeraArchitecture.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "CHIMERA.h"

// Load a mesh or fall back to basic shapes if Starter Content isn't downloaded yet.
static UStaticMesh* LoadMeshSafe(const TCHAR* Preferred, const TCHAR* Fallback)
{
	UStaticMesh* M = LoadObject<UStaticMesh>(nullptr, Preferred);
	return M ? M : LoadObject<UStaticMesh>(nullptr, Fallback);
}

// Apply material if available
static void ApplyMaterial(UStaticMeshComponent* Comp, const FString& LogicalMat)
{
	const TCHAR* MatPath = ChimeraMaterials::GetPath(LogicalMat);
	UMaterialInterface* Mat = LoadObject<UMaterialInterface>(nullptr, MatPath);
	if (Mat) Comp->SetMaterial(0, Mat);
}

// Spawn a wall section
static AStaticMeshActor* SpawnWall(UWorld* W, FVector Loc, FVector Scale, float Rotation, const FString& Material)
{
	AStaticMeshActor* A = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator(0, Rotation, 0));
	if (!A) return nullptr;
	UStaticMesh* M = LoadMeshSafe(ChimeraMeshes::StarterContent::Wall_400x300, ChimeraMeshes::Cube);
	A->GetStaticMeshComponent()->SetStaticMesh(M);
	A->SetActorScale3D(Scale);
	A->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
	A->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ApplyMaterial(A->GetStaticMeshComponent(), Material);
	return A;
}

// Spawn a floor slab
static AStaticMeshActor* SpawnFloor(UWorld* W, FVector Loc, FVector Scale, const FString& Material)
{
	AStaticMeshActor* A = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
	if (!A) return nullptr;
	UStaticMesh* M = LoadMeshSafe(ChimeraMeshes::StarterContent::Floor_400x400, ChimeraMeshes::Cube);
	A->GetStaticMeshComponent()->SetStaticMesh(M);
	A->SetActorScale3D(Scale);
	A->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
	ApplyMaterial(A->GetStaticMeshComponent(), Material);
	return A;
}

// Spawn a pillar
static AStaticMeshActor* SpawnPillar(UWorld* W, FVector Loc, float Height, const FString& Material)
{
	AStaticMeshActor* A = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
	if (!A) return nullptr;
	UStaticMesh* M = LoadMeshSafe(ChimeraMeshes::StarterContent::Pillar_Square, ChimeraMeshes::Cylinder);
	A->GetStaticMeshComponent()->SetStaticMesh(M);
	A->SetActorScale3D(FVector(0.5f, 0.5f, Height / 500.f));
	A->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
	ApplyMaterial(A->GetStaticMeshComponent(), Material);
	return A;
}

// === Main procedural building function ===
void AChimeraWorld::BuildProceduralStructure(UWorld* W, FVector Origin, const FBuildingSpec& Spec)
{
	if (!W) return;

	const float WallW = 400.f;  // StarterContent wall width
	const float WallH = Spec.WallHeight;
	const float FloorThickness = 40.f;
	const float TotalW = Spec.WidthSegments * WallW;
	const float TotalD = Spec.DepthSegments * WallW;
	const float HalfW = TotalW / 2.f;
	const float HalfD = TotalD / 2.f;

	// Foundation
	SpawnFloor(W, Origin + FVector(0, 0, 0), FVector(TotalW / 400.f, TotalD / 400.f, FloorThickness / 400.f), Spec.FloorMaterial);

	// For each floor
	for (int32 f = 0; f < Spec.Floors; ++f)
	{
		float BaseZ = FloorThickness + f * WallH;
		FVector FloorCenter = Origin + FVector(0, 0, BaseZ + WallH / 2.f);

		// Floor slab
		SpawnFloor(W, Origin + FVector(0, 0, BaseZ + WallH), FVector(TotalW / 400.f, TotalD / 400.f, FloorThickness / 400.f), Spec.FloorMaterial);

		// Front wall (facing +Y)
		for (int32 x = 0; x < Spec.WidthSegments; ++x)
		{
			FVector Loc = FloorCenter + FVector(-HalfW + (x + 0.5f) * WallW, HalfD, 0);
			// Door on ground floor middle segment
			if (f == 0 && x == Spec.WidthSegments / 2 && Spec.bHasDoor)
			{
				UStaticMesh* DoorM = LoadMeshSafe(ChimeraMeshes::StarterContent::Door_SM, ChimeraMeshes::Cube);
				AStaticMeshActor* Door = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
				if (Door) { Door->GetStaticMeshComponent()->SetStaticMesh(DoorM); Door->SetActorScale3D(FVector(1.5f, 1.5f, 2.f)); ApplyMaterial(Door->GetStaticMeshComponent(), TEXT("Wood")); }
				continue;
			}
			// Windows on upper floors, alternate segments
			if (f > 0 && Spec.bHasWindows && x % 2 == 0)
			{
				UStaticMesh* WinM = LoadMeshSafe(ChimeraMeshes::StarterContent::Window_SM, ChimeraMeshes::Cube);
				AStaticMeshActor* Window = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
				if (Window) { Window->GetStaticMeshComponent()->SetStaticMesh(WinM); Window->SetActorScale3D(FVector(1.5f, 0.3f, 1.5f)); ApplyMaterial(Window->GetStaticMeshComponent(), TEXT("Glass")); }
				continue;
			}
			SpawnWall(W, Loc, FVector(1.f, 0.3f, 1.f), 0.f, Spec.WallMaterial);
		}

		// Back wall (facing -Y)
		for (int32 x = 0; x < Spec.WidthSegments; ++x)
		{
			FVector Loc = FloorCenter + FVector(-HalfW + (x + 0.5f) * WallW, -HalfD, 0);
			if (f > 0 && Spec.bHasWindows && x % 2 == 1)
			{
				UStaticMesh* WinM = LoadMeshSafe(ChimeraMeshes::StarterContent::Window_SM, ChimeraMeshes::Cube);
				AStaticMeshActor* Window = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator(0, 180, 0));
				if (Window) { Window->GetStaticMeshComponent()->SetStaticMesh(WinM); Window->SetActorScale3D(FVector(1.5f, 0.3f, 1.5f)); ApplyMaterial(Window->GetStaticMeshComponent(), TEXT("Glass")); }
				continue;
			}
			SpawnWall(W, Loc, FVector(1.f, 0.3f, 1.f), 180.f, Spec.WallMaterial);
		}

		// Left wall (facing -X)
		for (int32 y = 0; y < Spec.DepthSegments; ++y)
		{
			FVector Loc = FloorCenter + FVector(-HalfW, -HalfD + (y + 0.5f) * WallW, 0);
			SpawnWall(W, Loc, FVector(1.f, 0.3f, 1.f), 90.f, Spec.WallMaterial);
		}

		// Right wall (facing +X)
		for (int32 y = 0; y < Spec.DepthSegments; ++y)
		{
			FVector Loc = FloorCenter + FVector(HalfW, -HalfD + (y + 0.5f) * WallW, 0);
			SpawnWall(W, Loc, FVector(1.f, 0.3f, 1.f), -90.f, Spec.WallMaterial);
		}

		// Corner pillars
		SpawnPillar(W, FloorCenter + FVector(-HalfW, -HalfD, 0), WallH, Spec.WallMaterial);
		SpawnPillar(W, FloorCenter + FVector(HalfW, -HalfD, 0), WallH, Spec.WallMaterial);
		SpawnPillar(W, FloorCenter + FVector(-HalfW, HalfD, 0), WallH, Spec.WallMaterial);
		SpawnPillar(W, FloorCenter + FVector(HalfW, HalfD, 0), WallH, Spec.WallMaterial);
	}

	// Roof
	if (Spec.bHasRoof)
	{
		float RoofZ = FloorThickness + Spec.Floors * WallH + 20.f;
		UStaticMesh* RoofM = Spec.Style == TEXT("Fantasy") || Spec.Style == TEXT("Medieval") ?
			LoadMeshSafe(ChimeraMeshes::StarterContent::Roof_Tile, ChimeraMeshes::Cone) :
			LoadMeshSafe(ChimeraMeshes::StarterContent::Floor_400x400, ChimeraMeshes::Cube);

		AStaticMeshActor* Roof = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),
			Origin + FVector(0, 0, RoofZ), FRotator::ZeroRotator);
		if (Roof)
		{
			Roof->GetStaticMeshComponent()->SetStaticMesh(RoofM);
			Roof->SetActorScale3D(FVector(TotalW / 400.f, TotalD / 400.f, 0.3f));
			Roof->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
			ApplyMaterial(Roof->GetStaticMeshComponent(), Spec.RoofMaterial);
		}
	}
}

// ======================================================================
// Ring-specific construction helpers
// ======================================================================
void AChimeraWorld::BuildHospital(UWorld* W, FVector Origin)
{
	BuildProceduralStructure(W, Origin, ChimeraArchStyle::MercyHospital());
}

void AChimeraWorld::BuildCastle(UWorld* W, FVector Origin)
{
	BuildProceduralStructure(W, Origin, ChimeraArchStyle::NeoKingdomCastle());
}

void AChimeraWorld::BuildHighrise(UWorld* W, FVector Origin)
{
	BuildProceduralStructure(W, Origin, ChimeraArchStyle::NightCityHighrise());
}

void AChimeraWorld::BuildPrecinct(UWorld* W, FVector Origin)
{
	BuildProceduralStructure(W, Origin, ChimeraArchStyle::MidWilshirePrecinct());
}

void AChimeraWorld::BuildShop(UWorld* W, FVector Origin)
{
	BuildProceduralStructure(W, Origin, ChimeraArchStyle::CleveleysShop());
}

void AChimeraWorld::BuildHouse(UWorld* W, FVector Origin)
{
	BuildProceduralStructure(W, Origin, ChimeraArchStyle::CheapHouse());
}
