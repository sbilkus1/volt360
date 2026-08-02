#include "Core/ChimeraWeapons.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraTrading.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UWeaponSystem::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C); SeedCatalog(); }

void UWeaponSystem::SeedCatalog()
{
	auto W = [&](const TCHAR* Id, const TCHAR* Name, const TCHAR* Uni, const TCHAR* Wielder, EWeaponClass Cls, int32 Dmg, float Spd, const TCHAR* Mat, const TCHAR* Col, const TCHAR* Spec, const TCHAR* Lore, int32 Cost) {
		FWeaponDef D; D.Id = Id; D.Name = Name; D.Universe = Uni; D.Wielder = Wielder; D.Class = Cls; D.Damage = Dmg; D.Speed = Spd;
		D.Material = Mat; D.Color = Col; D.Special = Spec; D.Lore = Lore; D.CreditCost = Cost; Catalog.Add(D);
	};

	// ======================================================================
	// ERAGON — EVERY DRAGON RIDER SWORD (the 12 known Rider blades)
	// ======================================================================
	// Rider swords are forged from brightsteel (meteorite metal) by Rhunon the elf smith.
	// Each blade's color matches the Rider's dragon. The sword chooses the Rider.

	W(TEXT("zarroc"), TEXT("Zar'roc — Misery"), TEXT("Eragon"), TEXT("Morzan → Eragon → Murtagh"), EWeaponClass::Sword, 55, 1.1f,
		TEXT("Brightsteel"), TEXT("Blood Red"), TEXT("Perfectly balanced. +10 damage vs Empire soldiers. Carries the weight of its previous owner."), TEXT("Forged for Morzan, the first and most terrible of the Forsworn. Passed to Eragon by Brom. Lost to Murtagh at the Burning Plains. 'Misery' in the Ancient Language."), 8000);

	W(TEXT("brisingr"), TEXT("Brisingr — Fire"), TEXT("Eragon"), TEXT("Eragon Shadeslayer"), EWeaponClass::Sword, 70, 1.2f,
		TEXT("Brightsteel"), TEXT("Sapphire Blue"), TEXT("Bursts into blue flame on command. Ignites instantly when Eragon speaks its true name. 'Brisingr!' — the blade erupts. Unblockable while flaming."), TEXT("Forged by Rhunon for Eragon after Zar'roc was lost. Eragon named it during the forging. The sword caught fire. Rhunon: 'I have never made a sword like this.'"), 20000);

	W(TEXT("arvindr"), TEXT("Arvindr"), TEXT("Eragon"), TEXT("Brom"), EWeaponClass::Sword, 45, 1.0f,
		TEXT("Brightsteel"), TEXT("Gold"), TEXT("The sword of a mentor. +10% XP gain while wielded. Wisdom of the old Rider flows through it."), TEXT("Brom's Rider sword. The pommel was set with a gold jewel. Lost when the Forsworn killed his dragon Saphira I. Brom never drew it again — until he needed to save Eragon."), 10000);

	W(TEXT("naegling"), TEXT("Naegling"), TEXT("Eragon"), TEXT("Oromis"), EWeaponClass::Sword, 60, 0.9f,
		TEXT("Brightsteel"), TEXT("Bronze-Gold"), TEXT("Stores up to 200 energy from the wielder over time. Release all stored energy in one devastating strike. 'Do not waste what you have gathered.'"), TEXT("Oromis's sword. Glaedr's golden Rider blade. Wielded by the last of the old order. Carries centuries of technique."), 15000);

	W(TEXT("undbitr"), TEXT("Undbitr"), TEXT("Eragon"), TEXT("Brom (first sword)"), EWeaponClass::Sword, 38, 1.0f,
		TEXT("Brightsteel"), TEXT("Silver"), TEXT("Brom's first blade. Lost with his dragon. Simple. Honest. Deadly in the right hands."), TEXT("Brom's original Rider sword before Arvindr. Wielded in the golden age of the Riders."), 6000);

	W(TEXT("tamerlein"), TEXT("Tamerlein"), TEXT("Eragon"), TEXT("Arya Dröttningu"), EWeaponClass::Sword, 62, 1.3f,
		TEXT("Brightsteel"), TEXT("Forest Green"), TEXT("Elven agility: +20% attack speed. The blade moves as fast as thought. Perfect for Arya's lightning-quick style."), TEXT("Arya's Rider sword. Green as the elf's forest home. Wielded by the princess who became queen, the courier who became Rider."), 14000);

	W(TEXT("islingr"), TEXT("Islingr — Light-Bringer"), TEXT("Eragon"), TEXT("Vrael → Galbatorix"), EWeaponClass::Sword, 75, 0.8f,
		TEXT("Brightsteel"), TEXT("White"), TEXT("White as dragon fire. +30 damage vs other Riders. Once wielded by the leader of the Riders. Now a symbol of everything that was lost."), TEXT("Vrael's sword. Leader of the Dragon Riders before the Fall. Galbatorix took this blade when he killed Vrael. Renamed it 'Vrangr' — Awry. The corrupted blade of a corrupted king."), 25000);

	W(TEXT("riders_blade_generic"), TEXT("Rider's Blade — Unmarked"), TEXT("Eragon"), TEXT("Fallen Rider"), EWeaponClass::Sword, 50, 1.0f,
		TEXT("Brightsteel"), TEXT("Variable"), TEXT("The color shifts to match your bonded dragon. Bonds grow 10% faster while this blade is drawn."), TEXT("The swords of the 13 Riders whose names were lost in the Fall. Their blades remain. Their dragons are remembered in the color."), 5000);

	// ======================================================================
	// DIVERGENT — EVERY WEAPON FROM THE SERIES
	// ======================================================================
	W(TEXT("tris_knife"), TEXT("Tris's Initiation Knife"), TEXT("Divergent"), TEXT("Tris Prior"), EWeaponClass::Dagger, 18, 1.5f,
		TEXT("Dauntless steel"), TEXT("Silver"), TEXT("Wielded during her first fight against Peter. +5 damage when below 50% HP — desperation sharpens the will."), TEXT("The knife Tris chose on her first day in Dauntless. She held it against Peter. She held it against herself in the fear landscape. It saved her life more times than she could count."), 2000);

	W(TEXT("four_knife"), TEXT("Four's Throwing Knife Set"), TEXT("Divergent"), TEXT("Tobias Eaton (Four)"), EWeaponClass::Thrown, 22, 1.3f,
		TEXT("Weighted steel"), TEXT("Black"), TEXT("Set of 3 throwing knives. Returns on kill. Four's precise aim — trained from childhood."), TEXT("Four learned to throw knives from his father, Marcus Eaton. He hates that he's good at it. He uses them to protect, never to intimidate."), 3500);

	W(TEXT("peter_knife"), TEXT("Peter's Coward Blade"), TEXT("Divergent"), TEXT("Peter Hayes"), EWeaponClass::Dagger, 15, 1.4f,
		TEXT("Standard steel"), TEXT("Grey"), TEXT("Backstab bonus: +30 damage when attacking from behind. The weapon of a coward."), TEXT("Peter stabbed Edward in the eye during initiation to eliminate competition. He never fought fair. This knife is tainted."), 1000);

	W(TEXT("erudite_pistol"), TEXT("Erudite Standard-Issue Pistol"), TEXT("Divergent"), TEXT("Erudite soldiers"), EWeaponClass::Gun, 35, 1.0f,
		TEXT("Steel/polymer"), TEXT("Grey"), TEXT("Semi-automatic. 12-round magazine. The weapon of the simulation army."), TEXT("Jeanine Matthews armed her soldiers with these. They fired on Abnegation. They fired on Dauntless. They fired on innocence."), 3000);

	W(TEXT("dauntless_rifle"), TEXT("Dauntless Combat Rifle"), TEXT("Divergent"), TEXT("Dauntless soldiers"), EWeaponClass::Rifle, 45, 0.7f,
		TEXT("Steel"), TEXT("Black"), TEXT("Full auto. 30-round magazine. The roar of Dauntless in battle. Accuracy improves with combat skill."), TEXT("Dauntless soldiers carried these when Eric ordered them to march on Abnegation. Some of them refused. Some of them didn't."), 5000);

	W(TEXT("simulation_serum"), TEXT("Simulation Serum Injector"), TEXT("Divergent"), TEXT("Jeanine Matthews"), EWeaponClass::Serum, 0, 0.5f,
		TEXT("Chemical"), TEXT("Blue"), TEXT("Injects the target with simulation serum. They see their worst fears. Disables them for 30 seconds. Works on non-Divergents only."), TEXT("Jeanine's masterpiece. The serum that controlled Dauntless. Tris was immune. That's how she knew she was Divergent."), 4000);

	W(TEXT("memory_serum"), TEXT("Memory Serum — The Wipe"), TEXT("Divergent"), TEXT("Bureau of Genetic Welfare"), EWeaponClass::Serum, 0, 0.3f,
		TEXT("Chemical"), TEXT("Purple"), TEXT("Erases the target's memories. Permanent. Irreversible. The Bureau's ultimate weapon against the Divergent."), TEXT("The serum David tried to use on Tris. Caleb volunteered to be wiped instead. Four watched his mother deploy this. No one was the same after."), 8000);

	W(TEXT("death_serum"), TEXT("Death Serum — The Final Injection"), TEXT("Divergent"), TEXT("Bureau of Genetic Welfare"), EWeaponClass::Serum, 999, 0.1f,
		TEXT("Chemical"), TEXT("Black"), TEXT("Instant death. No saving throw. The Bureau's execution method. Reserved for the gravest threats."), TEXT("The serum that killed... the Bureau's darkest secret. One injection. No pain. No warning. Just ending."), 15000);

	// ======================================================================
	// HUNGER GAMES — DISTRICT WEAPONS
	// ======================================================================
	W(TEXT("katniss_bow"), TEXT("Katniss's Hunting Bow"), TEXT("Hunger Games"), TEXT("Katniss Everdeen"), EWeaponClass::Bow, 32, 0.8f,
		TEXT("Wood/sinew"), TEXT("Brown"), TEXT("Silent shot. +20% critical hit chance in wilderness. The weapon that fed her family. The weapon that started a revolution."), TEXT("Her father made this bow. She used it to hunt in the woods beyond the fence. She used it to shoot the apple out of the Gamemakers' pig's mouth. She used it to aim at Snow's heart. She used it on Coin."), 5000);

	W(TEXT("peeta_camo"), TEXT("Peeta's Camouflage Kit"), TEXT("Hunger Games"), TEXT("Peeta Mellark"), EWeaponClass::Tool, 0, 0.5f,
		TEXT("Paint/earth"), TEXT("Variable"), TEXT("Camouflage: become nearly invisible for 30 seconds. Peeta's arena survival skill — the thing that kept him alive."), TEXT("Peeta decorated cakes in the bakery. In the arena, he decorated himself. The Gamemakers gave him an 8 for this skill. They should have given him a 12."), 800);

	W(TEXT("gale_snare"), TEXT("Gale's Snare Wire"), TEXT("Hunger Games"), TEXT("Gale Hawthorne"), EWeaponClass::Tool, 10, 0.5f,
		TEXT("Wire"), TEXT("Silver"), TEXT("Set a trap. Enemy walks in, takes 30 damage, immobilised for 5 seconds. Gale's hunting skill meets rebellion tactics."), TEXT("Gale set snares in the woods since he was old enough to hold wire. In the rebellion, he turned that skill into weapons. The bomb that killed Prim was his design."), 1200);

	W(TEXT("finnick_trident"), TEXT("Finnick's Trident"), TEXT("Hunger Games"), TEXT("Finnick Odair"), EWeaponClass::Spear, 48, 0.9f,
		TEXT("District 4 steel"), TEXT("Gold"), TEXT("Can be thrown and retrieved. +20 damage in water. The golden boy's golden weapon."), TEXT("Finnick won the 65th Hunger Games at age 14 with this trident. He used a net and trident combination that no one could beat. The youngest victor in history."), 7000);

	W(TEXT("johanna_axe"), TEXT("Johanna's Axe"), TEXT("Hunger Games"), TEXT("Johanna Mason"), EWeaponClass::Axe, 55, 0.6f,
		TEXT("District 7 steel"), TEXT("Grey"), TEXT("Savage blow: ignores 30% armor. Johanna doesn't do subtle. Johanna does survival."), TEXT("Johanna won her Games pretending to be weak. When the arena was down to the final tributes, she revealed she was the deadliest one there. She's been wielding an axe ever since."), 6000);

	W(TEXT("cato_sword"), TEXT("Cato's Gladius"), TEXT("Hunger Games"), TEXT("Cato"), EWeaponClass::Sword, 42, 1.1f,
		TEXT("District 2 steel"), TEXT("Silver"), TEXT("Career tribute weapon. Bleed effect on hit. Cato trained his whole life for the Games."), TEXT("Cato was the favourite to win the 74th Games. District 2's greatest tribute. He almost beat Katniss and Peeta. The muttations got him. He screamed all night."), 4000);

	W(TEXT("rue_slingshot"), TEXT("Rue's Slingshot"), TEXT("Hunger Games"), TEXT("Rue"), EWeaponClass::Thrown, 8, 1.8f,
		TEXT("Wood"), TEXT("Brown"), TEXT("Distraction weapon. Hit causes enemy to investigate the impact point for 3 seconds. Rue's gift."), TEXT("Rue couldn't fight the Careers. So she outsmarted them. She moved through the trees like a bird. Her slingshot never killed anyone. It saved Katniss instead."), 500);

	// ======================================================================
	// GREY'S ANATOMY — SURGICAL TOOLS AS WEAPONS
	// ======================================================================
	W(TEXT("scalpel_10"), TEXT("Meredith's #10 Scalpel"), TEXT("Greys"), TEXT("Meredith Grey"), EWeaponClass::Dagger, 12, 2.0f,
		TEXT("Surgical steel"), TEXT("Silver"), TEXT("Extremely fast. Ignores armor (finds the gap). The surgeon's blade. Precision over power."), TEXT("Meredith held this scalpel during her first solo surgery. She held it during the plane crash rescue. She held it when she operated on the man who shot the hospital. Surgeons don't carry weapons. They carry this."), 1500);

	W(TEXT("bone_saw"), TEXT("Ortho Bone Saw"), TEXT("Greys"), TEXT("Callie Torres"), EWeaponClass::Tool, 25, 0.4f,
		TEXT("Surgical steel"), TEXT("Silver"), TEXT("Heavy. Slow. Devastating against constructs and robots. 'It's ortho. It's not subtle.'"), TEXT("Callie Torres built bones from titanium and love. She took this saw into surgery every day. When the hospital was attacked, she was ready to use it differently."), 2000);

	W(TEXT("defibrillator"), TEXT("Crash Cart Defibrillator"), TEXT("Greys"), TEXT("Any surgeon"), EWeaponClass::Blunt, 20, 0.6f,
		TEXT("Medical"), TEXT("Red"), TEXT("Shock attack: stuns enemy for 2 seconds. 'CLEAR!' Can also revive downed allies. Charge: 10 seconds."), TEXT("Every code blue. Every 'CLEAR!' Every patient who came back. This machine has restarted more hearts than any weapon has stopped."), 3000);

	// ======================================================================
	// THE ROOKIE — NPD SIDEARMS
	// ======================================================================
	W(TEXT("glock_17"), TEXT("NPD Standard Issue Glock 17"), TEXT("The Rookie"), TEXT("John Nolan"), EWeaponClass::Gun, 28, 1.0f,
		TEXT("Steel/polymer"), TEXT("Black"), TEXT("17-round magazine. Standard issue for all NPD officers. Reliable. Familiar. Every cop's first sidearm."), TEXT("The first gun Nolan qualified with as a rookie at age 45. The same model Lucy Chen carries. The same model Talia Bishop carried. The same model that stopped Rosalind Dyer."), 3000);

	W(TEXT("taser_x26"), TEXT("Taser X26 — Less-Lethal"), TEXT("The Rookie"), TEXT("NPD patrol"), EWeaponClass::Gun, 5, 0.8f,
		TEXT("Plastic"), TEXT("Yellow"), TEXT("Stuns target for 8 seconds. No lethal damage. Cannot kill. Required before escalating to firearm. NPD use-of-force continuum."), TEXT("'Taser! Taser! Taser!' The three-word warning every NPD officer shouts before deploying. Non-lethal. Sometimes it doesn't work. Then the Glock comes out."), 1500);

	W(TEXT("tactical_baton"), TEXT("NPD Expandable Baton"), TEXT("The Rookie"), TEXT("Tim Bradford"), EWeaponClass::Blunt, 18, 1.3f,
		TEXT("Steel"), TEXT("Black"), TEXT("Quick melee. Tim's preferred close-quarters tool. 'Don't pull your gun if you can use your baton.'"), TEXT("Bradford drills this into every rookie: the baton solves 90% of problems. The other 10% need backup. Nolan learned this the hard way on day one."), 1200);

	W(TEXT("beanbag_shotgun"), TEXT("Less-Lethal Beanbag Shotgun"), TEXT("The Rookie"), TEXT("NPD SWAT"), EWeaponClass::Shotgun, 22, 0.5f,
		TEXT("Steel"), TEXT("Green"), TEXT("Knockdown on hit. Non-lethal but painful. SWAT uses this before breaching. 'You will comply.'"), TEXT("The beanbag round hits with the force of a major league fastball. It won't kill you. You'll wish it did. SWAT's first option before deadly force."), 4000);

	// ======================================================================
	// LEGENDARY WEAPONS — cross-universe, ultra-rare
	// ======================================================================

	W(TEXT("excalibur"), TEXT("Excalibur — The Sword in the Stone"), TEXT("Legend"), TEXT("Arthur Pendragon"), EWeaponClass::Sword, 90, 0.7f,
		TEXT("Unknown"), TEXT("Golden"), TEXT("Only the worthy may draw this blade. Deals double damage to the unjust. Glows when a true threat approaches."), TEXT("Pulled from stone. Forged in myth. In Nexopolis, it rests in the Library of Lost Worlds. The Keepers say it's waiting for the Convergence's final choice."), 100000);

	W(TEXT("blade_of_olympus"), TEXT("Blade of Olympus"), TEXT("Legend"), TEXT("Zeus"), EWeaponClass::Sword, 100, 0.4f,
		TEXT("Divine"), TEXT("White/Gold"), TEXT("God-killer. One strike. 60-second cooldown. Kratos carried this. Zeus feared it. You can barely lift it."), TEXT("The weapon that ended the Titanomachy. The weapon that Kratos turned on Olympus itself. Somehow it survived the Crash."), 200000);

	W(TEXT("lightsaber_blue"), TEXT("Lightsaber — Guardian Blue"), TEXT("Legend"), TEXT("Obi-Wan Kenobi"), EWeaponClass::Sword, 65, 1.5f,
		TEXT("Kyber crystal/plasma"), TEXT("Blue"), TEXT("Energy blade. Cuts through anything. Deflects blaster bolts. The weapon of a more elegant age from a galaxy far, far away that the Crash may have touched."), TEXT("A blue glow in the Library of Lost Worlds. The Keepers don't know where it came from. 'An elegant weapon for a more civilized age.'"), 75000);

	// ======================================================================
	// GENERAL WEAPONS — available at armories across all rings
	// ======================================================================
	W(TEXT("combat_knife"), TEXT("Combat Knife"), TEXT("General"), TEXT("Anyone"), EWeaponClass::Dagger, 15, 1.5f,
		TEXT("Steel"), TEXT("Silver"), TEXT("Standard issue. Reliable. Sharp. +5 damage when stealthed."), TEXT("Every soldier, every cop, every survivor carries one of these. It's not special. It just works."), 500);

	W(TEXT("fire_axe"), TEXT("Fire Axe"), TEXT("General"), TEXT("Anyone"), EWeaponClass::Axe, 30, 0.5f,
		TEXT("Steel/wood"), TEXT("Red/Black"), TEXT("Breaks doors. Breaks bones. Emergency services standard. The firehouse classic."), TEXT("Found on the back of every fire truck. Also found in the hands of survivors during the Crash."), 800);

	W(TEXT("crowbar"), TEXT("Crowbar — The Universal Key"), TEXT("General"), TEXT("Gordon Freeman"), EWeaponClass::Blunt, 20, 1.2f,
		TEXT("Steel"), TEXT("Orange"), TEXT("Opens crates. Opens doors. Opens skulls. The most versatile tool in any reality. A silent protagonist somewhere approves."), TEXT("The Crash scattered these everywhere. They unlock things. They break things. They're just crowbars. And they're perfect."), 300);
}

const FWeaponDef* UWeaponSystem::Find(const FString& Id) const { for (auto& W : Catalog) if (W.Id == Id) return &W; return nullptr; }

bool UWeaponSystem::CraftWeapon(const FString& Id)
{
	auto* W = const_cast<FWeaponDef*>(Find(Id));
	if (!W || W->bCrafted) return false;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < W->CreditCost) return false;
	auto* Trade = GetGameInstance()->GetSubsystem<UTradeSystem>();
	for (auto& P : W->RequiredPieces) if (!Trade || !Trade->HasPiece(P)) return false;
	for (auto& P : W->RequiredPieces) Trade->SellPiece(P);
	Sess->AddCurrency(ECurrency::Credits, -W->CreditCost);
	W->bCrafted = true;
	Sess->ShowMessage(FString::Printf(TEXT("WEAPON CRAFTED: %s — %s"), *W->Name, *W->Lore));
	return true;
}

void UWeaponSystem::Equip(const FString& Id) { if (Find(Id)) EquippedWeapon = Id; }
