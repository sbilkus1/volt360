#include "AI/ChimeraNPC.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraNeuralAI.h"
#include "Core/ChimeraWorldSim.h"
#include "Core/ChimeraEconomy.h"
#include "Character/ChimeraCharacter.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "CHIMERA.h"

ANPCObject::ANPCObject()
{
	PrimaryActorTick.bCanEverTick = true;
	// GDD 14.19.3 - gait generated from personality; prototype uses a cylinder body.
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeLocation(FVector(0, 0, -40));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cyl(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (Cyl.Succeeded()) BodyMesh->SetStaticMesh(Cyl.Object);
	BodyMesh->SetRelativeScale3D(FVector(0.9f, 0.9f, 1.6f));
	GetCapsuleComponent()->SetCapsuleRadius(40.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(60.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	AutoPossessAI = EAutoPossessAI::Disabled; // behavior is logic-in-Tick (no navmesh needed)
}

void ANPCObject::BeginPlay()
{
	Super::BeginPlay();
	// GDD 14.19.1 - every NPC gets a personality + a daily schedule.
	Personality.Openness = FMath::FRandRange(10, 90);
	Personality.Conscientiousness = FMath::FRandRange(10, 90);
	Personality.Extraversion = FMath::FRandRange(10, 90);
	Personality.Agreeableness = FMath::FRandRange(10, 90);
	Personality.Neuroticism = FMath::FRandRange(10, 90);
	Personality.Ambition = FMath::FRandRange(10, 90);
	Personality.Morality = FMath::FRandRange(10, 90);
	Needs = FNeedState();
	WanderTarget = GetActorLocation();
}

void ANPCObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickNeeds(DeltaTime);

	// Wander (GDD 14.19.1 - locomotion: NPCs walk, they never teleport).
	FVector Loc = GetActorLocation();
	if (FVector::DistSquared(Loc, WanderTarget) < 2500.f || WanderTimer <= 0.f)
	{
		WanderTimer = FMath::FRandRange(2.f, 6.f);
		WanderTarget = Loc + FVector(FMath::FRandRange(-600.f, 600.f), FMath::FRandRange(-600.f, 600.f), 0.f);
	}
	WanderTimer -= DeltaTime;
	AddMovementInput((WanderTarget - Loc).GetSafeNormal2D());

	// GDD 14.19 — Neural AI decision-making every 5 seconds.
	NeuralTimer += DeltaTime;
	if (NeuralTimer >= 5.f)
	{
		NeuralTimer = 0.f;
		UNeuralAISubsystem* NAI = GetGameInstance()->GetSubsystem<UNeuralAISubsystem>();
		if (NAI)
		{
			FAI_NeedState Needs; Needs.Values[(int32)EAI_Need::Survival] = bPanic ? 90.f : 20.f;
			FAI_Memory Mem; Mem.RememberShortTerm(FString::Printf(TEXT("Wandering %s"), *DailySchedule));
			FString Env = NAI->AssessEnvironment(TEXT("Mid-Wilshire"), TEXT("Precinct"), bPanic ? 80.f : 20.f);
			FAI_Decision D = NAI->Infer(NpcName, Needs, Personality, Mem, Env);
			// Use neural decision to influence behavior
			if (D.Action == TEXT("flee")) { AddMovementInput((Loc - GetActorLocation()).GetSafeNormal2D() * 3.f); }
			else if (D.Action == TEXT("fight") && D.Priority >= 3) { /* combat stance */ }
			// Record practice based on what they're doing
			if (D.Action == TEXT("shoot")) NAI->RecordPractice(NpcName, TEXT("shooting"), FMath::FRandRange(0.f, 1.f));
		}
	}

	// GDD 14.19.1 - emergency override: flee disasters / heavy wanted.
	UDisasterSubsystem* Dis = GetWorld()->GetSubsystem<UDisasterSubsystem>();
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	bool bPanic = false;
	if (Sess)
	{
		bPanic = (Dis && Dis->bDisasterActive && Dis->DisasterRing == Sess->CurrentRing) || Sess->WantedLevel >= 3;
	}
	if (bPanic && Personality.Neuroticism > 40.f)
	{
		FVector Away = FVector(FMath::FRandRange(-1.f, 1.f), FMath::FRandRange(-1.f, 1.f), 0.f);
		AddMovementInput(Away.GetSafeNormal2D() * 2.f);
	}
}

void ANPCObject::TickNeeds(float DeltaTime)
{
	// GDD 14.19.1 - needs drift and are satisfied by routine.
	Needs.Hunger = FMath::Clamp(Needs.Hunger + DeltaTime * 0.5f, 0.f, 100.f);
	Needs.Thirst = FMath::Clamp(Needs.Thirst + DeltaTime * 0.4f, 0.f, 100.f);
	Needs.Fatigue = FMath::Clamp(Needs.Fatigue + DeltaTime * 0.2f, 0.f, 100.f);
	Needs.Social = FMath::Clamp(Needs.Social + (Personality.Extraversion - 50.f) * DeltaTime * 0.05f, 0.f, 100.f);
	if (Needs.Fatigue > 80.f) Needs.Fatigue = 0.f; // slept through the night
}

void ANPCObject::Remember(const FString& Text, bool bImportant)
{
	FMemoryEntry M;
	M.Text = Text;
	M.TimeStamp = GetWorld()->GetTimeSeconds();
	M.bImportant = bImportant;
	Memory.Insert(M, 0);
	if (Memory.Num() > 20) Memory.Pop();
}

void ANPCObject::OnPlayerWrongedByPlayer(const FString& How)
{
	Remember(FString::Printf(TEXT("That player %s. I do not forget."), *How), true);
	if (UChimeraSessionSubsystem* Sess = GetGameInstance() ? GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>() : nullptr)
	{
		Sess->AddRelationship(NpcName, -10);
	}
}

FString ANPCObject::GetGreeting(AChimeraCharacter* Player)
{
	UChimeraSessionSubsystem* Sess = GetGameInstance() ? GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>() : nullptr;

	// GDD 14.19.2 - personality + memory + reputation shape the line.
	FString Tone = Personality.Agreeableness > 60 ? TEXT("Oh, hello!") : TEXT("What do you want.");
	FString Flavor = Personality.Openness > 60 ? TEXT(" Have you tried cross-ring cuisine? Fascinating.") : TEXT("");
	if (Player && Sess)
	{
		if (Sess->HeroVillainMeter > 30) Flavor += TEXT(" You're one of the good ones.");
		else if (Sess->HeroVillainMeter < -30) Flavor += TEXT(" Stay away from my shop.");

		// GDD 14.8 - fame opens doors, infamy closes them.
		if (Sess->GetFame() >= 80) Flavor += TEXT(" The famous hero of the rings! An honor.");
		else if (Sess->GetFame() >= 40) Flavor += TEXT(" I've heard your name around the plaza.");
		if (Sess->GetInfamy() >= 80) Flavor += TEXT(" I know what you've done. The whole ring does.");
		else if (Sess->GetInfamy() >= 40) Flavor += TEXT(" Word is you're trouble. Keep your distance.");

		// GDD 14.9 - a wanted player is refused service.
		if (Sess->GetPlayerBounty() > 0)
		{
			Flavor += FString::Printf(TEXT(" And I won't risk my shop with %d credits on your head."), Sess->GetPlayerBounty());
		}

		// GDD 14.19 - relationship tiers color the greeting.
		int32 Rel = Sess->GetRelationship(NpcName);
		if (Rel >= 60) Flavor += TEXT(" My friend! I owe you. Anything you need.");
		else if (Rel >= 30) Flavor += TEXT(" Good to see you again.");
		else if (Rel <= -60) Flavor += TEXT(" Get out of my sight.");
		else if (Rel <= -30) Flavor += TEXT(" I haven't forgotten what you did.");
	}
	if (Memory.Num() > 0)
	{
		Flavor += FString::Printf(TEXT(" I remember: %s"), *Memory[0].Text);
	}
	return Tone + Flavor;
}

// GDD 14.19 - positive interactions build the relationship up.
void ANPCObject::OnPlayerHelpedPlayer(const FString& How)
{
	Remember(FString::Printf(TEXT("That player %s. I owe them."), *How), true);
	if (UChimeraSessionSubsystem* Sess = GetGameInstance() ? GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>() : nullptr)
	{
		Sess->AddRelationship(NpcName, 12);
	}
}
