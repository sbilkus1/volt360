#include "Character/ChimeraCharacter.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraRecords.h"
#include "Core/ChimeraQuests.h"
#include "Core/ChimeraEconomy.h"
#include "Core/ChimeraWorldSim.h"
#include "World/ChimeraInteractable.h"
#include "Minigames/ChimeraMinigames.h"
#include "Player/ChimeraPlayerController.h"
#include "Engine/GameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "CHIMERA.h"

AChimeraCharacter::AChimeraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// GDD 6.1 - third-person view.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// GDD 5.1 - 7 attributes, 15 points at creation.
	Attributes.SetNum((int32)EAttribute::COUNT);
	for (int32 i = 0; i < (int32)EAttribute::COUNT; ++i) Attributes[i] = 5;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// GDD 3.1 - overlap events so we can scan for interactables.
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
}

void AChimeraCharacter::BeginPlay()
{
	Super::BeginPlay();
	Session = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Session)
	{
		// Inherit created attributes if a character exists (heir / reload).
		for (int32 i = 0; i < (int32)EAttribute::COUNT && i < Session->Attributes.Num(); ++i)
		{
			Attributes[i] = Session->Attributes[i];
		}
	}
}

void AChimeraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Build the mapping context at runtime (GDD 14.15 - remappable).
	IMC = NewObject<UInputMappingContext>(this);
	auto Make = [&](UInputAction*& Out, EInputActionValueType Type, TArray<FKey> Keys)
	{
		Out = NewObject<UInputAction>(this);
		Out->ValueType = Type;
		for (const FKey& K : Keys) IMC->MapKey(Out, K);
	};
	Make(IA_Move, EInputActionValueType::Axis2D, { EKeys::W, EKeys::A, EKeys::S, EKeys::D });
	Make(IA_Look, EInputActionValueType::Axis2D, { EKeys::MouseX, EKeys::MouseY });
	Make(IA_Jump, EInputActionValueType::Boolean, { EKeys::SpaceBar });
	Make(IA_Sprint, EInputActionValueType::Boolean, { EKeys::LeftShift });
	Make(IA_Crouch, EInputActionValueType::Boolean, { EKeys::C });
	Make(IA_Interact, EInputActionValueType::Boolean, { EKeys::E });
	Make(IA_Melee, EInputActionValueType::Boolean, { EKeys::F });
	Make(IA_Ranged, EInputActionValueType::Boolean, { EKeys::R });
	Make(IA_Dragon, EInputActionValueType::Boolean, { EKeys::X });       // GDD 6.9 mount/unmount flight
	Make(IA_Takedown, EInputActionValueType::Boolean, { EKeys::Q });      // GDD 6.2 non-lethal takedown

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Sub->AddMappingContext(IMC, 0);
		}
	}

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AChimeraCharacter::Move);
		EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AChimeraCharacter::Look);
		EIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AChimeraCharacter::StartJump);
		EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AChimeraCharacter::StopJump);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &AChimeraCharacter::StartSprint);
		EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AChimeraCharacter::StopSprint);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &AChimeraCharacter::ToggleCrouch);
		EIC->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &AChimeraCharacter::Interact);
		EIC->BindAction(IA_Melee, ETriggerEvent::Triggered, this, &AChimeraCharacter::MeleeAttack);
		EIC->BindAction(IA_Ranged, ETriggerEvent::Triggered, this, &AChimeraCharacter::RangedAttack);
		EIC->BindAction(IA_Dragon, ETriggerEvent::Triggered, this, &AChimeraCharacter::ToggleDragonFlight);
		EIC->BindAction(IA_Takedown, ETriggerEvent::Triggered, this, &AChimeraCharacter::StealthTakedown);
	}
}

void AChimeraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Session) Session->AddPlayTime(DeltaTime);
	if (Session && Session->LastMessageTime > 0.f) Session->LastMessageTime -= DeltaTime;
	AttackCooldown = FMath::Max(0.f, AttackCooldown - DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = bSprint ? 900.f : 450.f;
	if (bDragonFlying)
	{
		// GDD 6.9 - aerial movement.
		GetCharacterMovement()->MaxWalkSpeed = 2000.f;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		// Flying builds Dragon Riding.
		if (Session && FMath::FRand() < DeltaTime * 2.f)
		{
			Session->AddSkillXP(FName("Tech_DragonRiding"), 1);
			Session->AddStat(TEXT("dragon_flight_sec"), 1);
		}
	}
	else if (GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	// GDD 3.1 - find nearest interactable each scan interval.
	InteractionScan -= DeltaTime;
	if (InteractionScan <= 0.f)
	{
		InteractionScan = 0.15f;
		TArray<AActor*> Overlapping;
		GetOverlappingActors(Overlapping, AChimeraInteractable::StaticClass());
		AChimeraInteractable* Best = nullptr;
		float BestDist = TNumericLimits<float>::Max();
		for (AActor* A : Overlapping)
		{
			AChimeraInteractable* I = Cast<AChimeraInteractable>(A);
			if (!I) continue;
			float D = FVector::DistSquared(GetActorLocation(), I->GetActorLocation());
			if (D < BestDist) { BestDist = D; Best = I; }
		}
		Focus = Best;
	}
}

void AChimeraCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Axis = Value.Get<FVector2D>();
	if (Axis.SizeSquared() > 0.01f)
	{
		// GDD 12.2 - M1 "Survive the Crash" completes by moving.
		if (Session) Session->AddStat(TEXT("move"), 1);
		if (auto* Q = GetGameInstance()->GetSubsystem<UQuestSystem>()) Q->ProgressObjective(TEXT("move"));
	}
	AddMovementInput(GetActorForwardVector(), Axis.Y);
	AddMovementInput(GetActorRightVector(), Axis.X);
}

void AChimeraCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(-Axis.Y);
}

void AChimeraCharacter::ToggleCrouch()
{
	if (bStealth)
	{
		UnCrouch();
		bStealth = false;
	}
	else
	{
		Crouch();
		bStealth = true;
	}
}

void AChimeraCharacter::Interact()
{
	if (!Focus) return;
	Focus->OnInteract(this);
}

// GDD 6.1 - contextual combat (melee / ranged vs Focus, which may be a drone).
void AChimeraCharacter::DealMeleeDamage()
{
	if (AttackCooldown > 0.f || !Focus) return;
	AttackCooldown = 0.6f;
	MeleeCombo = (MeleeCombo % 3) + 1;
	float Dmg = 30.f;
	if (bStealth)
	{
		// GDD 6.2 - stealth multiplier.
		Dmg = 90.f;
		AddSkillXPByName(TEXT("Combat_Stealth"), 15);
	}
	Dmg *= (Session ? Session->GetDamageMultiplier() : 1.f);
	Focus->TakeHit(this, Dmg);
	AddSkillXPByName(TEXT("Combat_Brawling"), 10);
}

void AChimeraCharacter::StealthTakedown()
{
	// GDD 6.2 - non-lethal takedown only from stealth.
	if (!bStealth) { if (Session) Session->ShowMessage(TEXT("Takedown needs stealth (C).")); return; }
	if (!Focus) return;
	Focus->TakeHit(this, 9999.f);
	AddSkillXPByName(TEXT("Combat_Stealth"), 20);
	AddSkillXPByName(TEXT("Pol_Firearms"), 5);
	if (Session)
	{
		Session->HeroVillainMeter = FMath::Clamp(Session->HeroVillainMeter + 1, -100, 100);
		Session->ShowMessage(TEXT("Non-lethal takedown. Clean hands, clean conscience. +Stealth"));
	}
}

