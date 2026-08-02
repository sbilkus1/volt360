#include "Core/ChimeraTrading.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraSuperhero.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UTradeSystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	SeedPieces();
	RefreshOffers();
}

void UTradeSystem::SeedPieces()
{
	auto P = [&](const TCHAR* Id, const TCHAR* Name, const TCHAR* Cat, int32 Buy, const TCHAR* Desc) {
		FTradeablePiece R; R.Id = Id; R.Name = Name; R.Category = Cat; R.BuyPrice = Buy; R.SellPrice = Buy / 2; R.Description = Desc; Pieces.Add(R);
	};

	// === Tech ===
	P(TEXT("arc_reactor"), TEXT("Arc Reactor Core"), TEXT("Tech"), 8000, TEXT("The heart of Iron Man's armour. Palladium-powered, self-sustaining energy. Tony Stark's greatest invention."));
	P(TEXT("nanotube_alloy"), TEXT("Nanotube Alloy Sheet"), TEXT("Tech"), 4000, TEXT("Carbon nanotubes woven into flexible armour. Used in Iron Spider, War Machine, and most modern suits."));
	P(TEXT("repulsor_coil"), TEXT("Repulsor Coil"), TEXT("Tech"), 5000, TEXT("Miniaturised repulsor technology. Palm-mounted flight stabiliser and weapon."));
	P(TEXT("ai_chip"), TEXT("AI Assistant Chip"), TEXT("Tech"), 6000, TEXT("A sentient AI in a chip. Friday, JARVIS, or your own custom. Manages suit systems."));
	P(TEXT("scarab_carapace"), TEXT("Scarab Carapace Fragment"), TEXT("Tech"), 7000, TEXT("Blue Beetle's alien tech. Bonded to the spine. Can create any weapon the mind imagines."));
	P(TEXT("mother_box"), TEXT("Mother Box"), TEXT("Tech"), 15000, TEXT("Living computer from New Genesis/Apokolips. Cyborg's core. Boom Tube access. PING."));
	P(TEXT("wrist_blaster"), TEXT("Wrist-Mounted Blaster"), TEXT("Tech"), 3000, TEXT("Standard superhero sidearm. Repulsor, plasma, or sonic variants."));

	// === Metal / Alloys ===
	P(TEXT("vibranium"), TEXT("Vibranium Shard"), TEXT("Alloy"), 10000, TEXT("The rarest metal on Earth. Absorbs kinetic energy. Captain America's shield. Black Panther's suit."));
	P(TEXT("adamantium"), TEXT("Adamantium Fragment"), TEXT("Alloy"), 12000, TEXT("Indestructible metal. Wolverine's skeleton. Can only be worked while molten. Never again."));
	P(TEXT("uru_metal"), TEXT("Uru Metal Ingot"), TEXT("Alloy"), 9000, TEXT("Asgardian forge metal. Mjolnir and Stormbreaker. Forged in the heart of a dying star."));
	P(TEXT("nth_metal"), TEXT("Nth Metal Shard"), TEXT("Alloy"), 7000, TEXT("Thanagarian anti-gravity metal. Hawkman's mace. Negates magic. Defies gravity."));
	P(TEXT("promethium"), TEXT("Promethium Alloy"), TEXT("Alloy"), 5000, TEXT("DC's super-metal. Deathstroke's armour. Nearly indestructible. Energy-absorbing."));

	// === Cosmic ===
	P(TEXT("power_cosmic"), TEXT("Power Cosmic Shard"), TEXT("Cosmic"), 20000, TEXT("A fragment of Galactus's power. Silver Surfer's source. Matter manipulation at quantum level."));
	P(TEXT("phoenix_shard"), TEXT("Phoenix Force Shard"), TEXT("Cosmic"), 25000, TEXT("The Phoenix. Cosmic fire. Jean Grey's burden. Creation and destruction in one."));
	P(TEXT("tesseract_energy"), TEXT("Tesseract Energy Cell"), TEXT("Cosmic"), 12000, TEXT("Space Stone energy, contained. Captain Marvel's power source. Unstable. Handle with care."));
	P(TEXT("omega_beam"), TEXT("Omega Beam Focusing Lens"), TEXT("Cosmic"), 18000, TEXT("Darkseid's signature. Heat vision that tracks its target across dimensions. Unavoidable."));
	P(TEXT("kryptonite"), TEXT("Kryptonite Crystal"), TEXT("Cosmic"), 8000, TEXT("Radioactive mineral from Krypton. Green weakens Superman. Other colours have different effects."));
	P(TEXT("kryptonian_crystal"), TEXT("Kryptonian Sun Crystal"), TEXT("Cosmic"), 10000, TEXT("Stores yellow sun radiation. Powers Kryptonian abilities. The Fortress of Solitude is made of these."));

	// === Magic / Mystic ===
	P(TEXT("chaos_magic_tome"), TEXT("Chaos Magic Tome"), TEXT("Mystic"), 15000, TEXT("Scarlet Witch's grimoire. Reality warping. Probability manipulation. The Darkhold's sister volume."));
	P(TEXT("sling_ring"), TEXT("Sling Ring"), TEXT("Mystic"), 6000, TEXT("Doctor Strange's portal ring. Opens gateways between any two points. Sparks orange."));
	P(TEXT("eye_of_agamotto"), TEXT("Eye of Agamotto (Replica)"), TEXT("Mystic"), 14000, TEXT("Contains a fragment of time manipulation. Opens the Eye. Sees through all illusions."));
	P(TEXT("ancient_language_rune"), TEXT("Ancient Language Rune"), TEXT("Mystic"), 4000, TEXT("A rune from Alagaesia's old tongue. True Name magic. What you speak becomes truth."));
	P(TEXT("lazarus_water"), TEXT("Lazarus Pit Water Vial"), TEXT("Mystic"), 10000, TEXT("Resurrects the dead. Temporarily drives the revived insane. Ra's al Ghul's immortality secret."));
	P(TEXT("darkhold_page"), TEXT("Darkhold Page — The Book of the Damned"), TEXT("Mystic"), 20000, TEXT("Infinite knowledge. Infinite corruption. One page is enough to destroy a universe."));
	P(TEXT("helmet_of_nabu"), TEXT("Helmet of Nabu Fragment"), TEXT("Mystic"), 13000, TEXT("Doctor Fate's source of power. Lord of Order. Ankh magic. Clairvoyance."));

	// === Genetic / Biological ===
	P(TEXT("symbiote_sample"), TEXT("Symbiote Sample"), TEXT("Alien"), 9000, TEXT("Living alien organism. Venom. Carnage. Bonds to host. Weak to fire and sonic. We are Venom."));
	P(TEXT("super_soldier_serum"), TEXT("Super Soldier Serum Vial"), TEXT("Bio"), 8000, TEXT("The formula that created Captain America. Perfects human potential. One dose. Lifetime commitment."));
	P(TEXT("gamma_isotope"), TEXT("Gamma Radiation Isotope"), TEXT("Bio"), 7000, TEXT("Bruce Banner's curse and gift. Uncontrolled rage becomes unlimited strength. Hulk's catalyst."));
	P(TEXT("healing_factor"), TEXT("Healing Factor Extract"), TEXT("Bio"), 9000, TEXT("Wolverine's regeneration in a vial. Deadpool's immortality. Heals from nearly anything."));
	P(TEXT("martian_dna"), TEXT("Martian DNA Strand"), TEXT("Alien"), 11000, TEXT("J'onn J'onzz's genetic code. Shapeshifting. Telepathy. Intangibility. Avoid open flame."));
	P(TEXT("pym_particle"), TEXT("Pym Particle Capsule"), TEXT("Tech"), 6000, TEXT("Shrink to subatomic. Grow to skyscraper. Ant-Man's gift from Hank Pym. Handle carefully."));
	P(TEXT("web_fluid"), TEXT("Web Fluid Formula"), TEXT("Tech"), 2000, TEXT("Peter Parker's chemical engineering masterpiece. Tensile strength: off the charts. Dissolves in 2 hours."));

	// === Power Sources / Rare ===
	P(TEXT("infinity_gauntlet"), TEXT("Infinity Gauntlet (Empty)"), TEXT("Cosmic"), 50000, TEXT("The glove that held the stones. Eitri's forge. Without the stones, it's just a very expensive fashion accessory."));
	P(TEXT("power_stone"), TEXT("Power Stone Fragment"), TEXT("Cosmic"), 30000, TEXT("Unlimited power. Destroys organic matter on contact. Only the strongest can wield it."));
	P(TEXT("speed_force_conduit"), TEXT("Speed Force Conduit"), TEXT("Cosmic"), 12000, TEXT("The Flash's connection to the Speed Force. Frictionless. Lightning-marked. Time is a suggestion."));
	P(TEXT("green_lantern_ring"), TEXT("Green Lantern Ring (Uncharged)"), TEXT("Cosmic"), 10000, TEXT("Willpower-powered construct projector. Needs a Lantern Battery to charge. Oath required."));
	P(TEXT("anti_life"), TEXT("Anti-Life Equation Fragment"), TEXT("Cosmic"), 40000, TEXT("Darkseid's obsession. One fragment subjugates free will. The full equation controls all consciousness."));
	P(TEXT("storm_essence"), TEXT("Storm Essence — Raw Lightning"), TEXT("Mystic"), 5000, TEXT("Thor's domain, trapped in crystal. Crack it open for a thunderstorm. One use. Dramatic effect."));

	// === Link pieces to blueprints ===
	int32 Idx = 0;
	Pieces[Idx++].UsedIn = { TEXT("iron_man"), TEXT("war_machine"), TEXT("ironheart") };          // arc_reactor
	Idx++; Pieces[Idx].UsedIn = { TEXT("iron_man"), TEXT("iron_spider"), TEXT("war_machine") }; // nanotube
	Idx++; Pieces[Idx].UsedIn = { TEXT("iron_man"), TEXT("star_lord"), TEXT("blue_beetle") };   // repulsor
	Idx++; Pieces[Idx].UsedIn = { TEXT("iron_man"), TEXT("vision"), TEXT("ultron") };            // ai_chip
	Idx++; Pieces[Idx].UsedIn = { TEXT("blue_beetle") };                                        // scarab
	Idx++; Pieces[Idx].UsedIn = { TEXT("cyborg"), TEXT("darkseid") };                           // mother_box
	Idx++; Pieces[Idx].UsedIn = { TEXT("iron_man"), TEXT("booster_gold") };                     // wrist_blaster

	Idx++; Pieces[Idx].UsedIn = { TEXT("cap"), TEXT("black_panther"), TEXT("vision") };        // vibranium
	Idx++; Pieces[Idx].UsedIn = { TEXT("wolverine"), TEXT("deadpool") };                       // adamantium
	Idx++; Pieces[Idx].UsedIn = { TEXT("thor"), TEXT("asgardian") };                            // uru
	Idx++; Pieces[Idx].UsedIn = { TEXT("hawkgirl"), TEXT("hawkman") };                         // nth_metal
	Idx++; Pieces[Idx].UsedIn = { TEXT("deathstroke") };                                        // promethium

	Idx++; Pieces[Idx].UsedIn = { TEXT("silver_surfer") };                                      // power_cosmic
	Idx++; Pieces[Idx].UsedIn = { TEXT("jean_grey"), TEXT("scarlet_witch") };                  // phoenix_shard
	Idx++; Pieces[Idx].UsedIn = { TEXT("cap_marvel") };                                         // tesseract
	Idx++; Pieces[Idx].UsedIn = { TEXT("darkseid") };                                           // omega_beam
	Idx++; Pieces[Idx].UsedIn = { TEXT("lex_luthor") };                                         // kryptonite
	Idx++; Pieces[Idx].UsedIn = { TEXT("superman"), TEXT("supergirl") };                        // kryptonian

	Idx++; Pieces[Idx].UsedIn = { TEXT("scarlet_witch") };                                      // chaos_magic
	Idx++; Pieces[Idx].UsedIn = { TEXT("doctor_strange"), TEXT("wong") };                       // sling_ring
	Idx++; Pieces[Idx].UsedIn = { TEXT("doctor_strange") };                                     // eye
	Idx++; Pieces[Idx].UsedIn = { TEXT("eragon"), TEXT("dragon_rider") };                       // rune
	Idx++; Pieces[Idx].UsedIn = { TEXT("red_hood"), TEXT("ras_al_ghul") };                      // lazarus
	Idx++; Pieces[Idx].UsedIn = { TEXT("scarlet_witch") };                                      // darkhold
	Idx++; Pieces[Idx].UsedIn = { TEXT("doctor_fate") };                                        // helmet

	Idx++; Pieces[Idx].UsedIn = { TEXT("venom"), TEXT("carnage") };                             // symbiote
	Idx++; Pieces[Idx].UsedIn = { TEXT("cap"), TEXT("deathstroke") };                           // serum
	Idx++; Pieces[Idx].UsedIn = { TEXT("hulk"), TEXT("she_hulk") };                             // gamma
	Idx++; Pieces[Idx].UsedIn = { TEXT("wolverine"), TEXT("deadpool") };                        // healing
	Idx++; Pieces[Idx].UsedIn = { TEXT("martian_manhunter") };                                  // martian
	Idx++; Pieces[Idx].UsedIn = { TEXT("ant_man"), TEXT("wasp") };                              // pym
	Idx++; Pieces[Idx].UsedIn = { TEXT("spider_man"), TEXT("miles_morales") };                  // web

	Idx++; Pieces[Idx].UsedIn = { TEXT("thanos") };                                             // gauntlet
	Idx++; Pieces[Idx].UsedIn = { TEXT("thanos") };                                             // power_stone
	Idx++; Pieces[Idx].UsedIn = { TEXT("flash"), TEXT("reverse_flash") };                       // speed_force
	Idx++; Pieces[Idx].UsedIn = { TEXT("green_lantern"), TEXT("john_stewart") };                // ring
	Idx++; Pieces[Idx].UsedIn = { TEXT("darkseid") };                                           // anti_life
	Idx++; Pieces[Idx].UsedIn = { TEXT("thor"), TEXT("storm") };                                // storm_essence
}

