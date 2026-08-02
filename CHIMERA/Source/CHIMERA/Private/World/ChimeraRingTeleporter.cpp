#include "World/ChimeraRingTeleporter.h"
#include "World/ChimeraWorldBuilders.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraQuests.h"
#include "Character/ChimeraCharacter.h"
#include "Engine/GameInstance.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "CHIMERA.h"

ARingTeleporter::ARingTeleporter()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate"));
	Gate->SetupAttachment(RootComponent);
	Gate->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Gate->SetCollisionResponseToAllChannels(ECR_Ignore);
	Gate->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded()) Gate->SetStaticMesh(Cube.Object);
	Gate->SetRelativeScale3D(FVector(3.f, 0.6f, 5.f));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(220.f, 220.f, 320.f));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->SetGenerateOverlapEvents(true);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARingTeleporter::OnOverlapBegin);
}

void ARingTeleporter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AChimeraCharacter* C = Cast<AChimeraCharacter>(OtherActor);
	if (!C) return;
	UChimeraSessionSubsystem* Sess = C->GetSession();
	if (!Sess) return;

	// GDD 1.2 - Ring Transit System: instant, loadless travel between rings.
	// Destinations live at kChimeraWorldAltitude (see WorldBuilders) to stay clear
	// of the template map's terrain.
	const TArray<FRingDef>& Rings = FChimeraData::Rings();
	Sess->CurrentRing = Destination;
	const FRingDef& Dest = Rings[(int32)Destination];

	FVector Anchor = Dest.ZoneAnchor + FVector(0, 0, kChimeraWorldAltitude);
	if (Destination == ERing::Nexus) Anchor = FVector(0, 0, kChimeraWorldAltitude + 100);
	C->SetActorLocation(Anchor + FVector(0, 0, 60));

	// GDD 12 - radiant/cross-ring quest hooks.
	Sess->AddStat(TEXT("visit_ring"), 1);
	if (auto* Q = GetGameInstance()->GetSubsystem<UQuestSystem>()) Q->ProgressObjective(TEXT("visit_ring"));
	Sess->AddRep((EFaction)((int32)Destination % (int32)EFaction::COUNT), 1);
	Sess->RecordEvent(FString::Printf(TEXT("RTS: arrived in %s - %s"), *Dest.Name, *Dest.Genre));
	Sess->ShowMessage(FString::Printf(TEXT("Welcome to %s - %s"), *Dest.Name, *Dest.Genre));
}