// GDD 6.9 - mount the dragon for flight when bonded enough.
void AChimeraCharacter::ToggleDragonFlight()
{
	if (!Session) return;
	if (!Session->bHasDragon)
	{
		Session->ShowMessage(TEXT("No dragon yet. The egg is in Alagaesia."));
		return;
	}
	if (!Session->CanFlyDragon())
	{
		Session->ShowMessage(FString::Printf(TEXT("Bond too weak (%d/20). Feed and bond with your dragon."), Session->GetDragonBond()));
		return;
	}
	bDragonFlying = !bDragonFlying;
	Session->ShowMessage(bDragonFlying
		? FString::Printf(TEXT("%s launches into the sky. You ride as one. (X to land)"), *Session->GetDragonName())
		: TEXT("You dismount. The dragon folds its wings."));
	Session->BondWithDragon(2);
}

void AChimeraCharacter::DealRangedDamage()
{
	if (AttackCooldown > 0.f || !Focus) return;
	AttackCooldown = 0.9f;
	Focus->TakeHit(this, 45.f);
	AddSkillXPByName(TEXT("Combat_Handguns"), 10);
}

// --- GDD 5 progression funnel ---
void AChimeraCharacter::AddAttribute(EAttribute A, int32 Delta)
{
	Attributes[(int32)A] = FMath::Clamp(Attributes[(int32)A] + Delta, 1, 100);
	if (Session)
	{
		Session->Attributes = Attributes;
	}
}

void AChimeraCharacter::AddSkillXPByName(FName Id, int32 XP)
{
	if (Session) Session->AddSkillXP(Id, XP);
}

void AChimeraCharacter::OnMinigameMistake()
{
	if (Session)
	{
		if (auto* Q = GetGameInstance()->GetSubsystem<UQuestSystem>()) Q->OnMistake();
	}
}

void AChimeraCharacter::FinishCharacterCreation(const FString& LifePath)
{
	if (!Session) return;
	Session->LifePath = LifePath;
	Session->Attributes = Attributes;
	Session->bCharacterCreated = true;

	// GDD 5.1 - life path grants a starting skill advantage.
	if (LifePath == TEXT("Cop")) { Session->AddSkillXP(TEXT("Pol_Firearms"), 60); Session->AddRep(EFaction::NPD, 10); }
	else if (LifePath == TEXT("Doctor")) { Session->AddSkillXP(TEXT("Med_Surgery"), 60); Session->AddRep(EFaction::MedicalBoard, 10); }
	else if (LifePath == TEXT("Dragon Rider")) { Session->AddSkillXP(TEXT("Magic_AncientLanguage"), 60); Session->AddRep(EFaction::DragonRiders, 10); }
	else if (LifePath == TEXT("Superhero")) { Session->AddSkillXP(TEXT("Combat_Brawling"), 60); Session->AddRep(EFaction::HeroGuild, 10); }
	else if (LifePath == TEXT("Racer")) { Session->AddSkillXP(TEXT("Spt_RacingLine"), 60); Session->AddRep(EFaction::RacingFederation, 10); }
	else if (LifePath == TEXT("Builder")) { Session->AddSkillXP(TEXT("Tech_Building"), 60); Session->AddRep(EFaction::BuildersGuild, 10); }
	else if (LifePath == TEXT("Netrunner")) { Session->AddSkillXP(TEXT("Tech_Hacking"), 60); Session->AddRep(EFaction::FixersNetwork, 10); }
	else if (LifePath == TEXT("Fixer")) { Session->AddSkillXP(TEXT("Biz_Negotiation"), 60); Session->AddRep(EFaction::FixersNetwork, 10); }
	else { Session->AddSkillXP(TEXT("Med_Diagnosis"), 60); Session->AddRep(EFaction::MedicalBoard, 10); }

	Session->RecordEvent(FString::Printf(TEXT("The Crash. You wake in the Nexus as a %s. Reality is fracturing."), *LifePath));

	// GDD 12.2 - M1: First Contact auto-starts.
	if (auto* Q = GetGameInstance()->GetSubsystem<UQuestSystem>()) Q->StartQuest(TEXT("Q_A1_Contact"));
}

