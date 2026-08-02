#include "World/ChimeraInteractable.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraBounties.h"
#include "Core/ChimeraHeists.h"
#include "Core/ChimeraHungerGames.h"
#include "Core/ChimeraRebellion.h"
#include "Core/ChimeraPolice.h"
#include "Core/ChimeraBases.h"
#include "Core/ChimeraDating.h"
#include "Core/ChimeraTARDIS.h"
#include "Core/ChimeraVehicles.h"
#include "Core/ChimeraVehicleSpawns.h"
#include "Core/ChimeraDungeons.h"
#include "Core/ChimeraJustice.h"
#include "Core/ChimeraArc.h"
#include "Core/ChimeraFeatures.h"
#include "Core/ChimeraTrading.h"
#include "Core/ChimeraSuperhero.h"
#include "Core/ChimeraIronMan.h"
#include "Core/ChimeraWeapons.h"
#include "Core/ChimeraArsenal.h"
#include "Core/ChimeraLiving.h"
#include "Core/ChimeraWorldLife.h"
#include "Core/ChimeraQuests.h"
#include "Minigames/ChimeraMinigames.h"
#include "Character/ChimeraCharacter.h"
#include "Player/ChimeraPlayerController.h"
#include "AI/ChimeraNPC.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "CHIMERA.h"

AChimeraInteractable::AChimeraInteractable()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded()) Mesh->SetStaticMesh(Cube.Object);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetGenerateOverlapEvents(true);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Trigger->SetGenerateOverlapEvents(true);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AChimeraInteractable::OnTriggerOverlap);
}

