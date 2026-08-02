#include "World/ChimeraWorldBuilders.h"
#include "World/ChimeraInteractable.h"
#include "Core/ChimeraArchitecture.h"
#include "Core/ChimeraGlobe.h"
#include "Core/ChimeraAssetLoader.h"
#include "World/ChimeraRingTeleporter.h"
#include "AI/ChimeraNPC.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CHIMERA.h"

AStaticMeshActor* AChimeraWorld::PlaceMesh(UWorld* W, const FString& MeshPath, FVector Loc, FVector Scale)
{
	if (!W) return nullptr;
	AStaticMeshActor* A = W->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Loc, FRotator::ZeroRotator);
	if (!A) return nullptr;
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (Mesh)
	{
		A->GetStaticMeshComponent()->SetStaticMesh(Mesh);
		A->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
		A->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	A->SetActorScale3D(Scale);
	return A;
}

AChimeraInteractable* AChimeraWorld::PlaceInteractable(UWorld* W, FVector Loc, FVector Scale,
	const FString& Prompt, EMinigameKind Kind, const FString& Message, FName StatKey, bool bDrone)
{
	if (!W) return nullptr;
	AChimeraInteractable* I = W->SpawnActor<AChimeraInteractable>(AChimeraInteractable::StaticClass(), Loc, FRotator::ZeroRotator);
	if (!I) return nullptr;
	I->SetActorScale3D(Scale);
	I->Prompt = Prompt;
	I->Minigame = Kind;
	I->Message = Message;
	I->StatKey = StatKey;
	I->bIsDrone = bDrone;
	if (bDrone) I->DroneHealth = 60;
	return I;
}

ARingTeleporter* AChimeraWorld::PlaceGate(UWorld* W, FVector Loc, ERing Dest, const FString& Label)
{
	if (!W) return nullptr;
	ARingTeleporter* G = W->SpawnActor<ARingTeleporter>(ARingTeleporter::StaticClass(), Loc, FRotator::ZeroRotator);
	if (G)
	{
		G->Destination = Dest;
		G->Label = Label;
	}
	return G;
}

ANPCObject* AChimeraWorld::SpawnNpc(UWorld* W, FVector Loc, const FString& Name, const FString& Role)
{
	if (!W) return nullptr;
	ANPCObject* N = W->SpawnActor<ANPCObject>(ANPCObject::StaticClass(), Loc, FRotator::ZeroRotator);
	if (N)
	{
		N->NpcName = Name;
		N->Role = Role;
	}
	return N;
}

void AChimeraWorld::BeginPlay()
{
	Super::BeginPlay();
	UWorld* W = GetWorld();
	if (!W) return;

	// GDD 2 — Sky, lighting, post-processing.
	// On first UE5 compile, spawn a directional light and sky sphere for proper rendering.
	// In production: Lumen + path tracing handle all lighting dynamically.
	// For prototype: spawn a basic sun + sky.
	if (UClass* SkyClass = LoadClass<AActor>(nullptr, ChimeraMeshes::SkySphere))
	{ W->SpawnActor<AActor>(SkyClass, FVector(0,0,kChimeraWorldAltitude), FRotator::ZeroRotator); }
	// Directional light (sun)
	FActorSpawnParameters Params;
	Params.Name = TEXT("Sun");
	W->SpawnActor<AActor>(AActor::StaticClass(), FVector(0,0,kChimeraWorldAltitude + 5000), FRotator(-45,30,0), Params);

	BuildHub(W);
	const TArray<FRingDef>& Rings = FChimeraData::Rings();
	for (int32 i = 1; i < Rings.Num(); ++i)
	{
		BuildZone(W, Rings[i]);
	}
}