void UTradeSystem::RefreshOffers()
{
	Offers.Reset();
	// Generate 8 random offers from pieces the player doesn't yet have max of
	for (int32 i = 0; i < FMath::Min(8, Pieces.Num()); ++i)
	{
		int32 Pick = FMath::RandRange(0, Pieces.Num() - 1);
		FTradeOffer O;
		O.ItemId = Pieces[Pick].Id;
		O.AskPrice = Pieces[Pick].BuyPrice + FMath::RandRange(-1000, 2000);
		O.Quantity = FMath::RandRange(1, 3);
		O.Seller = FMath::RandBool() ? TEXT("Market") : TEXT("Trader");
		Offers.Add(O);
	}
}

bool UTradeSystem::BuyOffer(int32 OfferIndex)
{
	if (OfferIndex < 0 || OfferIndex >= Offers.Num()) return false;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Offers[OfferIndex].AskPrice) return false;
	Sess->AddCurrency(ECurrency::Credits, -Offers[OfferIndex].AskPrice);
	AddPiece(Offers[OfferIndex].ItemId, Offers[OfferIndex].Quantity);
	Sess->ShowMessage(FString::Printf(TEXT("Purchased: %s x%d for %d credits."), *Offers[OfferIndex].ItemId, Offers[OfferIndex].Quantity, Offers[OfferIndex].AskPrice));
	Offers.RemoveAt(OfferIndex);
	return true;
}