void AChimeraInteractable::OnInteract(AChimeraCharacter* C)
{
	UChimeraSessionSubsystem* Sess = C ? C->GetSession() : nullptr;

	// GDD 14.19.2 - talking to an NPC pulls a personality/memory-aware line.
	if (OwnerNpc)
	{
		Message = OwnerNpc->GetGreeting(C);
		Message += FString::Printf(TEXT("  [%s]"), *OwnerNpc->GetNameTag());
		// GDD 14.19 - every polite conversation nudges the relationship up.
		if (Sess && OwnerNpc->NpcName.Len() > 0) Sess->AddRelationship(OwnerNpc->NpcName, 1);
	}

	// GDD 13 - start a minigame if this interactable carries one.
	if (Minigame != EMinigameKind::COUNT)
	{
		if (AChimeraPlayerController* PC = GetWorld()->GetFirstPlayerController<AChimeraPlayerController>())
		{
			UChimeraMinigame* M = FChimeraMinigameFactory::Create(Minigame, PC);
			if (M && Minigame == EMinigameKind::Racing)
			{
				// GDD 13.4 - the trigger box at the end of the circuit is the finish line.
				if (URacingMinigame* R = Cast<URacingMinigame>(M)) R->FinishLine = Trigger;
			}
			PC->StartMinigame(M);
		}
		return;
	}

	// GDD 12 - stat funnels for quests + career skills.
	if (Sess && !StatKey.IsNone())
	{
		Sess->AddStat(StatKey, 1);
		if (auto* Q = GetGameInstance()->GetSubsystem<UQuestSystem>())
		{
			Q->ProgressObjective(StatKey);
		}
		if (StatKey == FName("craft"))
		{
			// GDD 6.4 - craft the first recipe the player has ingredients for.
			bool bCrafted = false;
			for (const FRecipe& R : FChimeraData::Recipes())
			{
				bool bHasAll = true;
				for (const FString& Ing : R.Ingredients)
				{
					if (!Sess->HasItem(FName(*Ing))) { bHasAll = false; break; }
				}
				if (bHasAll && Sess->Craft(FName(*R.Name)))
				{
					bCrafted = true;
					Message = FString::Printf(TEXT("Crafted: %s"), *R.Name);
					break;
				}
			}
			if (!bCrafted)
			{
				C->AddSkillXPByName(TEXT("Tech_Crafting"), 25);
				Message = TEXT("The station hums. You need ingredients (harvest plants, loot crates) to craft a potion.");
			}
		}
		else if (StatKey == FName("build")) { C->AddSkillXPByName(TEXT("Tech_Building"), 25); if (Sess) Sess->AddStat(TEXT("place_block"), 1); }
		else if (StatKey == FName("harvest"))
		{
			// GDD 6.4 - pick a plant; gain a random ingredient.
			static const FName Ingredients[] = {
				FName("NetherWart"), FName("BlazePowder"), FName("Sugar"), FName("FireFern"),
				FName("DragonScaleDust"), FName("GoldenApple"), FName("RegenCrystal"),
				FName("CyberneticImplant"), FName("Brightsteel"), FName("VoxelBlock")
			};
			FName Got = Ingredients[FMath::RandRange(0, 9)];
			Sess->AddItem(Got);
			Message = FString::Printf(TEXT("You harvest %s. It goes in your inventory."), *Got.ToString());
			C->AddSkillXPByName(TEXT("Tech_Farming"), 15);
		}
		else if (StatKey == FName("adopt"))
		{
			// GDD 14.3 - adopt a companion if you don't have one.
			static const TArray<FString> Pets = { TEXT("K9 Dog"), TEXT("Cat"), TEXT("Phoenix"), TEXT("Familiar"), TEXT("Griffin") };
			if (Sess->HasPet())
			{
				Message = FString::Printf(TEXT("Adoption center: %s (%s) already lives with you. Bond %d/100."),
					*Sess->GetPetName(), *Sess->GetPetType(), Sess->GetPetBond());
			}
			else
			{
				FString Type = Pets[FMath::RandRange(0, Pets.Num() - 1)];
				FString Name = FChimeraData::NpcNames()[FMath::RandRange(0, FChimeraData::NpcNames().Num() - 1)];
				if (Sess->AdoptPet(Type, Name))
				{
					Message = FString::Printf(TEXT("Adoption center: %s the %s bonds with you instantly."), *Name, *Type);
					C->AddSkillXPByName(TEXT("Soc_Empathy"), 20);
				}
			}
		}
		else if (StatKey == FName("bounty"))
		{
			// GDD 14.9 - bounty board: accept, then turn in alive (NPD style).
			UBountySubsystem* B = GetGameInstance()->GetSubsystem<UBountySubsystem>();
			if (!B) return;
			if (B->GetAcceptedIndex() < 0)
			{
				int32 Idx = B->AcceptNextBounty();
				if (Idx >= 0)
				{
					const FBountyDef& Def = B->GetActiveBounties()[Idx];
					Message = FString::Printf(TEXT("Contract accepted: %s, a %s in %s. Reward %d (alive +%d). Return to the board to turn in."),
						*Def.TargetName, *Def.Role, *Def.Ring, Def.Reward, Def.AliveBonus);
				}
				else
				{
					Message = TEXT("Bounty board: no contracts right now. Faction conflict will bring more.");
				}
			}
			else
			{
				int32 Reward = B->CompleteAcceptedBounty(true);
				if (Reward <= 0) Message = TEXT("Bounty board: the target slipped away. Try again.");
			}
		}
		else if (StatKey == FName("dragon_bond"))
		{
			// GDD 6.9 - the dragon perch: bond and feed.
			if (!Sess->bHasDragon)
			{
				Message = TEXT("A mossy perch. An egg-shaped dent in the stone. You need a dragon.");
			}
			else
			{
				Sess->BondWithDragon(3);
				if (Sess->HasItem(FName("RawMeat")))
				{
					Sess->FeedDragon(1);
					Message = TEXT("You bond and feed your dragon. The rider's bond strengthens.");
				}
				else
				{
					Message = TEXT("You bond with your dragon (need RawMeat to feed it). Bond grows with time aloft.");
				}
				C->AddSkillXPByName(TEXT("Magic_AncientLanguage"), 10);
			}
		}
		else if (StatKey == FName("hunt"))
		{
			// GDD 6.9 - hunting grounds for dragon feed / survival.
			if (FMath::RandRange(0, 3) == 0)
			{
				Sess->AddItem(FName("RawMeat"), 1);
				Message = TEXT("You hunt down game. RawMeat acquired.");
			}
			else
			{
				Message = TEXT("The wildlife is elusive today. Nothing caught.");
			}
			C->AddSkillXPByName(TEXT("Combat_Blades"), 15);
		}
		else if (StatKey == FName("heist"))
		{
			// GDD 12.8 - heist planning board.
			UHeistSubsystem* H = GetGameInstance()->GetSubsystem<UHeistSubsystem>();
			if (H)
			{
				if (H->GetPhase() == EHeistPhase::Planning)
				{
					const TArray<FHeistDef>& Heists = H->GetHeists();
					int32 Pick = FMath::RandRange(0, Heists.Num() - 1);
					H->StartHeist(Pick, Heists[Pick].Approaches[0]);
					// Auto-hire 1 random crew.
					H->HireCrew(FMath::RandRange(0, 3));
					H->AdvancePhase(); // -> Prep
					Message = FString::Printf(TEXT("Heist board: %s is active. Prep phase. Interact to perform actions."),
						*Heists[Pick].Title);
				}
				else if (H->GetPhase() == EHeistPhase::Prep)
				{
					// Simulate prep action: use a random skill check.
					H->PerformHeistAction(TEXT("Tech_Hacking"), TEXT("Case the security grid"));
				}
				else if (H->GetPhase() == EHeistPhase::Execution)
				{
					H->PerformHeistAction(TEXT("Combat_Stealth"), TEXT("Neutralize patrolling guards"));
				}
				else if (H->GetPhase() == EHeistPhase::Getaway)
				{
					H->PerformHeistAction(TEXT("Tech_Driving"), TEXT("Switch getaway vehicles at dead drop"));
				}
				else
				{
					Message = TEXT("Heist board: no active operation. Work the board for a new job.");
				}
				if (H->GetActiveHeist() && H->GetPhase() != EHeistPhase::Planning)
				{
					Message = FString::Printf(TEXT("Heist: %s | Phase: %s (%d/%d) | Approach: %s"),
						*H->GetActiveHeist()->Title,
						H->GetPhase() == EHeistPhase::Prep ? TEXT("Prep") :
						H->GetPhase() == EHeistPhase::Execution ? TEXT("Execution") :
						H->GetPhase() == EHeistPhase::Getaway ? TEXT("Getaway") : TEXT("?"),
						H->GetHeistProgress(), H->GetHeistMaxProgress(),
						*H->GetApproach());
				}
			}
		}
		else if (StatKey == FName("explore"))
		{
			// GDD 14.10 - hidden collectible / vista discovered.
			static const TArray<FString> Discoveries = {
				TEXT("ancient crash-relief carving"), TEXT("cyber-shrine to a dead netrunner"),
				TEXT("voxel monument built by the first builder"), TEXT("dragon claw fossil"),
				TEXT("smuggler's cache"), TEXT("overgrown Keeper sigil"),
				TEXT("forgotten subway tunnel entrance"), TEXT("floating isle fragment from Aether City's old skyline")
			};
			FString Found = Discoveries[FMath::RandRange(0, Discoveries.Num() - 1)];
			Sess->AddStat(TEXT("discoveries"), 1);
			C->AddSkillXPByName(TEXT("Tech_Survival"), 25);
			Message = FString::Printf(TEXT("You discover: %s. The world remembers."), *Found);
		}
		else if (StatKey == FName("tribute_train"))
		{
			// GDD 4.x – Divergent & Hunger Games combat training simulation.
			TArray<FString> Opponents = { TEXT("a Dauntless-born brawler"), TEXT("a Career tribute with a sword"),
				TEXT("an Erudite tactician"), TEXT("a District 2 volunteer"), TEXT("Peter at knife point"),
				TEXT("Cato at the Cornucopia") };
			FString Opponent = Opponents[FMath::RandRange(0, Opponents.Num() - 1)];
			int32 Check = Sess->GetSkillLevel(FName("Combat_Brawling")) + Sess->GetSkillLevel(FName("Combat_Blades"));
			if (FMath::RandRange(0, Check + 10) >= 10)
			{
				C->AddSkillXPByName(TEXT("Combat_Brawling"), 20);
				C->AddSkillXPByName(TEXT("Combat_Blades"), 15);
				Sess->AddStat(TEXT("tribute_wins"), 1);
				Message = FString::Printf(TEXT("Training simulation: you defeat %s. A cannon fires in the distance."), *Opponent);
			}
			else
			{
				Message = FString::Printf(TEXT("Training simulation: %s gets the upper hand. Back to the mat."), *Opponent);
			}
		}
		else if (StatKey == FName("survival"))
		{
			// GDD 4.x – Hunger Games survival station: find food, craft shelter, trap game.
			TArray<FString> Results = { TEXT("a snare catches a rabbit"), TEXT("you find nightlock — don't eat it"),
				TEXT("you build a waterproof shelter"), TEXT("you identify edible mushrooms"),
				TEXT("you purify water from a stream"), TEXT("sponsor parachute: burn cream and bread") };
			FString Result = Results[FMath::RandRange(0, Results.Num() - 1)];
			Sess->AddItem(FName("RawMeat"), FMath::RandRange(0, 2));
			C->AddSkillXPByName(TEXT("Tech_Survival"), 20);
			C->AddSkillXPByName(TEXT("Med_Diagnosis"), 10);
			Message = FString::Printf(TEXT("Survival station: %s. Stay alive."), *Result);
		}
		else if (StatKey == FName("hunger_games"))
		{
			UHungerGamesSubsystem* HG = GetGameInstance()->GetSubsystem<UHungerGamesSubsystem>();
			if (HG)
			{
				if (HG->GetPhase() == EArenaPhase::Idle) { HG->StartReaping(); Message = TEXT("REAPING DAY. 24 tributes chosen."); }
				else if (HG->GetPhase() == EArenaPhase::Reaping) { HG->AdvanceToTraining(); Message = TEXT("Training begins. 3 days."); }
				else if (HG->GetPhase() == EArenaPhase::Training) { HG->StartArena(); Message = FString::Printf(TEXT("THE GAMES BEGIN. %d tributes enter."), HG->GetAliveCount()); }
				else if (HG->GetPhase() == EArenaPhase::Arena) { Message = FString::Printf(TEXT("Day %d, %d alive."), HG->GetArenaDay(), HG->GetAliveCount()); }
				else { Message = FString::Printf(TEXT("VICTOR: %s."), *HG->GetVictorName()); }
			}
		}
		else if (StatKey == FName("sponsor"))
		{
			UHungerGamesSubsystem* HG = GetGameInstance()->GetSubsystem<UHungerGamesSubsystem>();
			if (HG && HG->GetPhase() == EArenaPhase::Arena && HG->GetAliveCount() > 0)
			{
				int32 Pk = FMath::RandRange(0, 23);
				if (HG->SponsorTribute(Pk)) Message = FString::Printf(TEXT("Gift sent to %s."), *HG->GetTributes()[Pk].Name);
				else Message = TEXT("Need 500 credits to sponsor.");
			}
		}
		else if (StatKey == FName("rebellion"))
		{
			URebellionSubsystem* Reb = GetGameInstance()->GetSubsystem<URebellionSubsystem>();
			if (Reb)
			{
				if (Reb->GetPhase() == ERebelPhase::Peace) { Reb->IgniteRebellion(); Message = TEXT("THE GRAND REBELLION IGNITES."); }
				else if (Reb->GetPhase() == ERebelPhase::Sparks)
					Reb->RebelAction(TEXT("Soc_Leadership"), TEXT("The Mockingjay's signal spreads across all rings."));
				else if (Reb->GetPhase() == ERebelPhase::DistrictRising)
					Reb->RebelAction(TEXT("Combat_Brawling"), TEXT("Sabotage the Nut — District 2's mountain fortress cracks."));
				else if (Reb->GetPhase() == ERebelPhase::Coalition)
					Reb->RebelAction(TEXT("Pol_Investigation"), TEXT("Rescue the hijacked victors from Capitol conditioning."));
				else if (Reb->GetPhase() == ERebelPhase::CapitolSiege)
					Reb->RebelAction(TEXT("Combat_Stealth"), TEXT("Infiltrate Snow's rose garden. The pods are everywhere."));
				else Message = TEXT("VICTORY. The Mockingjay flies. Snow is dead. CHIMERA is free.");
				if (Reb->GetPhase() != ERebelPhase::Peace && Reb->GetPhase() != ERebelPhase::Victory)
					Message = FString::Printf(TEXT("Rebellion: %d/%d. %d Mockingjay factions vs %d Capitol."),
						Reb->GetProgress(), Reb->GetMaxProgress(), Reb->GetMockingjayAllies().Num(), Reb->GetCapitolAllies().Num());
			}
		}
		else if (StatKey == FName("initiate_fight"))
		{
			TArray<FString> Opps = { TEXT("Peter"), TEXT("Molly"), TEXT("Drew"), TEXT("Edward"),
				TEXT("Myra"), TEXT("Christina"), TEXT("Will"), TEXT("Al") };
			FString Opp = Opps[FMath::RandRange(0, Opps.Num() - 1)];
			static int32 Rank = 0;
			int32 Chk = Sess->GetSkillLevel(FName("Combat_Brawling")) + Sess->GetSkillLevel(FName("Combat_Stealth"));
			if (FMath::RandRange(0, Chk + 10) >= 10)
			{
				Rank++; C->AddSkillXPByName(TEXT("Combat_Brawling"), 25);
				Sess->AddStat(TEXT("initiate_wins"), 1); Sess->AddFame(2);
				Message = FString::Printf(TEXT("Initiation fight: you defeat %s. Rank #%d. Dauntless-born notice you."), *Opp, Rank);
			}
			else
				Message = FString::Printf(TEXT("Initiation fight: %s beats you to the mat. Dauntless never quit."), *Opp);
		}
		else if (StatKey == FName("plane_crash"))
		{
			// GDD 4.x — Grey's Anatomy: relive the plane crash that took Lexie and Mark.
			static int32 Phase = 0;
			if (Phase == 0) { Message = TEXT("THE PLANE CRASH. Turbulence. The left engine is on fire. You hear Lexie screaming."); Phase = 1; }
			else if (Phase == 1) { Message = TEXT("You drag Mark out of the wreckage. Arizona is trapped. Cristina operates with a fountain pen. Lexie... Lexie doesn't make it."); Phase = 2; }
			else if (Phase == 2) { Message = TEXT("Rescue arrives after 4 days. You're different now. Mark holds on long enough to say goodbye to Sofia. Sofia grows up knowing her father was a hero."); Phase = 3; }
			else { Message = TEXT("The crash changed everything. Meredith became the sun. Callie became the legs. Derek never flew again. Lexie lives on in the hearts of everyone who loved her."); Phase = 0; }
			C->AddSkillXPByName(TEXT("Med_Surgery"), 50);
			C->AddSkillXPByName(TEXT("Soc_Empathy"), 30);
			Sess->AddStat(TEXT("plane_crash_survivor"), 1);
		}
		else if (StatKey == FName("transition"))
		{
			// GDD 5.1 / 6.8 — Medical gender transition pathway.
			if (Sess->TransitionStage == ETransitionStage::None)
			{ Sess->BeginTransition(); Message = TEXT("You begin your transition. The hospital staff greet you by your true name."); }
			else if (Sess->TransitionStage < ETransitionStage::Complete)
			{ Sess->AdvanceTransition(); Message = FString::Printf(TEXT("Transition progress: %s."),
				*StaticEnum<ETransitionStage>()->GetNameStringByValue((int64)Sess->TransitionStage)); }
			else Message = TEXT("Your transition is complete. You are yourself.");
			C->AddSkillXPByName(TEXT("Med_Surgery"), 20);
		}
		else if (StatKey == FName("police_case"))
		{
			UPoliceInvestigation* PI = GetGameInstance()->GetSubsystem<UPoliceInvestigation>();
			if (PI)
			{
				if (!PI->GetActiveCase()) PI->OpenNewCase();
				else Message = FString::Printf(TEXT("Active case: %s (%d/%d evidence)."), *PI->GetActiveCase()->Title,
					PI->GetActiveCase()->EvidenceCollected.Num(), PI->GetActiveCase()->RequiredEvidence);
			}
		}
		else if (StatKey == FName("evidence"))
		{
			UPoliceInvestigation* PI = GetGameInstance()->GetSubsystem<UPoliceInvestigation>();
			if (PI && PI->GetActiveCase())
			{
				if (PI->CollectEvidence(TEXT("Field evidence"), EEvidenceType::Physical))
					C->AddSkillXPByName(TEXT("Pol_Investigation"), 20);
			}
		}
		else if (StatKey == FName("arrest"))
		{
			UPoliceInvestigation* PI = GetGameInstance()->GetSubsystem<UPoliceInvestigation>();
			if (PI && PI->GetActiveCase() && PI->MakeArrest())
			{
				Message = FString::Printf(TEXT("ARREST MADE. Case closed: %s."), *PI->GetActiveCase()->Title);
				C->AddSkillXPByName(TEXT("Pol_Firearms"), 25);
			}
		}
		else if (StatKey == FName("fear_landscape"))
		{
			// GDD 4.x — Divergent fear landscape: confront one of your chosen fears.
			if (Sess->PlayerFears.Num() == 0) { Message = TEXT("You have not identified your fears yet. Visit the Choosing Ceremony first."); }
			else
			{
				FString Fear = Sess->PlayerFears[FMath::RandRange(0, Sess->PlayerFears.Num() - 1)];
				int32 Check = Sess->GetSkillLevel(FName("Combat_Stealth")) + Sess->GetSkillLevel(FName("Soc_Empathy"));
				if (FMath::RandRange(0, Check + 10) >= 10)
				{
					C->AddSkillXPByName(TEXT("Combat_Stealth"), 30);
					Message = FString::Printf(TEXT("Fear landscape: you face '%s' and stand your ground. Your heart rate drops. You are Divergent."), *Fear);
				}
				else
				{
					Message = FString::Printf(TEXT("Fear landscape: '%s' overwhelms you. The serum spikes. Try again."), *Fear);
				}
			}
		}
		else if (StatKey == FName("tardis"))
		{
			UTARDISSubsystem* T = GetGameInstance()->GetSubsystem<UTARDISSubsystem>();
			if (T)
			{
				T->bTARDISPresent = true;
				// Show current Doctor + quick reference.
				const FDoctorDef* Doc = nullptr;
				for (const FDoctorDef& D : T->GetDoctors()) if (D.Number == T->GetFavouriteDoctor()) { Doc = &D; break; }
				Message = Doc ? FString::Printf(TEXT("The TARDIS hums. The %d%s Doctor (%s) left the kettle on. '%s' Choose your Doctor with A/D, Space to explore."),
					Doc->Number, Doc->Number == 1 ? TEXT("st") : TEXT("th"), *Doc->Actor, *Doc->Catchphrase) : TEXT("The TARDIS stands silent. No Doctor chosen.");
				C->AddSkillXPByName(TEXT("Tech_Engineering"), 10);
			}
		}
		else if (StatKey == FName("tardis_explore"))
		{
			UTARDISSubsystem* T = GetGameInstance()->GetSubsystem<UTARDISSubsystem>();
			if (T && T->bTARDISPresent) { Message = T->ExploreRoom(); C->AddSkillXPByName(TEXT("Soc_Empathy"), 15); }
		}
		else if (StatKey == FName("tardis_sonic"))
		{
			UTARDISSubsystem* T = GetGameInstance()->GetSubsystem<UTARDISSubsystem>();
			if (T && T->bTARDISPresent)
			{
				int32 Next = (T->GetSonicIndex() + 1) % T->GetSonics().Num();
				T->SetSonic(Next);
				Message = FString::Printf(TEXT("Sonic selected: %s. %s"), *T->GetSonics()[Next].Name, *T->GetSonics()[Next].Description);
				C->AddSkillXPByName(TEXT("Tech_Hacking"), 10);
			}
		}
		else if (StatKey == FName("tardis_police"))
		{
			UTARDISSubsystem* T = GetGameInstance()->GetSubsystem<UTARDISSubsystem>();
			if (T && T->bTARDISPresent) { T->ActivatePoliceMode(); C->AddSkillXPByName(TEXT("Pol_Investigation"), 15); Message = TEXT("Police lights flash. The TARDIS is now on patrol."); }
		}
		else if (StatKey == FName("dealership"))
		{
			UVehicleSystem* VS = GetGameInstance()->GetSubsystem<UVehicleSystem>();
			if (VS)
			{
				static int32 Idx = 0;
				const TArray<FVehicleDef>& Cats = VS->GetCatalog();
				if (Cats.Num() == 0) return;
				Idx = (Idx + 1) % Cats.Num();
				if (VS->BuyVehicle(Cats[Idx].Id))
				{
					Message = FString::Printf(TEXT("DEALERSHIP: Sold! %s %s (%d). %d/%d garage slots used."),
						*Cats[Idx].Make, *Cats[Idx].Model, Cats[Idx].Year, VS->GetGarage().Num(), VS->GetGarageSlots());
				}
				else
				{
					Message = FString::Printf(TEXT("DEALERSHIP: %s %s costs %d credits. Need more money or garage space."),
						*Cats[Idx].Make, *Cats[Idx].Model, Cats[Idx].BaseValue);
				}
				C->AddSkillXPByName(TEXT("Biz_Negotiation"), 10);
			}
		}
		else if (StatKey == FName("chop_shop"))
		{
			UVehicleSystem* VS = GetGameInstance()->GetSubsystem<UVehicleSystem>();
			if (VS && VS->GetGarage().Num() > 0)
			{
				int32 Idx = VS->GetGarage().Num() - 1;
				VS->SellVehicle(Idx);
				C->AddSkillXPByName(TEXT("Biz_Negotiation"), 15);
			}
			else Message = TEXT("Chop shop: no vehicles in your garage to sell. Steal or buy one first.");
		}
		else if (StatKey == FName("vehicle_upgrade"))
		{
			UVehicleSystem* VS = GetGameInstance()->GetSubsystem<UVehicleSystem>();
			if (VS && VS->GetGarage().Num() > 0)
			{
				int32 GIdx = VS->GetGarage().Num() - 1;
				if (VS->ApplyUpgrade(GIdx, EUpgradeCategory::Engine))
				{
					const FVehicleDef* Def = VS->FindVehicle(VS->GetGarage()[GIdx].Id);
					Message = FString::Printf(TEXT("Upgraded %s: engine level %d."), Def ? *Def->Model : TEXT("?"), VS->GetGarage()[GIdx].Upgrades[0]);
				}
				else Message = TEXT("Garage: maxed out or need credits.");
				C->AddSkillXPByName(TEXT("Tech_Engineering"), 20);
			}
			else Message = TEXT("Garage: empty. Buy or steal a vehicle first.");
		}
		else if (StatKey == FName("base_buy"))
		{
			UBaseBuildingSubsystem* BB = GetGameInstance()->GetSubsystem<UBaseBuildingSubsystem>();
			if (BB)
			{
				static int32 Idx = 0;
				const TArray<FMBaseDef>& Cats = BB->GetCatalog();
				if (Cats.Num() == 0) return;
				Idx = FMath::RandRange(0, Cats.Num() - 1);
				if (BB->BuyBase(Cats[Idx].Id))
					Message = FString::Printf(TEXT("Real estate: purchased %s. %s"), *Cats[Idx].Name, *Cats[Idx].Bonus);
				else Message = FString::Printf(TEXT("Real estate: %s costs %d credits. Need credits or already owned."), *Cats[Idx].Name, Cats[Idx].Cost);
				C->AddSkillXPByName(TEXT("Biz_RealEstate"), 15);
			}
		}
		else if (StatKey == FName("business_buy"))
		{
			static const TArray<FString> Biz = { TEXT("food truck"), TEXT("magic item shop"), TEXT("racing team sponsorship"),
				TEXT("private detective agency"), TEXT("herbalist stall"), TEXT("netrunner consultancy"),
				TEXT("dragon-scale jewellery boutique"), TEXT("voxel construction firm") };
			FString Bought = Biz[FMath::RandRange(0, Biz.Num() - 1)];
			Sess->AddCurrency(ECurrency::Credits, -5000);
			Sess->AddStat(TEXT("businesses_owned"), 1);
			C->AddSkillXPByName(TEXT("Biz_Management"), 20);
			Message = FString::Printf(TEXT("Business purchased: %s. Your empire grows. +200 credits/hr passive income."), *Bought);
		}
		else if (StatKey == FName("date"))
		{
			UDatingSubsystem* D = GetGameInstance()->GetSubsystem<UDatingSubsystem>();
			if (D)
			{
				static const TArray<FString> Npcs = { TEXT("Lucy Chen"), TEXT("Meredith Grey"), TEXT("April Kepner"), TEXT("Katniss Everdeen"), TEXT("Tris Prior"), TEXT("Arya") };
				FString Npc = Npcs[FMath::RandRange(0, Npcs.Num() - 1)];
				D->AdvanceStage(Npc);
				D->GoOnDate(Npc, TEXT("dinner at a ring-side restaurant"));
				Message = FString::Printf(TEXT("Date with %s — affection growing."), *Npc);
				C->AddSkillXPByName(TEXT("Soc_Seduction"), 15);
			}
		}
		else if (StatKey == FName("underwater"))
		{
			TArray<FString> Finds = { TEXT("a sunken treasure chest"), TEXT("bioluminescent coral"), TEXT("an abandoned submarine"),
				TEXT("a cyber-dolphin pod"), TEXT("ancient drowned ruins"), TEXT("a sea serpent skeleton") };
			Sess->AddItem(FName("GoldenApple"), FMath::RandRange(0, 2));
			C->AddSkillXPByName(TEXT("Tech_Survival"), 30);
			Message = FString::Printf(TEXT("Underwater exploration: you discover %s."), *Finds[FMath::RandRange(0, Finds.Num() - 1)]);
		}
		else if (StatKey == FName("underground"))
		{
			TArray<FString> Finds = { TEXT("diamond veins"), TEXT("an ancient dwarven tunnel"), TEXT("a spider nest"),
				TEXT("a forgotten reactor core"), TEXT("deepslate emerald ore"), TEXT("a primordial fossil") };
			Sess->AddStat(TEXT("discoveries"), 1);
			C->AddSkillXPByName(TEXT("Tech_Engineering"), 25);
			Message = FString::Printf(TEXT("Deep underground: %s."), *Finds[FMath::RandRange(0, Finds.Num() - 1)]);
		}
		else if (StatKey == FName("supply_chain"))
		{
			static const TArray<FString> Routes = {
				TEXT("Alagaesia herbs → Minecraft Frontier lab → Aether City clinics"),
				TEXT("Panem coal → Night City power grid → Mid-Wilshire NPD"),
				TEXT("Vernet-les-Bains thermal water → Chastain Park spa → VIP patients"),
				TEXT("Cleveleys fish → Neo-Kingdom kitchens → royal banquet"),
				TEXT("Dragon scales → Alagaesia market → Seattle research lab"),
			};
			Sess->AddCurrency(ECurrency::Credits, 800);
			C->AddSkillXPByName(TEXT("Biz_Management"), 25);
			Message = FString::Printf(TEXT("Supply chain: %s. +800 credits."), *Routes[FMath::RandRange(0, Routes.Num() - 1)]);
		}
		else if (StatKey == FName("accessibility"))
		{
			// GDD 14.15 — Accessibility mode selection.
			EAccessibilityMode Next = (EAccessibilityMode)(((int32)Sess->Accessibility + 1) % (int32)EAccessibilityMode::COUNT);
			Sess->SetAccessibility(Next);
			Message = FString::Printf(TEXT("Accessibility mode: %s. All rings adapted."),
				*StaticEnum<EAccessibilityMode>()->GetNameStringByValue((int64)Next));
		}
		else if (StatKey == FName("ai_difficulty"))
		{
			EAIDifficulty Next = (EAIDifficulty)(((int32)Sess->AIDifficulty + 1) % (int32)EAIDifficulty::COUNT);
			Sess->SetAIDifficulty(Next);
			Message = FString::Printf(TEXT("AI Difficulty: %s"), *StaticEnum<EAIDifficulty>()->GetNameStringByValue((int64)Next));
		}
		else if (StatKey == FName("tutorial"))
		{
			static const TCHAR* Tips[] = {
				TEXT("E to interact, F to melee, R to shoot, C for stealth."),
				TEXT("Walk into a ring gate to fast-travel. Or drive across the continent."),
				TEXT("Visit the dealership to buy cars. Dark alleys to steal them (wanted level!)."),
				TEXT("Perform surgery at any hospital ER. Grey-Sloan, Gaffney, Chastain Park."),
				TEXT("Open cases at the police case board. Collect evidence, interview, arrest."),
				TEXT("The dragon egg is in Alagaesia. Bond with it. Feed RawMeat. Fly at bond 20."),
				TEXT("Craft potions at Neo-Kingdom. Forge weapons at the Alagaesia dwarven forge."),
				TEXT("Enchant gear at the enchanting table. Magic duels in the chamber."),
				TEXT("Gamble at Afterlife Casino: Texas Hold'em, Blackjack, Roulette, Slots."),
				TEXT("Start a Hunger Games at the Panem control room. Sponsor tributes."),
				TEXT("Ignite the grand rebellion from the War Room. All rings choose sides."),
				TEXT("Buy property at the Base Terminal. Hire staff. Earn passive income."),
				TEXT("Dungeons are at every corner of the continent. How deep can you go?"),
				TEXT("The TARDIS is in the Nexus. Choose your Doctor. Customise your sonic."),
				TEXT("Choose your hero/villain blueprint. Craft Iron Man suits. Wield Mjolnir."),
				TEXT("Dating spots at Costa Coffee, Nexus gazebo, Mid-Wilshire bistro."),
			};
			int32 Pick = FMath::RandRange(0, 15);
			if (Sess->ShouldShowTutorial(FString::Printf(TEXT("tip_%d"), Pick)))
			{
				Sess->MarkTutorialShown(FString::Printf(TEXT("tip_%d"), Pick));
				Message = Tips[Pick];
			}
			else Message = TEXT("You've seen all the tips. Explore and discover the rest yourself.");
		}
		else if (StatKey == FName("dungeon"))
		{
			UDungeonSubsystem* DS = GetGameInstance()->GetSubsystem<UDungeonSubsystem>();
			if (DS)
			{
				if (!DS->IsActive())
				{
					EDungeon D = (EDungeon)FMath::RandRange(0, (int32)EDungeon::COUNT - 1);
					DS->EnterDungeon(D);
				}
				else { DS->ClearFloor(); C->AddSkillXPByName(TEXT("Combat_Brawling"), 20); }
				Message = DS->IsActive() ?
					FString::Printf(TEXT("Floor %d."), DS->GetCurrentFloor()) :
					TEXT("Dungeon run complete. Check your deepest floor record.");
			}
		}
		else if (StatKey == FName("car_theft"))
		{
			UVehicleSystem* VS = GetGameInstance()->GetSubsystem<UVehicleSystem>();
			if (VS)
			{
				const TArray<FVehicleDef>& Cats = VS->GetCatalog();
				if (Cats.Num() > 0)
				{
					FVehicleDef V = Cats[FMath::RandRange(0, Cats.Num() - 1)];
					if (VS->StealVehicle(V.Id))
					{
						Message = FString::Printf(TEXT("You hotwire a %s %s. WANTED. Press H to surrender at the station."), *V.Make, *V.Model);
						// Connect to justice system
						if (auto* JS = GetGameInstance()->GetSubsystem<UJusticeSystem>()) JS->GetState().WantedStars = Sess->WantedLevel;
					}
				}
			}
		}
		else if (StatKey == FName("finale"))
		{
			UStoryArcSubsystem* Arc = GetGameInstance()->GetSubsystem<UStoryArcSubsystem>();
			if (Arc)
			{
				if (Arc->GetChapter() >= EStoryChapter::Finale_Choice)
				{
					static int32 EndIdx = 0;
					EndIdx = (EndIdx + 1) % 4;
					EFinalChoice Choice = (EFinalChoice)EndIdx;
					Arc->SetEnding(Choice);
					const TCHAR* EndTexts[] = {
						TEXT("SEPARATION: You restore the barriers. Each world returns to its own reality. The memories remain. The connections fade. But you know what you saved."),
						TEXT("INTEGRATION: You merge the worlds permanently. Nexopolis becomes the new normal. Dragons in the ER. Surgeons with magic. The impossible is now everyday. Everyone remembers. Everyone is changed."),
						TEXT("TRANSCENDENCE: You ascend. You become a Keeper. From the Nexus Tower, you watch over every ring, every soul. You are the bridge between realities. Eternal. Watching. Guiding."),
						TEXT("CONTROL: You seize the Nexus. The Convergence bows to you. You are the immortal god-king of Nexopolis. Every ring answers to your will. Power. Absolute. Forever.") };
					Message = EndTexts[EndIdx];
				}
				else Message = FString::Printf(TEXT("The Convergence is not ready. You are in %s. %s"), *Arc->GetChapterName(), *Arc->GetChapterObjective());
			}
		}
		else if (StatKey == FName("surrender"))
		{
			UJusticeSystem* JS = GetGameInstance()->GetSubsystem<UJusticeSystem>();
			if (JS && Sess->WantedLevel > 0) { JS->Surrender(); Message = TEXT("Hands up. Cuffs on. You're under arrest."); }
			else Message = TEXT("No wanted stars. Walk free.");
		}
		else if (StatKey == FName("visit_court"))
		{
			UJusticeSystem* JS = GetGameInstance()->GetSubsystem<UJusticeSystem>();
			if (JS && JS->GetState().Phase == ECourtPhase::AwaitingTrial) { JS->GoToCourt(); Message = JS->GetVerdict(); }
			else if (JS && JS->GetState().Phase == ECourtPhase::Booked) Message = FString::Printf(TEXT("Trial in %d days."), JS->GetState().DaysUntilTrial);
			else Message = TEXT("No pending court date.");
		}
		else if (StatKey == FName("bail"))
		{
			UJusticeSystem* JS = GetGameInstance()->GetSubsystem<UJusticeSystem>();
			if (JS) { if (JS->PostBail()) Message = TEXT("Bail posted. Out until trial."); else Message = FString::Printf(TEXT("Bail: %d credits."), JS->GetState().BailAmount); }
		}
		else if (StatKey == FName("prison_job"))
		{
			UJusticeSystem* JS = GetGameInstance()->GetSubsystem<UJusticeSystem>();
			if (JS && JS->IsInPrison()) { JS->AssignJob(EPrisonJob::Kitchen); JS->WorkShift(); Message = TEXT("Prison work shift complete."); }
		}
		else if (StatKey == FName("escape"))
		{
			UJusticeSystem* JS = GetGameInstance()->GetSubsystem<UJusticeSystem>();
			if (JS && JS->IsInPrison()) { JS->PlanEscape(EEscapeMethod::Tunnel); JS->WorkOnEscape(); JS->AttemptEscape(); Message = JS->GetState().Phase == ECourtPhase::Escaped ? TEXT("ESCAPED! Fugitive.") : TEXT("Escape in progress."); }
		}
		else if (StatKey == FName("riot"))
		{
			UJusticeSystem* JS = GetGameInstance()->GetSubsystem<UJusticeSystem>();
			if (JS && JS->IsInPrison()) { JS->FomentRiot(); if (JS->CheckForRiot()) Message = JS->RiotOutcome(); }
		}
		else if (StatKey == FName("phone"))
		{
			UPhoneSystem* PH = GetGameInstance()->GetSubsystem<UPhoneSystem>();
			if (PH) { PH->MarkAllRead(); Message = FString::Printf(TEXT("Phone: %d messages. %s"), PH->UnreadCount(), *PH->GetSocialFeed().Left(100)); }
		}
		else if (StatKey == FName("radio"))
		{
			URadioSystem* RS = GetGameInstance()->GetSubsystem<URadioSystem>();
			if (RS) { int32 Next = (RS->GetStation() + 1) % RS->GetStations().Num(); RS->SetStation(Next); Message = FString::Printf(TEXT("Radio: %s — %s. '%s'"), *RS->GetStations()[Next].Name, *RS->GetStations()[Next].Frequency, *RS->GetDJLine()); }
		}
		else if (StatKey == FName("clothing"))
		{
			UClothingSystem* CL = GetGameInstance()->GetSubsystem<UClothingSystem>();
			if (CL) { static int32 CI = 0; CI = (CI + 1) % CL->GetCatalog().Num(); if (CL->BuyOutfit(CL->GetCatalog()[CI].Name)) Message = FString::Printf(TEXT("Bought: %s (%d credits). %s"), *CL->GetCatalog()[CI].Name, CL->GetCatalog()[CI].Cost, *CL->GetOutfitEffect()); else Message = TEXT("Need more credits!"); }
		}
		else if (StatKey == FName("gang_attack"))
		{
			UGangWarfare* GW = GetGameInstance()->GetSubsystem<UGangWarfare>();
			if (GW) { auto& Gangs = GW->GetGangs(); int32 Pick = FMath::RandRange(0, Gangs.Num() - 1); GW->AttackTurf(Gangs[Pick].GangName); Message = GW->GetTurfReport(); }
		}
		else if (StatKey == FName("collectible"))
		{
			UCollectibleSystem* CS = GetGameInstance()->GetSubsystem<UCollectibleSystem>();
			if (CS) { int32 Pick = FMath::RandRange(0, CS->GetAll().Num() - 1); if (CS->FindCollectible(Pick)) Message = CS->GetProgressText(); else Message = CS->GetProgressText(); }
		}
		else if (StatKey == FName("furniture"))
		{
			UPropertyCustomizer* PC = GetGameInstance()->GetSubsystem<UPropertyCustomizer>();
			if (PC && !Sess->OwnedProperties.IsEmpty()) { static int32 FI = 0; FI = (FI + 1) % PC->GetFurniture().Num(); if (PC->BuyFurniture(Sess->OwnedProperties[0], PC->GetFurniture()[FI].Name)) Message = PC->DescribeBase(Sess->OwnedProperties[0]); }
		}
		else if (StatKey == FName("trade"))
		{
			UTradeSystem* TS = GetGameInstance()->GetSubsystem<UTradeSystem>();
			if (TS)
			{
				TS->RefreshOffers();
				auto& Offers = TS->GetOffers();
				if (Offers.Num() > 0) { if (TS->BuyOffer(0)) Message = FString::Printf(TEXT("Traded: %s from %s."), *Offers[0].ItemId, *Offers[0].Seller); }
				else Message = TEXT("No offers right now. Check back later.");
			}
		}
		else if (StatKey == FName("piece_buy"))
		{
			UTradeSystem* TS = GetGameInstance()->GetSubsystem<UTradeSystem>();
			if (TS) { static int32 PI = 0; PI = (PI + 1) % TS->GetPieces().Num(); if (TS->BuyPiece(TS->GetPieces()[PI].Id)) Message = FString::Printf(TEXT("Piece: %s — %s"), *TS->GetPieces()[PI].Name, *TS->GetPieces()[PI].Description); }
		}
		else if (StatKey == FName("craft_blueprint"))
		{
			UTradeSystem* TS = GetGameInstance()->GetSubsystem<UTradeSystem>();
			USuperheroSubsystem* SH = GetGameInstance()->GetSubsystem<USuperheroSubsystem>();
			if (TS && SH)
			{
				static int32 BPI = 0;
				auto& BPs = SH->GetBlueprints();
				if (BPs.Num() > 0)
				{
					BPI = (BPI + 1) % BPs.Num();
					FString Id = BPs[BPI].Id;
					if (TS->CanCraftBlueprint(Id))
					{
						// Consume pieces and craft
						for (auto& Piece : TS->GetPieces())
						{
							if (Piece.UsedIn.Contains(Id))
							{
								for (int32 i = 0; i < 3; ++i) TS->SellPiece(Piece.Id);
							}
						}
						// Mark blueprint as crafted
						const_cast<TArray<USuperheroSubsystem::FHeroBlueprint>&>(SH->GetBlueprints())[BPI].bCrafted = true;
						C->AddSkillXPByName(TEXT("Tech_Crafting"), 40);
						Message = FString::Printf(TEXT("BLUEPRINT CRAFTED: %s (%s). %s"), *BPs[BPI].Name, *BPs[BPI].Universe, *BPs[BPI].Description);
					}
					else
					{
						Message = FString::Printf(TEXT("%s (%s):\n%s"), *BPs[BPI].Name, *BPs[BPI].Universe, *TS->GetMissingPieces(Id));
					}
				}
			}
		}
		else if (StatKey == FName("iron_man_armor"))
		{
			UTradeSystem* TS = GetGameInstance()->GetSubsystem<UTradeSystem>();
			static TArray<FIronManArmor> Armors = ChimeraIronMan::GetAllArmors();
			static int32 AI = 0;
			AI = (AI + 1) % Armors.Num();
			FIronManArmor& A = Armors[AI];
			if (A.bCrafted) { Message = FString::Printf(TEXT("%s: Already crafted."), *A.Name); }
			else if (ChimeraIronMan::CanCraftArmor(A.Id, Armors, TS))
			{
				for (auto& P : A.RequiredPieces)
					for (int32 i = 0; i < FMath::Min(TS->GetPieceCount(P), 3); ++i) TS->SellPiece(P);
				Sess->AddCurrency(ECurrency::Credits, -A.CreditCost);
				A.bCrafted = true;
				C->AddSkillXPByName(TEXT("Tech_Crafting"), 50);
				Message = FString::Printf(TEXT("ARMOR: %s (%s). %s"), *A.Name, *A.Mark, *A.SpecialAbility);
			}
			else Message = FString::Printf(TEXT("%s (%s, %d) — %d cr\n%s"), *A.Name, *A.Era, A.Year, A.CreditCost, *A.SpecialAbility);
		}
		else if (StatKey == FName("weapon_craft"))
		{
			UWeaponSystem* WS = GetGameInstance()->GetSubsystem<UWeaponSystem>();
			if (WS) { static int32 WI = 0; WI = (WI + 1) % WS->GetCatalog().Num(); if (WS->CraftWeapon(WS->GetCatalog()[WI].Id)) Message = FString::Printf(TEXT("Crafted: %s"), *WS->GetCatalog()[WI].Name); else Message = FString::Printf(TEXT("%s — %d credits. %s"), *WS->GetCatalog()[WI].Name, WS->GetCatalog()[WI].CreditCost, *WS->GetCatalog()[WI].Lore); }
		}
		else if (StatKey == FName("weapon_equip"))
		{
			UWeaponSystem* WS = GetGameInstance()->GetSubsystem<UWeaponSystem>();
			if (WS) { static int32 WE = 0; WE = (WE + 1) % WS->GetCatalog().Num(); WS->Equip(WS->GetCatalog()[WE].Id); Message = FString::Printf(TEXT("Equipped: %s — %s"), *WS->GetCatalog()[WE].Name, *WS->GetCatalog()[WE].Special); }
		}
		else if (StatKey == FName("weapon_mod"))
		{
			UArsenalSubsystem* Ars = GetGameInstance()->GetSubsystem<UArsenalSubsystem>();
			UWeaponSystem* WS = GetGameInstance()->GetSubsystem<UWeaponSystem>();
			if (Ars && WS && !WS->GetEquippedWeapon().IsEmpty())
			{
				static int32 MI = 0; MI = (MI + 1) % Ars->GetModCatalog().Num();
				if (Ars->InstallMod(WS->GetEquippedWeapon(), Ars->GetModCatalog()[MI].Id))
					Message = FString::Printf(TEXT("Mod: %s on %s"), *Ars->GetModCatalog()[MI].Name, *WS->GetEquippedWeapon());
				else Message = FString::Printf(TEXT("Need %d credits."), Ars->GetModCatalog()[MI].Cost);
			}
		}
		else if (StatKey == FName("sleep"))
			{ GetGameInstance()->GetSubsystem<USleepSystem>()->Sleep(8); Message = TEXT("You sleep for 8 hours. Morning light fills the room."); }
		else if (StatKey == FName("travel_train"))
			{ auto* FT = GetGameInstance()->GetSubsystem<UFastTravel>(); if (FT) { static int32 TI = 1; TI = (TI + 1) % 15; const TCHAR* Rings[] = {TEXT("Nexus"),TEXT("Aether City"),TEXT("Mid-Wilshire"),TEXT("Seattle"),TEXT("Night City"),TEXT("Alagaesia"),TEXT("Panem"),TEXT("Cleveleys"),TEXT("VernetLesBains"),TEXT("Chicago"),TEXT("Mercy Heights"),TEXT("ChastainPark"),TEXT("NeoKingdom"),TEXT("MinecraftFrontier"),TEXT("GrandColiseum")}; FT->Travel(Rings[TI], ETravelType::Train); Message = FString::Printf(TEXT("Train to %s."), Rings[TI]); } }
		else if (StatKey == FName("drink"))
			{ auto* Bar = GetGameInstance()->GetSubsystem<UBarSubsystem>(); if (Bar) { static int32 DI = 0; DI = (DI + 1) % Bar->GetMenu().Num(); Bar->BuyDrink(Bar->GetMenu()[DI].Name); Message = TEXT("Cheers!"); } }
		else if (StatKey == FName("karaoke"))
			{ auto* Bar = GetGameInstance()->GetSubsystem<UBarSubsystem>(); if (Bar) Message = Bar->KaraokeSong(); }
		else if (StatKey == FName("stocks"))
			{ auto* ST = GetGameInstance()->GetSubsystem<UStockTrading>(); if (ST) { auto Q = ST->GetQuotes(); if (Q.Num() > 0) { Message = FString::Printf(TEXT("BAWSAQ: %s %.0f (%+.1f). Tip: %s"), *Q[0].Symbol, Q[0].Price, Q[0].Change, *Q[0].Tip); ST->BuyStock(Q[0].Symbol, 10); } } }
		else if (StatKey == FName("dive"))
			{ auto* UW = GetGameInstance()->GetSubsystem<UUnderwaterSystem>(); if (UW) { static int32 SI = 0; SI = (SI + 1) % UW->GetSites().Num(); if (UW->GetGear() == EDivingGear::None) UW->BuyGear(EDivingGear::Scuba); Message = UW->ExploreSite(UW->GetSites()[SI].Name); } }
		else if (StatKey == FName("paramedic"))
			{ auto* EJ = GetGameInstance()->GetSubsystem<UEmergencyJobs>(); if (EJ) { if (EJ->GetJob() == EEmergencyJob::None) EJ->StartJob(EEmergencyJob::Paramedic); else EJ->CompleteCall(); Message = EJ->GetCall().Victim + TEXT(" — go!"); } }
		else if (StatKey == FName("firefighter"))
			{ auto* EJ = GetGameInstance()->GetSubsystem<UEmergencyJobs>(); if (EJ) { if (EJ->GetJob() == EEmergencyJob::None) EJ->StartJob(EEmergencyJob::Firefighter); else EJ->CompleteCall(); Message = TEXT("Fire contained. Building saved."); } }
		else if (StatKey == FName("hunting"))
			{ auto* WL = GetGameInstance()->GetSubsystem<UWildlifeSystem>(); UChimeraSessionSubsystem* S = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>(); Message = WL ? WL->Hunt(S ? StaticEnum<ERing>()->GetNameStringByValue((int64)S->CurrentRing) : TEXT("Alagaesia")) : TEXT(""); }
		else if (StatKey == FName("legendary_fish"))
			{ auto* WL = GetGameInstance()->GetSubsystem<UWildlifeSystem>(); UChimeraSessionSubsystem* Sess2 = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>(); Message = WL ? WL->Fish(Sess2 ? StaticEnum<ERing>()->GetNameStringByValue((int64)Sess2->CurrentRing) : TEXT("Cleveleys"), true) : TEXT(""); }
		else if (StatKey == FName("bank"))
			{ auto* Bank = GetGameInstance()->GetSubsystem<UBankingSystem>(); if (Bank) { Bank->Deposit(5000); Message = Bank->GetLoanStatus(); } }
		else if (StatKey == FName("aircraft"))
			{ auto* Air = GetGameInstance()->GetSubsystem<UAircraftSystem>(); if (Air) { static int32 AI = 0; auto& C = Air->GetCatalog(); AI = (AI + 1) % C.Num(); if (Air->BuyAircraft(C[AI].Name)) Message = TEXT("Aircraft purchased!"); else Message = FString::Printf(TEXT("%s — %d credits. %s"), *C[AI].Name, C[AI].Cost, *C[AI].Special); } }
		else if (StatKey == FName("tv_news"))
			{ auto* TV = GetGameInstance()->GetSubsystem<UTVNewsSystem>(); if (TV) Message = TV->WatchNews(); }
		else if (StatKey == FName("school"))
			{ auto* Ed = GetGameInstance()->GetSubsystem<UEducationSystem>(); if (Ed) { static int32 CI = 0; CI = (CI + 1) % Ed->GetCourses().Num(); if (Ed->Enroll(Ed->GetCourses()[CI].Name)) Message = Ed->GetProgress(); else Message = TEXT("Need credits for tuition."); } }
		else if (StatKey == FName("gym"))
			{ auto* Gym = GetGameInstance()->GetSubsystem<UGymSystem>(); if (Gym) { static const TCHAR* Ex[] = {TEXT("Weights"),TEXT("Cardio"),TEXT("Sparring"),TEXT("Yoga")}; static int32 EX = 0; EX = (EX+1)%4; Message = Gym->Workout(Ex[EX]); } }
		else if (StatKey == FName("criminal"))
			{ auto* Cr = GetGameInstance()->GetSubsystem<UCriminalEmpire>(); if (Cr) { static int32 OI = 0; OI = (OI + 1) % Cr->GetOperations().Num(); if (Cr->StartOperation(Cr->GetOperations()[OI].Name)) Message = Cr->GetEmpireReport(); else if (Cr->CollectIncome()) Message = Cr->GetEmpireReport(); else Message = Cr->GetEmpireReport(); } }
		else if (StatKey == FName("property"))
		{
			// GDD 9.3 - buy the cheapest unowned property the player can afford.
			bool bBought = false;
			const TArray<FPropertyDef>& Props = FChimeraData::Properties();
			const FPropertyDef* Cheapest = nullptr;
			for (const FPropertyDef& P : Props)
			{
				if (Sess->IsPropertyOwned(P.Name)) continue;
				if (!Cheapest || P.Cost < Cheapest->Cost) Cheapest = &P;
			}
			if (Cheapest && Sess->BuyProperty(Cheapest->Name))
			{
				bBought = true;
				Message = FString::Printf(TEXT("Property purchased: %s (+%d credits/hour passive income)."), *Cheapest->Name, Cheapest->IncomePerHour);
			}
			else if (!Cheapest)
			{
				Message = TEXT("Property agent: you own everything worth owning in Nexopolis.");
			}
			else
			{
				Message = FString::Printf(TEXT("Property agent: the cheapest is %s (%d credits). You can't afford it yet."), *Cheapest->Name, Cheapest->Cost);
			}
			(void)bBought;
		}
		else if (StatKey == FName("insurance"))
		{
			// GDD 9.4 - buy the cheapest inactive plan the player can afford.
			const TArray<FInsuranceDef>& Plans = FChimeraData::Insurances();
			const FInsuranceDef* Cheapest = nullptr;
			for (const FInsuranceDef& P : Plans)
			{
				if (Sess->HasInsurance(P.Name)) continue;
				if (!Cheapest || P.PremiumPerHour < Cheapest->PremiumPerHour) Cheapest = &P;
			}
			if (Cheapest && Sess->BuyInsurance(Cheapest->Name))
			{
				Message = FString::Printf(TEXT("Policy active: %s insurance (-%d credits/hour)."), *Cheapest->Name, Cheapest->PremiumPerHour);
			}
			else if (!Cheapest)
			{
				Message = TEXT("Insurance office: every plan is already active. Bold.");
			}
			else
			{
				Message = FString::Printf(TEXT("Insurance office: the cheapest plan is %s (%d/hour). Come back with more credits."), *Cheapest->Name, Cheapest->PremiumPerHour);
			}
		}
		else if (StatKey == FName("dragon_egg"))
		{
			// GDD 4.6/6.9 - the egg hatches; the dragon bonds to you.
			Sess->bHasDragon = true;
			Sess->AddStat(TEXT("dragon_hatched"), 1);
			C->AddSkillXPByName(TEXT("Magic_AncientLanguage"), 40);
			C->AddSkillXPByName(TEXT("Tech_DragonRiding"), 30);
			Sess->AddRep(EFaction::DragonRiders, 15);
			Sess->RecordEvent(TEXT("The egg cracks. A dragon bonds to you. Rider's Bond: shared senses, shared magic pool."));
			Message = TEXT("A dragon hatches and bonds with you. You are a Rider now.");
		}
	}

	if (Sess && !Message.IsEmpty())
	{
		Sess->ShowMessage(Message);
	}
	else if (Sess && bIsDrone)
	{
		Sess->ShowMessage(TEXT("Training target. Use F (melee) or R (ranged)."));
	}
}

void AChimeraInteractable::TakeHit(AChimeraCharacter* C, float Dmg)
{
	if (!bIsDrone) return;
	DroneHealth -= (int32)Dmg;
	UChimeraSessionSubsystem* Sess = C ? C->GetSession() : nullptr;
	if (DroneHealth <= 0)
	{
		// GDD 12.2 M6 - destroy the Reality Aberration's drones.
		if (Sess)
		{
			Sess->AddStat(TEXT("kill_drone"), 1);
			Sess->AddStat(TEXT("kill"), 1); // GDD 14.18 - achievement tally
			Sess->HeroVillainMeter = FMath::Clamp(Sess->HeroVillainMeter + 2, -100, 100);
			if (auto* Q = GetGameInstance()->GetSubsystem<UQuestSystem>()) Q->ProgressObjective(TEXT("kill_drone"));
			Sess->ShowMessage(TEXT("Drone destroyed. Reality stabilizes a little."));
		}
		Destroy();
	}
}

void AChimeraInteractable::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Racing finish detection is done by URacingMinigame::Tick via IsOverlappingActor.
}