void AChimeraWorld::BuildHub(UWorld* W)
{
	const float Alt = kChimeraWorldAltitude;
	// GDD 1.1 / 4.1 - Ring 0: The Nexus.
	PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), FVector(0, 0, -100) + FVector(0, 0, Alt), FVector(80, 80, 1));
	// GDD 4 — Load real Nexus Tower if the user ran DownloadAssets.ps1, else fallback cylinder.
	if (UChimeraAssetLoader::HasAsset(TEXT("Nexus_Tower")))
		UChimeraAssetLoader::SpawnBuilding(W, FVector(0, 0, 700) + FVector(0, 0, Alt), TEXT("Nexus_Tower"), ChimeraArchStyle::NexusHub());
	else
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), FVector(0, 0, 700) + FVector(0, 0, Alt), FVector(6, 6, 22));
	PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), FVector(0, 0, 130) + FVector(0, 0, Alt), FVector(12, 12, 0.6f)); // plaza ring

	// GDD 12.2 M3 - the Keeper.
	ANPCObject* Keeper = SpawnNpc(W, FVector(400, 0, 100) + FVector(0, 0, Alt), TEXT("Keeper Elyon"), TEXT("Keeper of the Nexus"));
	AChimeraInteractable* KeeperTalk = PlaceInteractable(W, FVector(400, 0, 100) + FVector(0, 0, Alt), FVector(1.5f, 1.5f, 1.5f),
		TEXT("Talk to the Keeper"), EMinigameKind::COUNT, TEXT(""), FName("talk_keeper"), false);
	KeeperTalk->OwnerNpc = Keeper;

	// GDD 1.2 — Ring transit stations. Fast-travel terminals for instant ring-to-ring
	// travel. Physical roads, rivers, and mountain passes connect all regions too —
	// you can walk, drive, or fly between any two regions seamlessly.
	// Each station faces its destination on the continent.
	const TArray<FRingDef>& Rings = FChimeraData::Rings();
	const int32 NumRings = Rings.Num() - 1;
	for (int32 i = 1; i < Rings.Num(); ++i)
	{
		float Angle = (i - 1) * (360.f / NumRings);
		FVector Loc(2000.f * FMath::Cos(FMath::DegreesToRadians(Angle)),
			2000.f * FMath::Sin(FMath::DegreesToRadians(Angle)), 0.f);
		Loc.Z += Alt;
		PlaceGate(W, Loc, (ERing)i, FString::Printf(TEXT("%s — fast-travel to %s"), *Rings[i].Name, *Rings[i].Genre));
	}

	// GDD 13 - practice interactables in the hub.
	PlaceInteractable(W, FVector(-600, 600, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Breach Terminal (hacking)"),
		EMinigameKind::BreachProtocol, TEXT(""), FName(), false);
	PlaceInteractable(W, FVector(-600, -600, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("ER Intake (surgery)"),
		EMinigameKind::Surgery, TEXT(""), FName(), false);
	PlaceInteractable(W, FVector(600, 600, 0) + FVector(0, 0, Alt), FVector(1.5f, 1.5f, 1.5f), TEXT("Locked crate (lockpick)"),
		EMinigameKind::Lockpicking, TEXT(""), FName(), false);
	PlaceInteractable(W, FVector(600, -600, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Library of Lost Worlds"),
		EMinigameKind::COUNT, TEXT("All 11 realities, remembered. The Keepers catalogue the Crash here."), FName(), false);
	PlaceInteractable(W, FVector(0, 1400, 0) + FVector(0, 0, Alt), FVector(2, 1, 2), TEXT("RTS Terminal"),
		EMinigameKind::COUNT, TEXT("Hyperloop: 2-minute loadless travel to every ring. Or just walk to a gate."), FName(), false);

	// GDD 9.3 - real estate agent.
	{
		AChimeraInteractable* Agent = PlaceInteractable(W, FVector(-1400, 0, 0) + FVector(0, 0, Alt), FVector(2, 1.5f, 2),
			TEXT("Property Agent"), EMinigameKind::COUNT, TEXT(""), FName("property"), false);
		Agent->Message = TEXT("Property agent: buy a safehouse for passive income. Sell to recoup half.");
		// GDD 9.4 - insurance office.
		AChimeraInteractable* Ins = PlaceInteractable(W, FVector(1400, 0, 0) + FVector(0, 0, Alt), FVector(2, 1.5f, 2),
			TEXT("Insurance Office"), EMinigameKind::COUNT, TEXT(""), FName("insurance"), false);
		Ins->Message = TEXT("Insurance office: premiums cover hospital, vehicle, and dragon bills. Premiums are charged hourly.");
		// GDD 14.3 - companion adoption center.
		AChimeraInteractable* Adoption = PlaceInteractable(W, FVector(0, 1400, 0) + FVector(0, 0, Alt), FVector(2, 1.5f, 2),
			TEXT("Adoption Center"), EMinigameKind::COUNT, TEXT(""), FName("adopt"), false);
		Adoption->Message = TEXT("Adoption center: pets from every ring, rescued since the Crash.");
		// GDD 14.12 — Nexus Tower Depths dungeon entrance.
		AChimeraInteractable* DungeonGate = PlaceInteractable(W, FVector(0, -2000, 0) + FVector(0, 0, Alt), FVector(3, 3, 3),
			TEXT("Nexus Tower Depths — endless dungeon (Floor 1-500)"), EMinigameKind::COUNT, TEXT(""), FName("dungeon"), false);
		DungeonGate->Message = TEXT("The Tower extends downward. Reality fractures below. How deep can you go?");
		// GDD 12 — Grand Rebellion war room (all rings choose sides).
		AChimeraInteractable* WarRoom = PlaceInteractable(W, FVector(2000, 0, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("War Room — Grand Rebellion HQ"), EMinigameKind::COUNT, TEXT(""), FName("rebellion"), false);
		WarRoom->Message = TEXT("War Room: every ring, every faction, every character — choose your side.");
		// GDD 4.x — TARDIS landing site.
		AChimeraInteractable* TARDIS = PlaceInteractable(W, FVector(-2000, 0, 0) + FVector(0, 0, Alt), FVector(2, 1.5f, 3),
			TEXT("TARDIS — the Doctor's blue box"), EMinigameKind::COUNT, TEXT(""), FName("tardis"), false);
		TARDIS->Message = TEXT("The TARDIS. Bigger on the inside. Choose your Doctor. Customise your sonic. Explore infinite rooms.");
		AChimeraInteractable* TARDISExplore = PlaceInteractable(W, FVector(-2000, 200, 0) + FVector(0, 0, Alt), FVector(1.5f, 1.5f, 1.5f),
			TEXT("TARDIS interior — explore"), EMinigameKind::COUNT, TEXT(""), FName("tardis_explore"), false);
		AChimeraInteractable* TARDISSonic = PlaceInteractable(W, FVector(-2000, -200, 0) + FVector(0, 0, Alt), FVector(1.5f, 1.5f, 1.5f),
			TEXT("Sonic screwdriver workbench"), EMinigameKind::COUNT, TEXT(""), FName("tardis_sonic"), false);
		// GDD 6.5 — Vehicle stations.
		AChimeraInteractable* Dealer = PlaceInteractable(W, FVector(2000, 2000, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Premium Motors — vehicle dealership"), EMinigameKind::COUNT, TEXT(""), FName("dealership"), false);
		Dealer->Message = TEXT("Premium Motors: every manufacturer since 1980. Buy, sell, insure. Credit checks welcome.");
		AChimeraInteractable* Chop = PlaceInteractable(W, FVector(-2000, 2000, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Sal's Chop Shop — sell vehicles"), EMinigameKind::COUNT, TEXT(""), FName("chop_shop"), false);
		AChimeraInteractable* Garage = PlaceInteractable(W, FVector(2000, -2000, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Custom Auto — upgrade garage"), EMinigameKind::COUNT, TEXT(""), FName("vehicle_upgrade"), false);
		AChimeraInteractable* Theft = PlaceInteractable(W, FVector(-2000, -2000, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Dark alley — steal a parked car"), EMinigameKind::COUNT, TEXT(""), FName("car_theft"), false);
		// GDD 14.2 — Base building terminal + business office.
		AChimeraInteractable* BaseTerm = PlaceInteractable(W, FVector(0, 2000, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Base Building Terminal — purchase property"), EMinigameKind::COUNT, TEXT(""), FName("base_buy"), false);
		AChimeraInteractable* BizOff = PlaceInteractable(W, FVector(1400, 1400, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Business License Office — start a business"), EMinigameKind::COUNT, TEXT(""), FName("business_buy"), false);
		AChimeraInteractable* Kitchen = PlaceInteractable(W, FVector(-1400, 1400, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Keepers' Kitchen — cook a dish"), EMinigameKind::Cooking, TEXT(""), FName(), false);
		AChimeraInteractable* DateSpot = PlaceInteractable(W, FVector(0, 2500, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Nexus garden gazebo — romantic date"), EMinigameKind::COUNT, TEXT(""), FName("date"), false);
		AChimeraInteractable* Supply = PlaceInteractable(W, FVector(2500, 0, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Inter-Ring Trade Terminal — supply chain"), EMinigameKind::COUNT, TEXT(""), FName("supply_chain"), false);
		AChimeraInteractable* BarnFind = PlaceInteractable(W, FVector(-2500, 0, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Barn Find Map — locate hidden legendary vehicles"), EMinigameKind::COUNT, TEXT(""), FName("barn_find"), false);
		AChimeraInteractable* AIDiff = PlaceInteractable(W, FVector(0, -2500, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("AI Difficulty Terminal — configure NPC intelligence"), EMinigameKind::COUNT, TEXT(""), FName("ai_difficulty"), false);
		AChimeraInteractable* Tutorial = PlaceInteractable(W, FVector(-600, -600, 0) + FVector(0, 0, Alt), FVector(2, 2, 2),
			TEXT("Keeper's Guide — new player tips"), EMinigameKind::COUNT, TEXT(""), FName("tutorial"), false);
		AChimeraInteractable* Finale = PlaceInteractable(W, FVector(0, 0, 400) + FVector(0, 0, Alt), FVector(3, 3, 3),
			TEXT("The Convergence Altar — choose the fate of all realities"), EMinigameKind::COUNT, TEXT(""), FName("finale"), false);
		// Phone, Radio, Clothing, Gang, Collectibles, Furniture
		PlaceInteractable(W, FVector(800, -800, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("HoloPhone — messages / social feed"), EMinigameKind::COUNT, TEXT(""), FName("phone"), false);
		PlaceInteractable(W, FVector(-800, -800, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Radio Tower — 8 stations"), EMinigameKind::COUNT, TEXT(""), FName("radio"), false);
		PlaceInteractable(W, FVector(800, 800, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Nexopolis Outfitters — buy clothes"), EMinigameKind::COUNT, TEXT(""), FName("clothing"), false);
		PlaceInteractable(W, FVector(-800, 800, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Turf Map — gang warfare"), EMinigameKind::COUNT, TEXT(""), FName("gang_attack"), false);
		PlaceInteractable(W, FVector(1200, 0, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Scavenger's Log — 100 collectibles"), EMinigameKind::COUNT, TEXT(""), FName("collectible"), false);
		PlaceInteractable(W, FVector(-1200, 0, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("IKEA Nexopolis — property furniture"), EMinigameKind::COUNT, TEXT(""), FName("furniture"), false);
		// GDD 9 — Trading Post + Blueprint Crafting.
		PlaceInteractable(W, FVector(0, 1200, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Trading Post — buy/sell hero pieces"), EMinigameKind::COUNT, TEXT(""), FName("trade"), false);
		PlaceInteractable(W, FVector(300, 1200, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Piece Market — buy individual blueprint pieces"), EMinigameKind::COUNT, TEXT(""), FName("piece_buy"), false);
		PlaceInteractable(W, FVector(600, 1200, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Forge Station — craft hero blueprint"), EMinigameKind::COUNT, TEXT(""), FName("craft_blueprint"), false);
		// Weapon forge and armory
		PlaceInteractable(W, FVector(0, -1200, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Armory — craft legendary weapons"), EMinigameKind::COUNT, TEXT(""), FName("weapon_craft"), false);
		PlaceInteractable(W, FVector(300, -1200, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Weapon Rack — equip your arsenal"), EMinigameKind::COUNT, TEXT(""), FName("weapon_equip"), false);
		PlaceInteractable(W, FVector(600, -1200, 0) + FVector(0, 0, Alt), FVector(2, 2, 2), TEXT("Gunsmith Bench — install weapon mods"), EMinigameKind::COUNT, TEXT(""), FName("weapon_mod"), false);
	}
}

void AChimeraWorld::BuildZone(UWorld* W, const FRingDef& Ring)
{
	FVector A = Ring.ZoneAnchor + FVector(0, 0, kChimeraWorldAltitude);
	PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 0, -100), FVector(30, 30, 1));
	// Themed landmarks.
	PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 0, 300), FVector(6, 6, 8));
	PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(600, 300, 150), FVector(2, 2, 3));
	PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(-600, -300, 150), FVector(2, 2, 3));

	// Return gate to the Nexus.
	PlaceGate(W, A + FVector(0, 1800, 0), ERing::Nexus, TEXT("Back to the Nexus"));

	// GDD 14.19 - district citizens.
	const TArray<FString>& Names = FChimeraData::NpcNames();
	ANPCObject* N1 = SpawnNpc(W, A + FVector(400, 400, 100), Names[NpcNameCursor % Names.Num()], FString::Printf(TEXT("%s citizen"), *Ring.Name));
	NpcNameCursor++;
	ANPCObject* N2 = SpawnNpc(W, A + FVector(-400, -400, 100), Names[NpcNameCursor % Names.Num()], FString::Printf(TEXT("%s citizen"), *Ring.Name));
	NpcNameCursor++;
	if (N1) N1->DailySchedule = FString::Printf(TEXT("patrol %s"), *Ring.Name);
	if (N2) N2->DailySchedule = FString::Printf(TEXT("trade in %s"), *Ring.Name);

	// GDD 4.x - district signature content.
	switch (Ring.Ring)
	{
	case ERing::AetherCity:
		// GDD 4.2 - kaiju-bait training drones (M6 boss adds).
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(1, 1, 2), TEXT("Drone - practice target"),
			EMinigameKind::COUNT, TEXT(""), FName(), true);
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(1, 1, 2), TEXT("Drone - practice target"),
			EMinigameKind::COUNT, TEXT(""), FName(), true);
		PlaceInteractable(W, A + FVector(0, -800, 0), FVector(1, 1, 2), TEXT("Drone - practice target"),
			EMinigameKind::COUNT, TEXT(""), FName(), true);
		// GDD 13.7 - dance battle stage (musical numbers).
		PlaceInteractable(W, A + FVector(0, 1200, 0), FVector(2, 2, 2), TEXT("Dance stage (battle)"),
			EMinigameKind::Dance, TEXT(""), FName(), false);
		// GDD 13.10 - sky drone racing circuit.
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Sky Race Gate (drone racing)"),
			EMinigameKind::DroneRacing, TEXT(""), FName(), false);
		// GDD 14.10 - exploration vista.
		PlaceInteractable(W, A + FVector(0, 1500, 0), FVector(1, 1, 1), TEXT("Sky platform vista (explore)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// GDD 14 — Tony Stark's Armor Forge. Craft every Iron Man armor.
		PlaceInteractable(W, A + FVector(-400, 1500, 0), FVector(2, 2, 2), TEXT("Stark Armor Forge — craft Iron Man suits"),
			EMinigameKind::COUNT, TEXT(""), FName("iron_man_armor"), false);
		break;
	case ERing::MercyHeights:
		// GDD 4.4 - surgery.
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("ER - perform surgery"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(0, 1200, 0), FVector(2, 2, 2), TEXT("The Forgotten Ward — medical horror dungeon"),
			EMinigameKind::COUNT, TEXT(""), FName("dungeon"), false);
		break;
	case ERing::MidWilshire:
		// GDD 4.5 - NPD Mid-Wilshire Station (The Rookie base).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 400, 300), FVector(8, 4, 6));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(300, 0, 300), FVector(1, 1, 8));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, -400, 100), FVector(4, 4, 2));
		// Roll call room, front desk, evidence.
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Interrogation room"),
			EMinigameKind::Interrogation, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Locked evidence locker"),
			EMinigameKind::Lockpicking, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(0, 1200, 0), FVector(2, 2, 2), TEXT("Bounty Board (NPD)"),
			EMinigameKind::COUNT, TEXT(""), FName("bounty"), false);
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Backroom poker table"),
			EMinigameKind::Poker, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(0, 1500, 0), FVector(2, 2, 2), TEXT("Heist Planning Board"),
			EMinigameKind::COUNT, TEXT(""), FName("heist"), false);
		// Mid-Wilshire market street.
		PlaceInteractable(W, A + FVector(1200, 400, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Mid-Wilshire street market (explore)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// GDD 6.7 — Police procedural stations.
		PlaceInteractable(W, A + FVector(0, 1700, 0), FVector(2, 2, 2), TEXT("Case board — open a new investigation"),
			EMinigameKind::COUNT, TEXT(""), FName("police_case"), false);
		PlaceInteractable(W, A + FVector(400, 1700, 0), FVector(2, 2, 2), TEXT("Evidence locker — collect field evidence"),
			EMinigameKind::COUNT, TEXT(""), FName("evidence"), false);
		PlaceInteractable(W, A + FVector(800, 1700, 0), FVector(2, 2, 2), TEXT("Arrest warrant office — make an arrest"),
			EMinigameKind::COUNT, TEXT(""), FName("arrest"), false);
		// GDD 6.7 — Criminal justice system.
		PlaceInteractable(W, A + FVector(-800, 1700, 0), FVector(2, 2, 2), TEXT("Courtroom — stand trial"),
			EMinigameKind::COUNT, TEXT(""), FName("visit_court"), false);
		PlaceInteractable(W, A + FVector(-400, 1700, 0), FVector(2, 2, 2), TEXT("Bail bondsman — post bail"),
			EMinigameKind::COUNT, TEXT(""), FName("bail"), false);
		PlaceInteractable(W, A + FVector(-1200, 1700, 0), FVector(2, 2, 2), TEXT("Holding cell — surrender to police"),
			EMinigameKind::COUNT, TEXT(""), FName("surrender"), false);
		// Prison interactables (accesible when sentenced)
		PlaceInteractable(W, A + FVector(-1200, 1900, 0), FVector(2, 2, 2), TEXT("Prison yard — work detail / plan escape / stir up riot"),
			EMinigameKind::COUNT, TEXT(""), FName("prison_job"), false);
		PlaceInteractable(W, A + FVector(-400, 1700, 0), FVector(2, 2, 2), TEXT("Police Box — TARDIS patrol mode (activate lights)"),
			EMinigameKind::COUNT, TEXT(""), FName("tardis_police"), false);
		// GDD 13.10 — pickpocketing spot.
		PlaceInteractable(W, A + FVector(1200, 800, 0), FVector(2, 2, 2), TEXT("Crowded market — pick a pocket"),
			EMinigameKind::Pickpocket, TEXT(""), FName(), false);
		// GDD 13.10 — safe cracking challenge.
		PlaceInteractable(W, A + FVector(-1200, 1800, 0), FVector(2, 2, 2), TEXT("Locked evidence vault — crack the safe"),
			EMinigameKind::SafeCrack, TEXT(""), FName(), false);
		// GDD 13.10 — photography vista.
		PlaceInteractable(W, A + FVector(1500, -200, 0), FVector(1, 1, 1), TEXT("Hollywood sign overlook — take a photo"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// GDD 13.7 — date night spots.
		PlaceInteractable(W, A + FVector(-1500, -200, 0), FVector(2, 2, 2), TEXT("Ring-side bistro — go on a date"),
			EMinigameKind::COUNT, TEXT(""), FName("date"), false);
		// NPD characters (The Rookie).
		SpawnNpc(W, A + FVector(300, 400, 100), TEXT("John Nolan"), TEXT("NPD Training Officer"));
		SpawnNpc(W, A + FVector(-300, 400, 100), TEXT("Lucy Chen"), TEXT("NPD Officer"));
		SpawnNpc(W, A + FVector(0, -300, 100), TEXT("Tim Bradford"), TEXT("NPD Sergeant"));
		SpawnNpc(W, A + FVector(400, -200, 100), TEXT("Angela Lopez"), TEXT("NPD Detective"));
		SpawnNpc(W, A + FVector(-400, -200, 100), TEXT("Nyla Harper"), TEXT("NPD Detective"));
		SpawnNpc(W, A + FVector(0, 500, 100), TEXT("Wade Grey"), TEXT("Watch Commander"));
		SpawnNpc(W, A + FVector(600, 0, 100), TEXT("Talia Bishop"), TEXT("NPD Training Officer (former)"));
		SpawnNpc(W, A + FVector(-600, 0, 100), TEXT("Jackson West"), TEXT("NPD Officer, son of IA commander"));
		SpawnNpc(W, A + FVector(500, 200, 100), TEXT("Celina Juarez"), TEXT("NPD Officer, aura reader, Nolan's rookie"));
		SpawnNpc(W, A + FVector(-500, 200, 100), TEXT("Aaron Thorsen"), TEXT("NPD Officer, reality TV star turned cop"));
		SpawnNpc(W, A + FVector(0, -500, 100), TEXT("Zoe Andersen"), TEXT("NPD Captain (deceased), Nolan's first captain"));
		SpawnNpc(W, A + FVector(200, -500, 100), TEXT("Rosalind Dyer"), TEXT("Serial killer, nemesis of the precinct"));
		SpawnNpc(W, A + FVector(-200, -500, 100), TEXT("Elijah Stone"), TEXT("Crime boss, April's nemesis"));
		SpawnNpc(W, A + FVector(300, 600, 100), TEXT("Wesley Evers"), TEXT("Public defender, Angela's husband"));
		SpawnNpc(W, A + FVector(-300, 600, 100), TEXT("Bailey Nune"), TEXT("Firefighter, Nolan's wife"));
		break;
	case ERing::Alagaesia:
		// GDD 4.6 - Farthen Dûr (dwarf city beneath the mountain).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(0, 0, 800), FVector(12, 12, 20));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 0, 900), FVector(8, 8, 3));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(600, 0, 800), FVector(3, 3, 15));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(-600, 0, 800), FVector(3, 3, 15));
		SpawnNpc(W, A + FVector(200, 300, 100), TEXT("Orik"), TEXT("Dwarf King, 43rd in line to the throne"));
		SpawnNpc(W, A + FVector(-200, 300, 100), TEXT("Hrothgar"), TEXT("Dwarf King of Tronjheim"));
		// Ellesméra (elf forest capital).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(2000, 0, 400), FVector(3, 3, 25));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(2000, 500, 300), FVector(2, 2, 6));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(2000, -500, 300), FVector(2, 2, 6));
		SpawnNpc(W, A + FVector(2000, 200, 100), TEXT("Oromis"), TEXT("Golden elf Rider, keeper of the ancient lore"));
		SpawnNpc(W, A + FVector(2000, -200, 100), TEXT("Glaedr"), TEXT("Golden dragon, Oromis's bonded partner"));
		SpawnNpc(W, A + FVector(2200, 0, 100), TEXT("Islanzadi"), TEXT("Elf Queen of Ellesméra"));
		SpawnNpc(W, A + FVector(1800, 0, 100), TEXT("Arya"), TEXT("Elf princess, guardian of Saphira's egg"));
		// Dras-Leona (walled city, Helgrind in the distance).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(-2000, 0, 400), FVector(6, 6, 5));
		SpawnNpc(W, A + FVector(-2000, 300, 100), TEXT("Murtagh"), TEXT("Dragon Rider, son of Morzan, rider of Thorn"));
		// Teirm (port city).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 2500, 200), FVector(5, 1, 3));
		SpawnNpc(W, A + FVector(0, 2500, 100), TEXT("Jeod"), TEXT("Scholar and merchant of Teirm"));
		// Core Alagaesia characters.
		SpawnNpc(W, A + FVector(500, 100, 100), TEXT("Eragon"), TEXT("Dragon Rider, last free Rider of Alagaesia"));
		SpawnNpc(W, A + FVector(-500, 100, 100), TEXT("Brom"), TEXT("Former Rider, storyteller and mentor"));
		SpawnNpc(W, A + FVector(0, -300, 100), TEXT("Roran"), TEXT("Hammer-wielding warrior of Carvahall"));
		SpawnNpc(W, A + FVector(300, -400, 100), TEXT("Nasuada"), TEXT("Leader of the Varden"));
		SpawnNpc(W, A + FVector(-300, -400, 100), TEXT("Angela"), TEXT("Herbalist, werecat companion, reads the bones"));
		// The dragon egg (original).
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Dragon egg"),
			EMinigameKind::COUNT, TEXT(""), FName("dragon_egg"), false);
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(1, 1, 1), TEXT("Fire fern patch (harvest)"),
			EMinigameKind::COUNT, TEXT(""), FName("harvest"), false);
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Dragon perch (bond)"),
			EMinigameKind::COUNT, TEXT(""), FName("dragon_bond"), false);
		PlaceInteractable(W, A + FVector(0, -800, 0), FVector(2, 2, 2), TEXT("Hunting grounds"),
			EMinigameKind::COUNT, TEXT(""), FName("hunt"), false);
		PlaceInteractable(W, A + FVector(0, -1500, 0), FVector(2, 1, 2), TEXT("Angwar River (fishing)"),
			EMinigameKind::Fishing, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(400, 1200, 0), FVector(1, 1, 1), TEXT("Ancient dwarven ruin (explore)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// Urû'baen vista (north of the zone).
		PlaceInteractable(W, A + FVector(0, 3500, 0), FVector(1, 1, 1), TEXT("View of Urû'baen citadel (explore)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(0, 3000, 0), FVector(2, 2, 2), TEXT("Dragon Graveyard — elder dragon dungeon"),
			EMinigameKind::COUNT, TEXT(""), FName("dungeon"), false);
		// GDD 13.8 — Blacksmithing forge.
		PlaceInteractable(W, A + FVector(-400, 1700, 0), FVector(2, 2, 2), TEXT("Dwarven forge — craft a weapon"),
			EMinigameKind::Blacksmithing, TEXT(""), FName(), false);
		break;
	case ERing::MinecraftFrontier:
		// GDD 4.7 - building plot.
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Building plot (place blocks)"),
			EMinigameKind::COUNT, TEXT("The Frontier is voxel: every block is destructible and placeable."), FName("build"), false);
		// GDD 6.4 - voxel crops.
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(1, 1, 1), TEXT("Voxel wheat field (harvest)"),
			EMinigameKind::COUNT, TEXT(""), FName("harvest"), false);
		// GDD 13.10 - lake fishing in the Frontier.
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 1, 2), TEXT("Blocky Lake (fishing)"),
			EMinigameKind::Fishing, TEXT(""), FName(), false);
		// GDD 13.10 - cave drone races.
		PlaceInteractable(W, A + FVector(0, -1200, 0), FVector(2, 2, 2), TEXT("Voxel Cave (drone racing)"),
			EMinigameKind::DroneRacing, TEXT(""), FName(), false);
		// GDD 14.10 - underground exploration.
		PlaceInteractable(W, A + FVector(-400, 1200, 0), FVector(1, 1, 1), TEXT("Deep mine shaft (explore)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(400, -800, 0), FVector(2, 2, 2), TEXT("Deep earth — underground expedition"),
			EMinigameKind::COUNT, TEXT(""), FName("underground"), false);
		break;
	case ERing::GrandColiseum:
		// GDD 4.8 - F1 hot lap. Start gate + finish trigger at the end of the circuit.
		{
			AChimeraInteractable* Race = PlaceInteractable(W, A + FVector(0, 900, 0), FVector(2, 2, 2),
				TEXT("Garage - hot lap"), EMinigameKind::Racing, TEXT(""), FName(), false);
			if (Race)
			{
				Race->Trigger->SetRelativeLocation(FVector(0, -1800, 0));
				Race->Trigger->SetBoxExtent(FVector(500, 60, 100));
				Race->Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Race->Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			}
			// Track markers for the lap circuit.
			PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 0, 20), FVector(36, 1, 0.2f));
		}
		// GDD 13.8 - penalty shootout pitch.
		PlaceInteractable(W, A + FVector(0, 1200, 0), FVector(2, 2, 2), TEXT("Penalty pitch (shootout)"),
			EMinigameKind::FootballShootout, TEXT(""), FName(), false);
		// F1 track selector — choose from 39 circuits.
		{
			AChimeraInteractable* TrackSel = PlaceInteractable(W, A + FVector(0, 1500, 0), FVector(2, 2, 2),
				TEXT("F1 Circuit Selector — hot lap"), EMinigameKind::Racing, TEXT(""), FName(), false);
			if (TrackSel)
			{
				TrackSel->Trigger->SetRelativeLocation(FVector(0, -1800, 0));
				TrackSel->Trigger->SetBoxExtent(FVector(500, 60, 100));
				TrackSel->Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				TrackSel->Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			}
		}
		PlaceInteractable(W, A + FVector(1800, 0, 0), FVector(2, 2, 2), TEXT("Stadium of Champions — boss rush dungeon"),
			EMinigameKind::COUNT, TEXT(""), FName("dungeon"), false);
		break;
	case ERing::NeoKingdom:
		// GDD 4.3 - potion brewing (spell creation).
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Potion brewing station"),
			EMinigameKind::COUNT, TEXT("Physics-based brewing. Perfect execution = Masterwork."), FName("craft"), false);
		// GDD 13.10 - courtyard pond (kingdom fishing).
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 1, 2), TEXT("Courtyard Pond (fishing)"),
			EMinigameKind::Fishing, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(400, 0, 0), FVector(2, 2, 2), TEXT("Dueling chamber — magical combat"),
			EMinigameKind::MagicDuel, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(800, 400, 0), FVector(2, 2, 2), TEXT("Enchanting table — inscribe your gear"),
			EMinigameKind::Enchanting, TEXT(""), FName(), false);
		// GDD 14.10 - royal garden exploration.
		PlaceInteractable(W, A + FVector(400, 1200, 0), FVector(1, 1, 1), TEXT("Royal Garden ruins (explore)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		break;
	case ERing::NightCity:
		// GDD 4.9 - netrunning terminal.
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Net terminal (breach)"),
			EMinigameKind::BreachProtocol, TEXT(""), FName(), false);
		// GDD 13.9 - braindance wreath.
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(2, 2, 2), TEXT("Braindance wreath (scrub memory)"),
			EMinigameKind::Braindance, TEXT(""), FName(), false);
		// GDD 13.9 - poker at the Afterlife Casino.
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Afterlife Casino (Texas Hold'em)"),
			EMinigameKind::Poker, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(-600, 300, 0), FVector(2, 2, 2), TEXT("Afterlife Casino — Blackjack"),
			EMinigameKind::Blackjack, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(-600, -300, 0), FVector(2, 2, 2), TEXT("Afterlife Casino — Roulette"),
			EMinigameKind::Roulette, TEXT(""), FName(), false);
		// GDD 13.10 - dock fishing in the Expanse.
		PlaceInteractable(W, A + FVector(0, -1200, 0), FVector(2, 1, 2), TEXT("Expanse Docks (fishing)"),
			EMinigameKind::Fishing, TEXT(""), FName(), false);
		break;
	// --- Cinematic universe expansion zones ---
	case ERing::Seattle:
		// Grey-Sloan Memorial Hospital (Grey's Anatomy).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 300, 400), FVector(10, 6, 10));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 300, 900), FVector(6, 4, 2));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(400, 0, 300), FVector(3, 10, 6));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(-400, 0, 300), FVector(2, 2, 10));
		// OR board, ER bay, clinic.
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Grey-Sloan Memorial - ER intake"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(2, 2, 2), TEXT("Grey-Sloan - OR board"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Grey-Sloan - Clinic (free care)"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(0, 1500, 0), FVector(1, 1, 1), TEXT("The catwalk — where surgeons go to think"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(0, 1800, 0), FVector(2, 2, 2), TEXT("Flight 212 crash memorial — relive the crash"),
			EMinigameKind::COUNT, TEXT(""), FName("plane_crash"), false);
		PlaceInteractable(W, A + FVector(-300, 1200, 0), FVector(2, 2, 2), TEXT("Grey-Sloan LGBTQIA+ transition clinic"),
			EMinigameKind::COUNT, TEXT(""), FName("transition"), false);
		// Add Lexie, Mark, Arizona, Callie (plane crash survivors and victims).
		SpawnNpc(W, A + FVector(0, 1800, 100), TEXT("Lexie Grey"), TEXT("Surgical resident. Photographic memory. Loved Mark."));
		SpawnNpc(W, A + FVector(100, 1800, 100), TEXT("Mark Sloan"), TEXT("Chief of Plastics. McSteamy. Sofia's father."));
		SpawnNpc(W, A + FVector(-100, 1800, 100), TEXT("Arizona Robbins"), TEXT("Pediatric surgeon. Lost a leg. Kept her smile."));
		SpawnNpc(W, A + FVector(200, 1800, 100), TEXT("Callie Torres"), TEXT("Orthopaedic surgeon. Built Calliope. Loves Arizona."));
		// Grey's Anatomy characters.
		SpawnNpc(W, A + FVector(200, 300, 100), TEXT("Meredith Grey"), TEXT("Chief of General Surgery"));
		SpawnNpc(W, A + FVector(-200, 300, 100), TEXT("Miranda Bailey"), TEXT("Residency Director"));
		SpawnNpc(W, A + FVector(400, 100, 100), TEXT("Richard Webber"), TEXT("Former Chief, mentor to all"));
		SpawnNpc(W, A + FVector(-400, 100, 100), TEXT("Amelia Shepherd"), TEXT("Head of Neurosurgery"));
		SpawnNpc(W, A + FVector(0, -200, 100), TEXT("Owen Hunt"), TEXT("Chief of Trauma Surgery"));
		SpawnNpc(W, A + FVector(300, -300, 100), TEXT("Jackson Avery"), TEXT("Chief of Plastics, Harper Avery legacy"));
		SpawnNpc(W, A + FVector(-300, -300, 100), TEXT("April Kepner"), TEXT("Trauma surgeon, OR logistics"));
		SpawnNpc(W, A + FVector(500, 0, 100), TEXT("Addison Montgomery"), TEXT("Neonatal / OB/GYN surgeon"));
		break;
	case ERing::Chicago:
		// Gaffney Chicago Medical Center (Chicago Med).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 300, 400), FVector(10, 6, 10));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, -400, 100), FVector(4, 4, 2));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(400, 400, 300), FVector(1, 1, 12));
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Gaffney Medical Center - Emergency Dept"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(2, 2, 2), TEXT("Gaffney - Psychiatry wing (Dr. Charles)"),
			EMinigameKind::Interrogation, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Gaffney - ICU isolation ward"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(0, -1200, 0), FVector(1, 1, 1), TEXT("Goodwin's corner office with the skyline view"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// Chicago Med characters.
		SpawnNpc(W, A + FVector(200, 300, 100), TEXT("Will Halstead"), TEXT("ED attending, ex-plastic surgeon"));
		SpawnNpc(W, A + FVector(-200, 300, 100), TEXT("Natalie Manning"), TEXT("ED pediatrician"));
		SpawnNpc(W, A + FVector(400, 100, 100), TEXT("Dr. Charles"), TEXT("Chief of Psychiatry, the wise counsellor"));
		SpawnNpc(W, A + FVector(-400, 100, 100), TEXT("Ethan Choi"), TEXT("Chief of the ED"));
		SpawnNpc(W, A + FVector(0, -200, 100), TEXT("April Sexton"), TEXT("Charge nurse, runs the ED floor"));
		SpawnNpc(W, A + FVector(300, -300, 100), TEXT("Maggie Lockwood"), TEXT("Senior nurse, heart of the programme"));
		SpawnNpc(W, A + FVector(-300, -300, 100), TEXT("Sharon Goodwin"), TEXT("CEO, ex-nurse, the moral compass"));
		SpawnNpc(W, A + FVector(500, -100, 100), TEXT("Dean Archer"), TEXT("Trauma surgeon, old-school but brilliant"));
		// ---- Divergent series: post-war Chicago, the faction system ----
		// The Hub / Choosing Ceremony hall.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(2500, 0, 400), FVector(10, 12, 8));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(2500, 0, 850), FVector(6, 8, 2));
		PlaceInteractable(W, A + FVector(2500, 0, 0), FVector(2, 2, 2), TEXT("The Choosing Ceremony hall"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// Dauntless compound — The Pit.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(2500, 1000, 500), FVector(5, 5, 16));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(2500, 1000, 200), FVector(6, 6, 4));
		PlaceInteractable(W, A + FVector(2500, 1000, 0), FVector(2, 2, 2), TEXT("The Pit — Dauntless combat training"),
			EMinigameKind::COUNT, TEXT(""), FName("tribute_train"), false);
		PlaceInteractable(W, A + FVector(2500, 800, 0), FVector(2, 2, 2), TEXT("The Pit — initiation ranked fight"),
			EMinigameKind::COUNT, TEXT(""), FName("initiate_fight"), false);
		PlaceInteractable(W, A + FVector(2500, 600, 0), FVector(2, 2, 2), TEXT("The Hub — aptitude test (choose your faction)"),
			EMinigameKind::AptitudeTest, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(2500, 1200, 0), FVector(2, 2, 2), TEXT("The Pit — tattoo parlour (mark your faction)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(2500, 200, 0), FVector(2, 2, 2), TEXT("Fear landscape simulation — confront your fears"),
			EMinigameKind::COUNT, TEXT(""), FName("fear_landscape"), false);
		PlaceInteractable(W, A + FVector(2500, 1400, 0), FVector(1, 1, 1), TEXT("The Ferris wheel — a test of nerve"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// Erudite headquarters.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(2500, -1000, 500), FVector(8, 8, 12));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(2500, -1000, 900), FVector(2, 2, 6));
		PlaceInteractable(W, A + FVector(2500, -1000, 0), FVector(2, 2, 2), TEXT("Erudite HQ — serum laboratory"),
			EMinigameKind::COUNT, TEXT(""), FName("craft"), false);
		// Amity farms.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(3500, 0, 80), FVector(12, 6, 1));
		PlaceInteractable(W, A + FVector(3500, 0, 0), FVector(2, 1, 2), TEXT("Amity orchards (harvest)"),
			EMinigameKind::COUNT, TEXT(""), FName("harvest"), false);
		// Abnegation sector.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(1500, 1000, 100), FVector(4, 4, 2));
		SpawnNpc(W, A + FVector(1500, 1000, 100), TEXT("Andrew Prior"), TEXT("Abnegation council member. Father to Tris and Caleb."));
		// The Wall / fence — boundary of Chicago.
		PlaceInteractable(W, A + FVector(3500, 1500, 0), FVector(1, 1, 1), TEXT("The Wall — beyond lies the unknown world"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// Divergent characters.
		SpawnNpc(W, A + FVector(2500, 800, 100), TEXT("Tris Prior"), TEXT("Divergent. Dauntless initiate. The one who broke the system."));
		SpawnNpc(W, A + FVector(2500, 700, 100), TEXT("Four (Tobias Eaton)"), TEXT("Dauntless instructor. Four fears. More than his father's son."));
		SpawnNpc(W, A + FVector(2600, 900, 100), TEXT("Christina"), TEXT("Dauntless initiate, Candor-born, Tris's first ally"));
		SpawnNpc(W, A + FVector(2400, 900, 100), TEXT("Will"), TEXT("Erudite-born Dauntless transfer. Kind-hearted. Christina's other half."));
		SpawnNpc(W, A + FVector(2500, 1100, 100), TEXT("Eric"), TEXT("Dauntless leader. Cruel. Erudite sympathiser. Five fears, all ugly."));
		SpawnNpc(W, A + FVector(2500, -800, 100), TEXT("Jeanine Matthews"), TEXT("Erudite leader. The serum architect. Believes logic must rule."));
		SpawnNpc(W, A + FVector(2500, -1100, 100), TEXT("Caleb Prior"), TEXT("Erudite transfer. Tris's brother. Chose knowledge over family."));
		SpawnNpc(W, A + FVector(2400, 800, 100), TEXT("Tori Wu"), TEXT("Dauntless tattoo artist. Administered Tris's aptitude test."));
		SpawnNpc(W, A + FVector(2700, 1000, 100), TEXT("Uriah"), TEXT("Dauntless-born. Loves the zip line. Pure nerve."));
		SpawnNpc(W, A + FVector(2300, 1000, 100), TEXT("Zeke"), TEXT("Dauntless-born. Uriah's brother. Expert climber, quick wit."));
		SpawnNpc(W, A + FVector(2600, -900, 100), TEXT("Peter"), TEXT("Dauntless initiate. Coward wrapped in cruelty. Stabbed the nightlock."));
		// Aptitude test — fear landscape simulation (StatKey handled in interactable).
		PlaceInteractable(W, A + FVector(2500, 500, 0), FVector(2, 2, 2), TEXT("Aptitude test — face your fears (simulation)"),
			EMinigameKind::Braindance, TEXT(""), FName(), false);
		break;
	case ERing::ChastainPark:
		// Chastain Park Memorial Hospital, Atlanta (The Resident).
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 300, 400), FVector(10, 6, 10));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, -300, 200), FVector(6, 6, 4));
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Chastain Park - OR suite (the Raptor's theatre)"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(2, 2, 2), TEXT("Chastain Park - ER trauma bay"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Chastain Park - Admin wing (Kit Voss)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// The Resident characters.
		SpawnNpc(W, A + FVector(200, 300, 100), TEXT("Conrad Hawkins"), TEXT("Senior resident, cowboy medicine"));
		SpawnNpc(W, A + FVector(-200, 300, 100), TEXT("Nic Nevin"), TEXT("Nurse practitioner, Conrad's partner"));
		SpawnNpc(W, A + FVector(400, 100, 100), TEXT("Randolph Bell"), TEXT("Chief of Surgery, redemption arc"));
		SpawnNpc(W, A + FVector(-400, 100, 100), TEXT("Mina Okafor"), TEXT("Cardio-thoracic prodigy, the Raptor's protégé"));
		SpawnNpc(W, A + FVector(0, -200, 100), TEXT("AJ Austin"), TEXT("The Raptor – cardio-thoracic surgeon, triple-board"));
		SpawnNpc(W, A + FVector(300, -300, 100), TEXT("Kit Voss"), TEXT("Head of orthopaedics, later CEO"));
		SpawnNpc(W, A + FVector(-300, -300, 100), TEXT("Devon Pravesh"), TEXT("Idealistic intern, Conrad's rookie partner"));
		break;
	case ERing::Cleveleys:
		// Cleveleys, Lancashire, UK — 53°52′52″N 3°02′24″W. Population 10,754.
		// Postcode FY5. 4mi north of Blackpool, 2mi south of Fleetwood.
		// Blackpool Tramway, Victoria Road pubs, Star Wars Andor filming location.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 600, 150), FVector(2, 18, 3));  // promenade
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(0, 400, 600), FVector(3, 3, 16));  // tram tower
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, -800, 200), FVector(6, 3, 4)); // Fleetwood docks
		// Victoria Road West — the main street
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 800, 0), FVector(16, 1, 0.3f));
		// Pubs (real, from Wikipedia)
		PlaceInteractable(W, A + FVector(-400, 820, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("The Victoria Hotel — Cleveleys pub since Victorian era"),
			EMinigameKind::COUNT, TEXT(""), FName("date"), false);
		PlaceInteractable(W, A + FVector(0, 820, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("The Shipwreck Brewhouse — craft beer, sea views"),
			EMinigameKind::COUNT, TEXT(""), FName("craft"), false);
		PlaceInteractable(W, A + FVector(400, 820, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("The Jolly Tars — proper Lancashire pub"),
			EMinigameKind::COUNT, TEXT(""), FName("date"), false);
		// Blackpool Tramway stops (5 stops in Cleveleys area)
		PlaceInteractable(W, A + FVector(-500, 500, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Cleveleys Tram Stop — Blackpool Tramway to Fleetwood & Starr Gate"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// Star Wars Andor filming location (planet Niamos, May 2021)
		PlaceInteractable(W, A + FVector(500, 600, 0), FVector(1, 1, 1), TEXT("Niamos beach — Star Wars: Andor filming location"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// === Victoria Road West / Beach Road — the High Street ===
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 800, 0), FVector(16, 1, 0.3f)); // pavement
		// Shop row (Beach Road, 60-75)
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(-300, 800, 100), FVector(2, 2, 2));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(-100, 800, 100), FVector(2, 2, 2));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(100, 800, 100), FVector(2, 2, 2));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(300, 800, 100), FVector(2, 2, 2));
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(500, 800, 100), FVector(2, 2, 2));
		// UBU Supported Living — 67-69 Beach Road. Mental health recovery flats.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(200, 800, 400), FVector(3, 3, 10)); // building
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(200, 800, 250), FVector(4, 4, 5));
		AChimeraInteractable* UBU = PlaceInteractable(W, A + FVector(200, 800, 0), FVector(2, 2, 2),
			TEXT("UBU Supported Living — 67-69 Beach Road"), EMinigameKind::COUNT, TEXT(""), FName("base_buy"), false);
		UBU->Message = TEXT("UBU & Partners Foundation: 8 flats, 24/7 support. 'Treat a person as they could be and they become as they should be.' Flat 6 has a view of the Irish Sea.");
		AChimeraInteractable* Flat6 = PlaceInteractable(W, A + FVector(200, 900, 0), FVector(1.5f, 1.5f, 1.5f),
			TEXT("Flat 6, UBU — your home by the sea"), EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		Flat6->Message = TEXT("Flat 6: your own space. The window faces the Irish Sea. The tide goes in and out. You're safe here. You're home.");
		// Beach Road shops
		PlaceInteractable(W, A + FVector(-300, 850, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("The Beach Café — full English and a cuppa"),
			EMinigameKind::Cooking, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(-100, 850, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Cleveleys Bookshop — second-hand paperbacks"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(100, 850, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Bay Gift Shop — rock, sticks of rock, and postcards"),
			EMinigameKind::COUNT, TEXT(""), FName("craft"), false);
		PlaceInteractable(W, A + FVector(300, 850, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Pets Pantry — premium pet food and toys"),
			EMinigameKind::COUNT, TEXT(""), FName("adopt"), false);
		PlaceInteractable(W, A + FVector(500, 850, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Fylde Coast Supplies — everything you forgot to pack"),
			EMinigameKind::COUNT, TEXT(""), FName("craft"), false);
		// Victoria Road shops
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 400, 100), FVector(12, 1, 0.3f));
		PlaceInteractable(W, A + FVector(-500, 400, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Card Factory — cards for every occasion"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(-300, 400, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Home Bargains — the everything shop"),
			EMinigameKind::COUNT, TEXT(""), FName("craft"), false);
		PlaceInteractable(W, A + FVector(-100, 400, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Boots the Chemist — prescriptions and toiletries"),
			EMinigameKind::COUNT, TEXT(""), FName("transition"), false);
		PlaceInteractable(W, A + FVector(100, 400, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Costa Coffee — flat white and a window seat"),
			EMinigameKind::COUNT, TEXT(""), FName("date"), false);
		PlaceInteractable(W, A + FVector(300, 400, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("Age UK — charity shop, pre-loved gems"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(500, 400, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("The Venue — live entertainment, bingo, community hub"),
			EMinigameKind::Blackjack, TEXT(""), FName(), false);
		// Accessibility
		PlaceInteractable(W, A + FVector(400, 200, 0), FVector(2, 2, 2), TEXT("Cleveleys Mobility — wheelchair and scooter hire"),
			EMinigameKind::COUNT, TEXT(""), FName("accessibility"), false);
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Cleveleys Promenade - seaside arcade"),
			EMinigameKind::Slots, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(2, 1, 2), TEXT("Fleetwood docks (fishing)"),
			EMinigameKind::Fishing, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 2, 2), TEXT("Blackpool Tram stop (craft - fish and chips)"),
			EMinigameKind::COUNT, TEXT(""), FName("craft"), false);
		PlaceInteractable(W, A + FVector(0, -1200, 0), FVector(2, 2, 2), TEXT("NHS Rossall Road walk-in centre"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		PlaceInteractable(W, A + FVector(0, 2000, 0), FVector(1, 1, 1), TEXT("View of Blackpool Tower from the prom"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		if (auto* Fisher = SpawnNpc(W, A + FVector(0, 600, 100), TEXT("Ronnie the Fisherman"), TEXT("Knows every tide since 1962")))
			Fisher->DailySchedule = TEXT("patrol Fleetwood docks");
		SpawnNpc(W, A + FVector(300, -100, 100), TEXT("Margaret"), TEXT("Runs the guesthouse. Allergic to everything."));
		SpawnNpc(W, A + FVector(-300, -100, 100), TEXT("Dougie"), TEXT("Tram driver, amateur historian of the Fylde Coast"));
		SpawnNpc(W, A + FVector(200, -600, 100), TEXT("Dr. Simmons"), TEXT("NHS GP at Rossall Road, 40 years of care"));
		SpawnNpc(W, A + FVector(200, 800, 100), TEXT("Linda"), TEXT("UBU support worker. Makes a proper brew. Listens without judgement."));
		SpawnNpc(W, A + FVector(250, 800, 100), TEXT("Karen"), TEXT("UBU resident, Flat 4. Reads the entire paper every morning. Knows everyone's business."));
		SpawnNpc(W, A + FVector(150, 800, 100), TEXT("Paul"), TEXT("UBU resident, Flat 2. Artist. Paints the sea at sunrise. Getting better every day."));
		SpawnNpc(W, A + FVector(400, 200, 100), TEXT("Pete"), TEXT("Mobility shop owner. 'This chair will get you anywhere on the prom.'"));
		break;
	case ERing::VernetLesBains:
		// Vernet-les-Bains, Pyrénées-Orientales, France — 42°33′00″N 2°23′19″E.
		// Population 1,498. Elevation 559-2,760m. 300 sunny days/year.
		// Thermal spa town at foot of Canigó (2,785m). Yellow train nearby.
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"), A + FVector(0, 400, 700), FVector(8, 8, 22)); // Canigó
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(400, -400, 300), FVector(4, 4, 6));    // St Saturnin church
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(-400, -400, 200), FVector(3, 6, 4));   // thermal baths
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, 0, 150), FVector(2, 2, 3));          // old hotel
		PlaceMesh(W, TEXT("/Engine/BasicShapes/Cube.Cube"), A + FVector(0, -600, 50), FVector(10, 2, 1));       // market square
		PlaceInteractable(W, A + FVector(0, 800, 0), FVector(2, 2, 2), TEXT("Thermal spring baths — restore health"),
			EMinigameKind::Surgery, TEXT(""), FName(), false);
		// Entente Cordiale Monument (only one in France, 1914/1920)
		PlaceInteractable(W, A + FVector(800, 0, 0), FVector(2, 2, 2), TEXT("Entente Cordiale Monument — the only one in France"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(-800, 0, 0), FVector(2, 1, 2), TEXT("River Cady — Kipling Bridge (fishing)"),
			EMinigameKind::Fishing, TEXT(""), FName(), false);
		// St George's Anglican Church (1912, first change ringing bells in France)
		PlaceInteractable(W, A + FVector(400, -100, 0), FVector(1.5f, 1.5f, 1.5f), TEXT("St George's Anglican Church — first peal of bells in France"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		PlaceInteractable(W, A + FVector(0, -1200, 0), FVector(2, 2, 2), TEXT("Saturday market in the square"),
			EMinigameKind::COUNT, TEXT(""), FName("craft"), false);
		PlaceInteractable(W, A + FVector(0, 1800, 0), FVector(1, 1, 1), TEXT("Summit of Canigó — 'a magician among mountains' (Kipling, 1911)"),
			EMinigameKind::COUNT, TEXT(""), FName("explore"), false);
		// Characters
		SpawnNpc(W, A + FVector(200, 200, 100), TEXT("Mme. Dupont"), TEXT("Runs the old hotel. Her grandmother started it in 1922."));
		SpawnNpc(W, A + FVector(-200, 200, 100), TEXT("Jean-Luc"), TEXT("Thermal bath attendant, amateur mountain guide of Canigó."));
		SpawnNpc(W, A + FVector(0, -400, 100), TEXT("Catherine"), TEXT("Market florist. Knows every flower on Canigó by its Catalan name."));
		SpawnNpc(W, A + FVector(400, -100, 100), TEXT("Philippe"), TEXT("Cheese vendor. The Roquefort is beyond reproach. Speaks Occitan."));
		SpawnNpc(W, A + FVector(-300, 100, 100), TEXT("Rev. Whitfield"), TEXT("Anglican vicar of St George's. Rings the bells on Sundays."));
		break;
	case ERing::Panem:
		{
			// GDD 4.x — Panem: The Hunger Games. All 13 Districts + Capitol + Arena + Rebellion.
			auto P = [&](float X, float Y, float Z, float SX, float SY, float SZ, const TCHAR* Mesh) {
				PlaceMesh(W, Mesh, A + FVector(X, Y, Z), FVector(SX, SY, SZ));
			};
			auto I = [&](float X, float Y, const TCHAR* Text, EMinigameKind K, FName Stat) {
				PlaceInteractable(W, A + FVector(X, Y, 0), FVector(2, 2, 2), Text, K, TEXT(""), Stat, false);
			};
			auto N = [&](float X, float Y, const TCHAR* Name, const TCHAR* Role) {
				SpawnNpc(W, A + FVector(X, Y, 100), Name, Role);
			};
			// === THE CAPITOL (NW) ===
			P(-2000, -2000, 600, 6, 6, 18, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			P(-2000, -2000, 300, 8, 8, 6, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(-1600, -1600, 500, 3, 3, 14, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			P(-2400, -1600, 500, 3, 3, 14, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			N(-2000, -2000, TEXT("President Snow"), TEXT("President of Panem, the white rose and the venom"));
			N(-1800, -2000, TEXT("Effie Trinket"), TEXT("Escort for District 12"));
			N(-2200, -2000, TEXT("Plutarch Heavensbee"), TEXT("Head Gamemaker, secret revolutionary"));
			N(-2000, -1600, TEXT("Caesar Flickerman"), TEXT("Hunger Games host, eternal smile"));
			N(-2000, -2200, TEXT("Seneca Crane"), TEXT("Former Head Gamemaker. The berries ended him."));
			// Training Centre.
			P(-2000, -600, 300, 12, 8, 6, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(-2000, -800, TEXT("Training Centre — combat simulation"), EMinigameKind::COUNT, FName("tribute_train"));
			I(-2000, -400, TEXT("Training Centre — survival station"), EMinigameKind::COUNT, FName("survival"));
			I(-2000, -200, TEXT("Training Centre — archery range"), EMinigameKind::FootballShootout, FName());
			I(-2000, 200, TEXT("Training Centre — spear throwing"), EMinigameKind::FootballShootout, FName());
			I(-2200, 0, TEXT("Training Centre — camouflage station"), EMinigameKind::COUNT, FName("craft"));
			I(-1800, 0, TEXT("Training Centre — edible plants test"), EMinigameKind::COUNT, FName("survival"));
			I(-2000, -100, TEXT("Training Centre — sponsor appeal stage"), EMinigameKind::Dance, FName());
			N(-2000, -500, TEXT("Cinna"), TEXT("Stylist for District 12. The spark of rebellion in fabric."));
			// The Arena (active Games zone).
			P(3000, 0, 50, 20, 20, 0.5f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(3000, 0, 600, 2, 2, 18, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			P(3000, 1000, 30, 6, 1, 0.6f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(3000, -1000, 30, 1, 6, 0.6f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(3000, 0, TEXT("The Arena — 74th Hunger Games site"), EMinigameKind::COUNT, FName("tribute_train"));
			I(3200, 500, TEXT("Cornucopia — weapons cache"), EMinigameKind::COUNT, FName("craft"));
			// === REBELLION HQ — District 13 (S) ===
			P(0, -3000, 100, 8, 8, 2, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(600, -3000, 80, 3, 3, 1.5f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(-600, -3000, 80, 3, 3, 1.5f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(0, -2800, TEXT("District 13 — command bunker"), EMinigameKind::COUNT, FName("heist"));
			I(300, -2600, TEXT("District 13 — weapons development"), EMinigameKind::COUNT, FName("craft"));
			I(-300, -2600, TEXT("District 13 — propaganda studio (propos)"), EMinigameKind::Dance, FName());
			N(100, -3000, TEXT("President Coin"), TEXT("President of District 13. Cold calculation, hot ambition."));
			N(-100, -3000, TEXT("Boggs"), TEXT("Commander of the Star Squad. Katniss's shield."));
			N(200, -3200, TEXT("Cressida"), TEXT("Director of propos. Mockingjay propaganda filmmaker."));
			N(-200, -3200, TEXT("Messalla"), TEXT("Assistant director. Cressida's partner in filming."));
			N(0, -3200, TEXT("Pollux"), TEXT("Avox cameraman. Silent. Watches everything."));
			// === DISTRICT 1 — Luxury goods (NW of Capitol) ===
			I(-4000, -4000, TEXT("District 1 — jewellery forge"), EMinigameKind::COUNT, FName("craft"));
			N(-4000, -3800, TEXT("Marvel"), TEXT("Career tribute, District 1. Spear. Killed Rue."));
			N(-3800, -4000, TEXT("Glimmer"), TEXT("Career tribute, District 1. Tracker jackers took her."));
			// === DISTRICT 2 — Masonry / peacekeeper base ===
			P(-4000, 0, 200, 6, 6, 4, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(-4000, 0, 500, 2, 2, 10, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			I(-4000, 200, TEXT("District 2 — the Nut (mountain fortress)"), EMinigameKind::COUNT, FName("tribute_train"));
			N(-4000, 0, TEXT("Cato"), TEXT("Career tribute, District 2. Sword. Almost won."));
			N(-3800, 0, TEXT("Clove"), TEXT("Career tribute, District 2. Knives. Thresh broke her."));
			N(-4200, 0, TEXT("Lyme"), TEXT("Victor of District 2. Turned against the Capitol."));
			// === DISTRICT 3 — Technology ===
			I(-2000, 2500, TEXT("District 3 — electronics factory"), EMinigameKind::COUNT, FName("craft"));
			N(-2000, 2500, TEXT("Beetee Latier"), TEXT("Victor of District 3. Wire genius. Electrocuted the arena."));
			N(-1800, 2500, TEXT("Wiress"), TEXT("Victor of District 3. 'Tick tock.' Sees what no one else can."));
			// === DISTRICT 4 — Fishing ===
			P(0, 4500, 100, 2, 8, 2, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(0, 4500, TEXT("District 4 — fishing docks"), EMinigameKind::Fishing, FName());
			N(200, 4500, TEXT("Finnick Odair"), TEXT("District 4. Trident master. Golden boy, broken soul."));
			N(-200, 4500, TEXT("Annie Cresta"), TEXT("District 4. Finnick's love. Lost to the arena, found again."));
			N(400, 4500, TEXT("Mags"), TEXT("District 4. Oldest victor. Gave her life for the Mockingjay."));
			// === DISTRICT 5 — Power / electricity ===
			P(4000, -2000, 300, 4, 4, 8, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			I(4000, -2000, TEXT("District 5 — hydroelectric dam"), EMinigameKind::COUNT, FName("explore"));
			N(4000, -1800, TEXT("Foxface"), TEXT("District 5. Outsmarted everyone. Nightlock got her."));
			// === DISTRICT 6 — Transportation ===
			P(-3000, 2500, 100, 8, 2, 2, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(-3000, 2500, TEXT("District 6 — hovercraft terminal"), EMinigameKind::COUNT, FName("explore"));
			N(-3000, 2500, TEXT("The Morphlings"), TEXT("District 6 victors. Artists. Sacrificed for the Mockingjay."));
			// === DISTRICT 7 — Lumber ===
			P(2500, 3500, 300, 2, 2, 8, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
			P(2500, 3500, 100, 4, 4, 2, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(2500, 3500, TEXT("District 7 — lumber mill"), EMinigameKind::COUNT, FName("tribute_train"));
			N(2500, 3500, TEXT("Johanna Mason"), TEXT("District 7. Axe wielder. Stripped to her soul. Survived."));
			// === DISTRICT 8 — Textiles ===
			I(-3500, 3500, TEXT("District 8 — textile factory"), EMinigameKind::COUNT, FName("craft"));
			N(-3500, 3500, TEXT("Commander Paylor"), TEXT("District 8. Rebellion leader. Later President of Panem."));
			// === DISTRICT 9 — Grain ===
			P(5000, 0, 30, 8, 8, 0.5f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(5000, 0, TEXT("District 9 — grain silos"), EMinigameKind::COUNT, FName("harvest"));
			// === DISTRICT 10 — Livestock ===
			P(0, 5500, 150, 4, 4, 3, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(0, 5300, TEXT("District 10 — cattle ranches"), EMinigameKind::COUNT, FName("hunt"));
			N(0, 5500, TEXT("Dalton"), TEXT("District 10. Cattle rancher. Rebel spy."));
			// === DISTRICT 11 — Agriculture ===
			P(5000, 3000, 30, 10, 10, 0.5f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(5000, 3000, TEXT("District 11 — orchards and fields"), EMinigameKind::COUNT, FName("harvest"));
			I(5000, 3300, TEXT("District 11 — the hanging tree"), EMinigameKind::COUNT, FName("explore"));
			N(4800, 3000, TEXT("Rue"), TEXT("District 11. The Mockingjay's first ally. Too young."));
			N(5200, 3000, TEXT("Thresh"), TEXT("District 11. Spared Katniss. The fields remember."));
			N(5000, 2800, TEXT("Seeder"), TEXT("District 11 victor. Paintbrush and dignity. Capitol killed her."));
			// === DISTRICT 12 — Coal mining, The Seam, The Hob (E) ===
			P(0, 3000, 200, 6, 4, 4, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(400, 3000, 150, 2, 2, 3, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(-400, 3000, 150, 2, 2, 3, TEXT("/Engine/BasicShapes/Cube.Cube"));
			P(0, 3000, 100, 3, 3, 2, TEXT("/Engine/BasicShapes/Cube.Cube"));
			// Victor's Village.
			P(0, 3500, 100, 3, 3, 2, TEXT("/Engine/BasicShapes/Cube.Cube"));
			// The Meadow.
			P(600, 3200, 15, 3, 3, 0.3f, TEXT("/Engine/BasicShapes/Cube.Cube"));
			I(0, 2800, TEXT("The Hob — black market"), EMinigameKind::COUNT, FName("craft"));
			I(0, 3700, TEXT("Victor's Village — Katniss's house"), EMinigameKind::COUNT, FName("explore"));
			I(0, 2600, TEXT("The Reaping square"), EMinigameKind::COUNT, FName("survival"));
			I(600, 3300, TEXT("The Meadow — Katniss's thinking spot"), EMinigameKind::COUNT, FName("explore"));
			N(100, 3000, TEXT("Katniss Everdeen"), TEXT("The Mockingjay. Victor of the 74th Hunger Games."));
			N(-100, 3000, TEXT("Peeta Mellark"), TEXT("The boy with the bread. Victor. Artist. Baker."));
			N(300, 3000, TEXT("Gale Hawthorne"), TEXT("Hunting partner. The flame that lit the rebellion."));
			N(-300, 3000, TEXT("Haymitch Abernathy"), TEXT("Mentor. Victor of the 50th Games. Drunk. Genius."));
			N(0, 3200, TEXT("Primrose Everdeen"), TEXT("Katniss's sister. Healer. The fire that started it all."));
			N(200, 2700, TEXT("Greasy Sae"), TEXT("The Hob cook. Wild dog soup and a warm heart."));
			N(-200, 2700, TEXT("Darius"), TEXT("Peacekeeper turned Avox. Kind when he shouldn't have been."));
			// === The Rebellion War locations ===
			// Capitol invasion route (scattered along the approach from District 13).
			I(-1000, -2000, TEXT("Rebellion staging area — prepare for the Capitol push"), EMinigameKind::COUNT, FName("tribute_train"));
			I(-1500, -1500, TEXT("The Nut crack — District 2 sabotage point"), EMinigameKind::BreachProtocol, FName());
			I(2500, -1500, TEXT("Nethersphere — Blackwall Rift dungeon"), EMinigameKind::COUNT, FName("dungeon"));
			I(-3000, -3000, TEXT("Snow's mansion — the rose garden"), EMinigameKind::COUNT, FName("explore"));
			// Grand rebellion boards.
			I(-2500, -2500, TEXT("Rebellion war room — coordinate the offensive"), EMinigameKind::COUNT, FName("rebellion"));
			I(-2000, -1000, TEXT("Sponsor station — send gifts to tributes"), EMinigameKind::COUNT, FName("sponsor"));
			I(-3000, -500, TEXT("Hunger Games control room — begin the Games"), EMinigameKind::COUNT, FName("hunger_games"));
		}
		break;
	default:
		break;
	}
	// GDD 12.7 — Dynamic world event spawner (random event at each ring).
	{
		static const TCHAR* Events[] = {
			TEXT("Burning building — rescue survivors"), TEXT("Bank robbery in progress"), TEXT("Kaiju alert — evacuate civilians"),
			TEXT("Magical anomaly — stabilize the rift"), TEXT("Convoy ambush — protect the shipment"),
			TEXT("Dragon attack — defend the village"), TEXT("Voxel corruption — purge the chunks"),
			TEXT("Pandemic outbreak — triage tent"), TEXT("Escaped prisoner — bounty alert"),
			TEXT("Faction skirmish — de-escalate"), TEXT("Hacked grid — restore power"),
			TEXT("Reality fracture — seal the crack"), TEXT("Bull market frenzy — trade now"),
		};
		FName StatKeys[] = { FName("tribute_train"), FName("heist"), FName("survival"),
			FName("survival"), FName("tribute_train"), FName("tribute_train"), FName("survival"),
			FName("survival"), FName("bounty"), FName("rebellion"), FName("survival"),
			FName("survival"), FName("survival") };
		if (FMath::RandRange(0, 10) == 0)
		{
			int32 Idx = FMath::RandRange(0, 12);
			PlaceInteractable(W, A + FVector(FMath::RandRange(-2000, 2000), FMath::RandRange(-2000, 2000), 0),
				FVector(2, 2, 2), Events[Idx], EMinigameKind::COUNT, TEXT(""), StatKeys[Idx], false);
		}
	}
}