bool UTradeSystem::HasPiece(const FString& Id) const { return OwnedPieces.FindRef(Id) > 0; }
int32 UTradeSystem::GetPieceCount(const FString& Id) const { return OwnedPieces.FindRef(Id); }

void UTradeSystem::AddPiece(const FString& Id, int32 Count) { OwnedPieces.FindOrAdd(Id) += Count; }

bool UTradeSystem::BuyPiece(const FString& Id)
{
	const FTradeablePiece* P = FindPiece(Id);
	if (!P) return false;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < P->BuyPrice) return false;
	Sess->AddCurrency(ECurrency::Credits, -P->BuyPrice);
	AddPiece(Id);
	Sess->ShowMessage(FString::Printf(TEXT("Bought %s for %d credits."), *P->Name, P->BuyPrice));
	return true;
}

bool UTradeSystem::SellPiece(const FString& Id)
{
	if (!HasPiece(Id)) return false;
	const FTradeablePiece* P = FindPiece(Id);
	if (!P) return false;
	OwnedPieces[Id]--;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) { Sess->AddCurrency(ECurrency::Credits, P->SellPrice); Sess->ShowMessage(FString::Printf(TEXT("Sold %s for %d credits."), *P->Name, P->SellPrice)); }
	return true;
}

const FTradeablePiece* UTradeSystem::FindPiece(const FString& Id) const
{
	for (auto& P : Pieces) if (P.Id == Id) return &P;
	return nullptr;
}

bool UTradeSystem::CanCraftBlueprint(const FString& BlueprintId) const
{
	// Check if player has all pieces needed
	for (auto& P : Pieces)
	{
		if (P.UsedIn.Contains(BlueprintId))
		{
			if (!HasPiece(P.Id)) return false;
		}
	}
	return true;
}

FString UTradeSystem::GetMissingPieces(const FString& BlueprintId) const
{
	FString Missing;
	for (auto& P : Pieces)
	{
		if (P.UsedIn.Contains(BlueprintId) && !HasPiece(P.Id))
		{
			Missing += FString::Printf(TEXT("  Missing: %s (%s — %d credits in market)\n"), *P.Name, *P.Category, P.BuyPrice);
		}
	}
	return Missing.IsEmpty() ? TEXT("All pieces collected! You can craft this blueprint.") : Missing;
}