void AChimeraCharacter::HandleMinigameResult(EMinigameKind Kind, bool bSuccess, int32 Score)
{
	if (!Session) return;
	// GDD 13 -> 5.2: minigame performance feeds skill progression + world sim.
	switch (Kind)
	{
	case EMinigameKind::BreachProtocol:
		AddSkillXPByName(TEXT("Tech_Hacking"), bSuccess ? 40 + Score : 10);
		Session->AddStat(TEXT("hack"), bSuccess ? 1 : 0);
		if (bSuccess)
		{
			Session->RecordEvent(TEXT("Breach Protocol: daemons chained. +Street Cred, +Tech reputation."));
		}
		break;
	case EMinigameKind::Surgery:
		Session->AddStat(TEXT("surgery_done"), 1);
		AddSkillXPByName(TEXT("Med_Surgery"), bSuccess ? 50 + Score / 10 : 10);
		if (auto* M = GetGameInstance()->GetSubsystem<UManagementSubsystem>()) M->OnSurgeryDone(bSuccess);
		Session->RecordEvent(bSuccess ? TEXT("Surgery: success. The patient will live.") : TEXT("Surgery: the patient died. The family is notified. -emotional"));
		break;
	case EMinigameKind::Racing:
		Session->AddStat(TEXT("race_win"), bSuccess ? 1 : 0);
		AddSkillXPByName(TEXT("Spt_RacingLine"), bSuccess ? 50 : 15);
		if (auto* M = GetGameInstance()->GetSubsystem<UManagementSubsystem>()) M->OnRaceFinished(bSuccess ? 1 : 18);
		Session->RecordEvent(bSuccess ? TEXT("Hot lap: P1. The factory team is pleased.") : TEXT("Hot lap: off the pace. Back to the garage."));
		break;
	case EMinigameKind::Lockpicking:
		AddSkillXPByName(TEXT("Tech_Engineering"), bSuccess ? 30 : 5);
		Session->RecordEvent(bSuccess ? TEXT("Lockpicking: clean entry. Nobody saw.") : TEXT("Lockpicking: the pick snapped."));
		break;
	case EMinigameKind::Interrogation:
		Session->AddStat(TEXT("arrest"), bSuccess ? 1 : 0);
		AddSkillXPByName(TEXT("Pol_Interrogation"), bSuccess ? 40 : 10);
		Session->AddStat(TEXT("interrogate"), 1);
		if (auto* Q = GetGameInstance()->GetSubsystem<UQuestSystem>()) Q->ProgressObjective(TEXT("interrogate"));
		Session->RecordEvent(bSuccess ? TEXT("Interrogation: confession on the record.") : TEXT("Interrogation: they lawyered up."));
		break;
	case EMinigameKind::Braindance:
		AddSkillXPByName(TEXT("Tech_Hacking"), bSuccess ? 45 + Score / 5 : 10);
		Session->RecordEvent(bSuccess ? TEXT("Braindance: trace found in the memory stream.") : TEXT("Braindance: the edit was too deep to follow."));
		break;
	case EMinigameKind::Dance:
		AddSkillXPByName(TEXT("Soc_Dance"), bSuccess ? 45 : 12);
		Session->RecordEvent(bSuccess ? TEXT("Dance battle: you own the floor. The crowd remembers.") : TEXT("Dance battle: step on the beat. The crowd is unimpressed."));
		break;
	case EMinigameKind::FootballShootout:
		Session->AddStat(TEXT("goal"), Score / 15); // each scored kick = 15 pts
		AddSkillXPByName(TEXT("Spt_Shooting"), bSuccess ? 40 : 10);
		Session->RecordEvent(bSuccess ? TEXT("Shootout: top corner. The crowd roars.") : TEXT("Shootout: the keeper guesses right."));
		break;
	case EMinigameKind::Fishing:
		AddSkillXPByName(TEXT("Tech_Survival"), bSuccess ? 30 : 5);
		if (auto* Rec = GetGameInstance()->GetSubsystem<UHallOfRecords>()) Rec->Tally(TEXT("fish_caught"), bSuccess ? 1 : 0);
		Session->RecordEvent(bSuccess ? TEXT("Fishing: a fine catch for the cookpot.") : TEXT("Fishing: the big one got away."));
		break;
	case EMinigameKind::Poker:
		AddSkillXPByName(TEXT("Biz_Gambling"), bSuccess ? 25 : 5);
		Session->RecordEvent(bSuccess ? TEXT("Poker: bluffed the AI clean out of a hand.") : TEXT("Poker: the AI smelled the bluff."));
		break;
	case EMinigameKind::DroneRacing:
		AddSkillXPByName(TEXT("Tech_Engineering"), bSuccess ? 30 : 10);
		Session->AddStat(TEXT("drone_race_win"), bSuccess ? 1 : 0);
		Session->RecordEvent(bSuccess ? TEXT("Drone racing: clean run through the gates.") : TEXT("Drone racing: clipped a gate."));
		break;
	case EMinigameKind::MagicDuel:
		AddSkillXPByName(TEXT("Magic_AncientLanguage"), bSuccess ? 45 : 15);
		Session->RecordEvent(bSuccess ? TEXT("Magical duel: opponent defeated with arcane mastery.") : TEXT("Magical duel: bested by superior spellwork."));
		break;
	case EMinigameKind::Pickpocket:
		AddSkillXPByName(TEXT("Combat_Stealth"), bSuccess ? 35 : 10);
		if (!bSuccess) Session->WantedLevel = FMath::Min(5, Session->WantedLevel + 1);
		if (auto* Rec = GetGameInstance()->GetSubsystem<UHallOfRecords>()) Rec->Tally(TEXT("pickpockets"), bSuccess ? 1 : 0);
		Session->RecordEvent(bSuccess ? TEXT("Pickpocketing: lifted the goods clean.") : TEXT("Pickpocketing: caught red-handed."));
		break;
	case EMinigameKind::SafeCrack:
		AddSkillXPByName(TEXT("Tech_Engineering"), bSuccess ? 40 : 10);
		Session->RecordEvent(bSuccess ? TEXT("Safe cracking: the vault surrenders.") : TEXT("Safe cracking: the tumblers won't yield."));
		break;
	case EMinigameKind::Enchanting:
		AddSkillXPByName(TEXT("Magic_AncientLanguage"), bSuccess ? 50 : 15);
		Session->RecordEvent(bSuccess ? TEXT("Enchanting: runes carved, item imbued with power.") : TEXT("Enchanting: the rune faded before completion."));
		break;
	case EMinigameKind::Cooking:
		AddSkillXPByName(TEXT("Tech_Crafting"), bSuccess ? 35 : 10);
		Session->RecordEvent(bSuccess ? FString::Printf(TEXT("Cooking: a perfect dish. Score: %d"), Score) : TEXT("Cooking: the kitchen is a mess."));
		break;
	case EMinigameKind::Blacksmithing:
		AddSkillXPByName(TEXT("Tech_Crafting"), bSuccess ? 40 : 10);
		Session->RecordEvent(bSuccess ? TEXT("Blacksmithing: a fine blade forged in fire.") : TEXT("Blacksmithing: the forge rejected the work."));
		break;
	case EMinigameKind::Blackjack:
		AddSkillXPByName(TEXT("Biz_Gambling"), bSuccess ? 20 : 5);
		break;
	case EMinigameKind::Roulette:
		AddSkillXPByName(TEXT("Biz_Gambling"), bSuccess ? 15 : 5);
		break;
	case EMinigameKind::Slots:
		AddSkillXPByName(TEXT("Biz_Gambling"), bSuccess ? 10 : 3);
		break;
	default:
		break;
	}
}
