// GDD 14 — Complete Iron Man Armor Catalog. Every single Mark I-LXXXV + comic specials.
// Progression tree: craft earlier suits to unlock later ones. Pieces from trading system.

#include "Core/ChimeraIronMan.h"

void ChimeraIronMan::BuildCompleteCatalog(TArray<FIronManArmor>& A)
{
	FIronManArmor IM;
	auto Add = [&](const TCHAR* Id, const TCHAR* Mark, const TCHAR* Name, int32 Yr, const TCHAR* Era, int32 Ar, int32 Mo, int32 Pw, int32 St, int32 Cost, const TCHAR* Ability, std::initializer_list<const TCHAR*> Pieces, std::initializer_list<const TCHAR*> Prereqs) {
		IM = {}; IM.Id = Id; IM.Mark = Mark; IM.Name = Name; IM.Year = Yr; IM.Era = Era;
		IM.ArmorStat = Ar; IM.MobilityStat = Mo; IM.PowerStat = Pw; IM.StealthStat = St;
		IM.CreditCost = Cost; IM.SpecialAbility = Ability;
		for (auto& P : Pieces) IM.RequiredPieces.Add(P);
		for (auto& P : Prereqs) IM.Prerequisites.Add(P);
		A.Add(IM);
	};

	// ===== CLASSIC COMIC ERA =====
	Add(TEXT("mk_model1"), TEXT("Model 1"), TEXT("Grey Armor — Built in a Cave"), 1963, TEXT("Classic"), 4,2,2,1, 5000, TEXT("Built with a box of scraps. Chest-mounted unibeam. Heavy. Slow. Indestructible spirit."), {TEXT("arc_reactor")}, {});
	Add(TEXT("mk_model2"), TEXT("Model 2"), TEXT("Golden Avenger"), 1964, TEXT("Classic"), 4,3,3,1, 8000, TEXT("First repulsor tech. All gold. Intimidating. No flight stabilisers yet."), {TEXT("arc_reactor"),TEXT("repulsor_coil")}, {TEXT("mk_model1")});
	Add(TEXT("mk_model3"), TEXT("Model 3"), TEXT("Classic Red & Gold"), 1964, TEXT("Classic"), 5,4,4,1, 10000, TEXT("The iconic red and gold. Sleeve repulsors. Unibeam projector. Iron Man is born."), {TEXT("arc_reactor"),TEXT("repulsor_coil")}, {TEXT("mk_model2")});
	Add(TEXT("mk_model4"), TEXT("Model 4"), TEXT("Refined Classic"), 1970, TEXT("Classic"), 5,5,5,1, 12000, TEXT("Better power distribution. First onboard computer assist. Lighter. Faster."), {TEXT("arc_reactor"),TEXT("ai_chip")}, {TEXT("mk_model3")});
	Add(TEXT("mk_space1"), TEXT("Space Mk I"), TEXT("Deep Space Suit"), 1970, TEXT("Classic"), 5,6,5,3, 15000, TEXT("Vacuum-sealed. 72hr oxygen. First suit to leave Earth's orbit."), {TEXT("arc_reactor"),TEXT("tesseract_energy")}, {TEXT("mk_model3")});
	Add(TEXT("mk_stealth1"), TEXT("Stealth Mk I"), TEXT("Blue Ghost"), 1981, TEXT("Classic"), 3,5,3,9, 15000, TEXT("Radar-absorbent. Silent flight. Thermal masking. Invisible to all sensors."), {TEXT("arc_reactor"),TEXT("ai_chip")}, {TEXT("mk_model4")});
	Add(TEXT("mk_hydro1"), TEXT("Hydro Armor"), TEXT("Underwater Suit"), 1981, TEXT("Classic"), 5,4,4,5, 14000, TEXT("4000m depth rating. Sonar. Electric eel shock defense. Hydro-jets."), {TEXT("arc_reactor"),TEXT("vibranium")}, {TEXT("mk_model4")});
	Add(TEXT("mk_silver"), TEXT("Silver Centurion"), TEXT("Chameleon Armor"), 1985, TEXT("Classic"), 6,5,6,2, 20000, TEXT("Silver and crimson. Chameleon mode bends light. Pulse bolts. Shoulder weapons."), {TEXT("arc_reactor"),TEXT("ai_chip"),TEXT("repulsor_coil")}, {TEXT("mk_model4")});
	Add(TEXT("mk_modular"), TEXT("Modular Armor"), TEXT("War Machine Proto"), 1992, TEXT("Modular"), 7,6,6,2, 25000, TEXT("Hot-swappable weapon modules. Gatling. Missile pods. War Machine prototype."), {TEXT("arc_reactor"),TEXT("wrist_blaster"),TEXT("ai_chip")}, {TEXT("mk_silver")});
	Add(TEXT("mk_hb1"), TEXT("Hulkbuster Mk I"), TEXT("Original"), 1994, TEXT("Modular"), 9,3,8,1, 50000, TEXT("Built to fight Hulk. Reinforced everything. Auto-repair. Banner's nightmare."), {TEXT("arc_reactor"),TEXT("adamantium"),TEXT("gamma_isotope")}, {TEXT("mk_modular")});
	Add(TEXT("mk_arctic"), TEXT("Arctic Armor"), TEXT("Sub-Zero"), 1996, TEXT("Modular"), 5,5,4,4, 12000, TEXT("-200 operational. Heating coils in every plate. Ice shatters on contact."), {TEXT("arc_reactor")}, {TEXT("mk_modular")});
	Add(TEXT("mk_extremis"), TEXT("Extremis"), TEXT("Nanotech Genesis"), 2005, TEXT("Extremis"), 7,8,7,4, 40000, TEXT("Stored in Tony's bones. Deploys from body. Direct brain-to-armor neural link."), {TEXT("arc_reactor"),TEXT("nanotube_alloy"),TEXT("ai_chip"),TEXT("super_soldier_serum")}, {TEXT("mk_silver")});
	Add(TEXT("mk_bleeding"), TEXT("Bleeding Edge"), TEXT("Liquid Metal"), 2010, TEXT("Extremis"), 8,9,8,6, 60000, TEXT("Liquid smart-metal. Shapes any weapon. Repairs mid-combat. Unstoppable."), {TEXT("arc_reactor"),TEXT("nanotube_alloy"),TEXT("repulsor_coil"),TEXT("ai_chip")}, {TEXT("mk_extremis")});
	Add(TEXT("mk_endosym"), TEXT("Endo-Sym"), TEXT("Symbiote Hybrid"), 2014, TEXT("Extremis"), 7,9,8,7, 75000, TEXT("Liquid metal bonded with symbiote biology. Shapeshift. Weak to sonic/fire."), {TEXT("arc_reactor"),TEXT("symbiote_sample"),TEXT("nanotube_alloy")}, {TEXT("mk_bleeding")});
	Add(TEXT("mk_thorbuster"), TEXT("Thorbuster"), TEXT("Asgardian Counter"), 2011, TEXT("Extremis"), 9,6,10,1, 120000, TEXT("Powered by Asgardian crystal. Absorbs Mjolnir lightning. Fights gods."), {TEXT("arc_reactor"),TEXT("uru_metal"),TEXT("storm_essence"),TEXT("adamantium")}, {TEXT("mk_extremis"),TEXT("mk_hb1")});
	Add(TEXT("mk_phoenixbuster"), TEXT("Phoenix Buster"), TEXT("Cosmic Counter"), 2012, TEXT("Extremis"), 8,6,10,1, 200000, TEXT("Fights the Phoenix Force. Splits cosmic fire. Barely worked. Still legendary."), {TEXT("arc_reactor"),TEXT("phoenix_shard"),TEXT("power_cosmic")}, {TEXT("mk_thorbuster")});
	Add(TEXT("mk_hyper"), TEXT("Hypervelocity"), TEXT("Mach 10"), 2008, TEXT("Extremis"), 5,10,6,3, 55000, TEXT("Mach 10. Tony uploaded consciousness while body healed. Became pure data."), {TEXT("arc_reactor"),TEXT("speed_force_conduit")}, {TEXT("mk_extremis")});
	Add(TEXT("mk_godkiller"), TEXT("Godkiller"), TEXT("Celestial Slayer"), 2013, TEXT("Extremis"), 9,7,10,1, 500000, TEXT("Colossal. Planet-cracking weapons. Kills Celestials. One suit. One god. One winner."), {TEXT("arc_reactor"),TEXT("power_cosmic"),TEXT("uru_metal"),TEXT("mother_box")}, {TEXT("mk_phoenixbuster"),TEXT("mk_thorbuster")});

	// ===== MCU — EVERY SINGLE MARK I–LXXXV =====
	// Marks I-II: origin armors
	Add(TEXT("mcu_mk1"), TEXT("Mark I"), TEXT("Cave Escape Armor"), 2008, TEXT("MCU Phase 1"), 3,1,2,1, 3000, TEXT("Built in a cave. WITH A BOX OF SCRAPS. Flamethrower. Rocket boots. Barely flies."), {TEXT("arc_reactor")}, {});
	Add(TEXT("mcu_mk2"), TEXT("Mark II"), TEXT("First Flight Prototype"), 2008, TEXT("MCU Phase 1"), 4,6,4,1, 5000, TEXT("Chrome test suit. Icing problem at altitude. First to fly. 'How'd you solve the icing problem?'"), {TEXT("arc_reactor"),TEXT("repulsor_coil")}, {TEXT("mcu_mk1")});
	Add(TEXT("mcu_mk3"), TEXT("Mark III"), TEXT("Battle of Gulmira"), 2008, TEXT("MCU Phase 1"), 5,5,5,2, 10000, TEXT("First red and gold. Hot-rod red. Tank missile. 'Yeah, I can fly.'"), {TEXT("arc_reactor"),TEXT("repulsor_coil"),TEXT("ai_chip")}, {TEXT("mcu_mk2")});
	Add(TEXT("mcu_mk4"), TEXT("Mark IV"), TEXT("Expo Suit"), 2010, TEXT("MCU Phase 1"), 5,5,5,2, 8000, TEXT("Stark Expo entrance suit. Cleaner lines. Better flight. 'It's good to be back.'"), {TEXT("arc_reactor")}, {TEXT("mcu_mk3")});
	Add(TEXT("mcu_mk5"), TEXT("Mark V"), TEXT("Suitcase Suit"), 2010, TEXT("MCU Phase 1"), 3,6,3,3, 12000, TEXT("Portable briefcase armor. Deploys in seconds. Lighter weapons. Saved Tony at the racetrack."), {TEXT("arc_reactor"),TEXT("repulsor_coil")}, {TEXT("mcu_mk3")});
	Add(TEXT("mcu_mk6"), TEXT("Mark VI"), TEXT("Triangle Reactor"), 2010, TEXT("MCU Phase 1"), 6,6,6,2, 15000, TEXT("Triangular arc reactor. Underwater deployment. Laser cutter. Suitcase-portable. Fought Thor."), {TEXT("arc_reactor"),TEXT("vibranium"),TEXT("repulsor_coil")}, {TEXT("mcu_mk5")});
	Add(TEXT("mcu_mk7"), TEXT("Mark VII"), TEXT("Battle of New York"), 2012, TEXT("MCU Phase 1"), 7,7,7,3, 25000, TEXT("Remote-deploy via wristbands. Caught Tony mid-fall. Nuke through the portal. Avengers assembled."), {TEXT("arc_reactor"),TEXT("nanotube_alloy"),TEXT("ai_chip"),TEXT("repulsor_coil")}, {TEXT("mcu_mk6")});

	// Marks VIII-XIV: Iron Legion (House Party Protocol armors)
	Add(TEXT("mcu_mk8"), TEXT("Mark VIII"), TEXT("Iron Legion"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 4000, TEXT("Iron Legion. Improved flight. Modular weapons bay. House Party Protocol."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk9"), TEXT("Mark IX"), TEXT("Iron Legion"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 4000, TEXT("Iron Legion. Enhanced repulsor output. Streamlined aerodynamics."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk10"), TEXT("Mark X"), TEXT("Iron Legion"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 4000, TEXT("Iron Legion. Chest RT upgrade. Improved targeting systems."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk11"), TEXT("Mark XI"), TEXT("Iron Legion"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 4000, TEXT("Iron Legion. Heavier chest armour. Shoulder-mounted cannons."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk12"), TEXT("Mark XII"), TEXT("Iron Legion"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 4000, TEXT("Iron Legion. Dark grey/black finish. Night operations."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk13"), TEXT("Mark XIII"), TEXT("Iron Legion"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 4000, TEXT("Iron Legion. Silver/red. Hybrid propulsion system."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk14"), TEXT("Mark XIV"), TEXT("Iron Legion"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 4000, TEXT("Iron Legion. Lightweight prototype. Minimal armour."), {}, {TEXT("mcu_mk7")});

	// Marks XV-XX: Specialised Iron Legion
	Add(TEXT("mcu_mk15"), TEXT("Mark XV"), TEXT("Sneaky"), 2013, TEXT("MCU Phase 2"), 3,7,4,9, 6000, TEXT("Stealth suit. Nicknamed 'Sneaky.' Angular plates deflect radar. Silent hover."), {TEXT("arc_reactor"),TEXT("ai_chip")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk16"), TEXT("Mark XVI"), TEXT("Nightclub"), 2013, TEXT("MCU Phase 2"), 3,6,4,8, 6000, TEXT("Black stealth armor. 'Nightclub.' Camouflage projection system."), {}, {TEXT("mcu_mk15")});
	Add(TEXT("mcu_mk17"), TEXT("Mark XVII"), TEXT("Heartbreaker"), 2013, TEXT("MCU Phase 2"), 7,7,8,2, 8000, TEXT("Artillery-level RT. Oversized chest repulsor. 'Heartbreaker.' Devastating beam."), {TEXT("arc_reactor"),TEXT("repulsor_coil")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk18"), TEXT("Mark XVIII"), TEXT("Casanova"), 2013, TEXT("MCU Phase 2"), 4,6,4,8, 6000, TEXT("Stealth artillery. 'Casanova.' Black with silver trim. Silent repulsors."), {}, {TEXT("mcu_mk17")});
	Add(TEXT("mcu_mk19"), TEXT("Mark XIX"), TEXT("Tiger"), 2013, TEXT("MCU Phase 2"), 5,10,5,2, 7000, TEXT("High-speed prototype. 'Tiger.' Afterburners. Fastest Legion suit."), {TEXT("arc_reactor"),TEXT("speed_force_conduit")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk20"), TEXT("Mark XX"), TEXT("Python"), 2013, TEXT("MCU Phase 2"), 4,5,4,2, 5000, TEXT("Long-distance suit. 'Python.' Extended flight range. Sub-orbital capable."), {}, {TEXT("mcu_mk7")});

	// Marks XXI-XXX: More Legion + Specialised
	Add(TEXT("mcu_mk21"), TEXT("Mark XXI"), TEXT("Midas"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("High-altitude suit. 'Midas.' Pure gold-titanium alloy. Flashy. Impractical. Tony."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk22"), TEXT("Mark XXII"), TEXT("Hot Rod"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("War Machine prototype colours. 'Hot Rod.' Dark grey with flame decals."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk23"), TEXT("Mark XXIII"), TEXT("Shades"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("Desert camo. 'Shades.' Heat-resistant plating. Sandstorm operational."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk24"), TEXT("Mark XXIV"), TEXT("Tank"), 2013, TEXT("MCU Phase 2"), 8,4,7,1, 7000, TEXT("Heavy combat. 'Tank.' Reinforced everything. Mini-guns in both arms."), {TEXT("arc_reactor"),TEXT("wrist_blaster")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk25"), TEXT("Mark XXV"), TEXT("Striker"), 2013, TEXT("MCU Phase 2"), 7,5,8,2, 9000, TEXT("Pneumatic jackhammer arms. 'Striker.' Punches through concrete. Construction AND combat."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk26"), TEXT("Mark XXVI"), TEXT("Gamma"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("Gamma radiation resistant. Green trim. For Hulk-adjacent operations."), {TEXT("gamma_isotope")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk27"), TEXT("Mark XXVII"), TEXT("Disco"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("Chameleon armor. 'Disco.' Blue and orange. Changes colour at will. For parties and espionage."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk28"), TEXT("Mark XXVIII"), TEXT("Jack"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("Radiation-zone armor. 'Jack.' Full hazmat sealing. Reactor meltdown rated."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk29"), TEXT("Mark XXIX"), TEXT("Fiddler"), 2013, TEXT("MCU Phase 2"), 5,7,5,6, 6000, TEXT("Agile combat. 'Fiddler.' Lightweight construction frame. Evasive maneuvers."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk30"), TEXT("Mark XXX"), TEXT("Blue Steel"), 2013, TEXT("MCU Phase 2"), 6,6,6,2, 6000, TEXT("Silver Centurion homage. 'Blue Steel.' Enhanced chest RT. Beautiful. Deadly."), {}, {TEXT("mcu_mk7")});

	// Marks XXXI-XL: Prehensile + Specialised
	Add(TEXT("mcu_mk31"), TEXT("Mark XXXI"), TEXT("Piston"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("High-speed centrifuge. 'Piston.' Rapid spin attacks. Dizzying."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk32"), TEXT("Mark XXXII"), TEXT("Romeo"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("Enhanced RT. 'Romeo.' Overclocked arc reactor. Burns bright. Burns fast."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk33"), TEXT("Mark XXXIII"), TEXT("Silver Centurion MCU"), 2013, TEXT("MCU Phase 2"), 7,6,7,2, 12000, TEXT("MCU's Silver Centurion. 'Silver.' Enhanced combat. Blade arm. The one that cut through Extremis soldiers."), {TEXT("arc_reactor"),TEXT("repulsor_coil")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk34"), TEXT("Mark XXXIV"), TEXT("Southpaw"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("Left-handed combat. 'Southpaw.' Asymmetrical weapons. Catches enemies off guard."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk35"), TEXT("Mark XXXV"), TEXT("Red Snapper"), 2013, TEXT("MCU Phase 2"), 6,5,7,2, 7000, TEXT("Pincer arms. 'Red Snapper.' Grips and crushes. Disaster rescue specialist."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk36"), TEXT("Mark XXXVI"), TEXT("Peacemaker"), 2013, TEXT("MCU Phase 2"), 5,5,5,2, 5000, TEXT("Riot control. 'Peacemaker.' Sonic pacification. Non-lethal crowd dispersal."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk37"), TEXT("Mark XXXVII"), TEXT("Hammerhead"), 2013, TEXT("MCU Phase 2"), 5,7,5,6, 6000, TEXT("Deep sea armor. 'Hammerhead.' Abyssal depth rated. Crushing pressure resistant."), {TEXT("vibranium")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk38"), TEXT("Mark XXXVIII"), TEXT("Igor"), 2013, TEXT("MCU Phase 2"), 9,2,8,1, 10000, TEXT("Heavy lifting suit. 'Igor.' Not combat. Construction. Can carry a skyscraper girder."), {}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk39"), TEXT("Mark XXXIX"), TEXT("Starboost"), 2013, TEXT("MCU Phase 2"), 6,7,8,4, 10000, TEXT("Sub-orbital suit. 'Starboost.' Gemini. White with gold. Breathtaking. Literally — in space."), {TEXT("arc_reactor"),TEXT("tesseract_energy")}, {TEXT("mcu_mk7")});
	Add(TEXT("mcu_mk40"), TEXT("Mark XL"), TEXT("Shotgun"), 2013, TEXT("MCU Phase 2"), 5,9,6,3, 7000, TEXT("Hyper-velocity. 'Shotgun.' Supersonic. The fastest Legion suit after Tiger."), {TEXT("speed_force_conduit")}, {TEXT("mcu_mk7")});

	// Marks XLI: Prehensile prototype for Mk42
	Add(TEXT("mcu_mk41"), TEXT("Mark XLI"), TEXT("Bones"), 2013, TEXT("MCU Phase 2"), 3,9,4,6, 8000, TEXT("Skeletal frame. 'Bones.' Prehensile prototype. Pieces fly to Tony independently. Test bed for Mark XLII."), {TEXT("nanotube_alloy")}, {TEXT("mcu_mk7")});

	// Mark XLII: The Prodigal Son
	Add(TEXT("mcu_mk42"), TEXT("Mark XLII"), TEXT("The Prodigal Son"), 2013, TEXT("MCU Phase 2"), 6,8,6,4, 25000, TEXT("Prehensile. Individual pieces fly to Tony from anywhere. Falls apart. Still saves the day. 'I'm the best.'"), {TEXT("arc_reactor"),TEXT("nanotube_alloy"),TEXT("ai_chip")}, {TEXT("mcu_mk41")});
	Add(TEXT("mcu_mk43"), TEXT("Mark XLIII"), TEXT("Ultron Pre-Battle"), 2015, TEXT("MCU Phase 2"), 7,7,7,3, 15000, TEXT("Refined Mk42. Prehensile without the bugs. Sokovia opening suit. Paired with Veronica."), {}, {TEXT("mcu_mk42")});

	// Mark XLIV: Hulkbuster (Veronica)
	Add(TEXT("mcu_mk44"), TEXT("Mark XLIV"), TEXT("Hulkbuster — Veronica"), 2015, TEXT("MCU Phase 2"), 10,4,9,1, 80000, TEXT("Orbital deployment from satellite Veronica. Replacement parts mid-fight. Traps Hulk inside a cage. 'Go to sleep, go to sleep, go to sleep.'"), {TEXT("arc_reactor"),TEXT("adamantium"),TEXT("gamma_isotope"),TEXT("vibranium")}, {TEXT("mcu_mk43"),TEXT("mk_hb1")});

	// Marks XLV-XLIX: Sokovia/Ultron era
	Add(TEXT("mcu_mk45"), TEXT("Mark XLV"), TEXT("Heartbreaker — Friday"), 2015, TEXT("MCU Phase 2"), 7,7,8,4, 30000, TEXT("First Friday AI suit. Hexagonal chest RT. Sokovia finale. 'Together.' 'Then we'll do that together too.'"), {TEXT("arc_reactor"),TEXT("ai_chip"),TEXT("repulsor_coil")}, {TEXT("mcu_mk43")});
	Add(TEXT("mcu_mk46"), TEXT("Mark XLVI"), TEXT("Civil War"), 2016, TEXT("MCU Phase 3"), 7,8,8,4, 35000, TEXT("Civil War suit. Fought Cap and Bucky. Deployable helmet. Bleeding edge prototype elements. 'He's my friend.' 'So was I.'"), {TEXT("arc_reactor"),TEXT("ai_chip"),TEXT("nanotube_alloy")}, {TEXT("mcu_mk45")});
	Add(TEXT("mcu_mk47"), TEXT("Mark XLVII"), TEXT("Spider-Man Homecoming"), 2017, TEXT("MCU Phase 3"), 7,7,6,3, 20000, TEXT("Remote controlled via drone. Saved the ferry. Peter's mentor suit. Silver and red highlights."), {}, {TEXT("mcu_mk46")});
	Add(TEXT("mcu_mk48"), TEXT("Mark XLVIII"), TEXT("Hulkbuster Mk II"), 2018, TEXT("MCU Phase 3"), 9,5,9,1, 60000, TEXT("Hulkbuster 2.0. Deployed in Wakanda. Fought the Black Order alongside Banner. 'You guys are so screwed now!'"), {}, {TEXT("mcu_mk44")});
	Add(TEXT("mcu_mk49"), TEXT("Mark XLIX"), TEXT("Rescue"), 2019, TEXT("MCU Phase 3"), 6,8,7,4, 25000, TEXT("Pepper Potts' suit. Rescue armor. Blue and silver. Flight stabilised for non-combat pilot. Pepper uses it to save Tony."), {TEXT("arc_reactor"),TEXT("ai_chip")}, {TEXT("mcu_mk47")});

	// Mark L: Nanotech Genesis (Infinity War)
	Add(TEXT("mcu_mk50"), TEXT("Mark L"), TEXT("Nanotech Genesis"), 2018, TEXT("MCU Phase 3"), 8,9,9,5, 100000, TEXT("Nanotech. Deploys from chest RT. Forms ANY weapon instantly. Shields, cannons, blades, wings. Fought Thanos 1v1 and drew blood. 'You throw another moon at me and I'm gonna lose it.'"), {TEXT("arc_reactor"),TEXT("nanotube_alloy"),TEXT("ai_chip"),TEXT("repulsor_coil"),TEXT("infinity_gauntlet")}, {TEXT("mcu_mk46"),TEXT("mk_bleeding")});

	// Marks LI-LXXXIV: Supporting/Experimental
	Add(TEXT("mcu_mk51"), TEXT("Mark LI"), TEXT("Nanotech Refinement"), 2018, TEXT("MCU Phase 3"), 7,8,8,4, 15000, TEXT("Mark L variant. Experimental repulsor configuration. Never fielded in combat."), {}, {TEXT("mcu_mk50")});
	Add(TEXT("mcu_mk52"), TEXT("Mark LII"), TEXT("Deep Space Nanotech"), 2019, TEXT("MCU Phase 3"), 6,7,8,5, 20000, TEXT("Mark L adapted for deep space. Extended life support. Autonomous drone bay."), {TEXT("tesseract_energy")}, {TEXT("mcu_mk50")});
	Add(TEXT("mcu_mk53"), TEXT("Mark LIII"), TEXT("Arctic Nanotech"), 2019, TEXT("MCU Phase 3"), 7,6,7,3, 12000, TEXT("Mark L adapted for extreme cold. Heating nanites. Ice cannot form on the surface."), {}, {TEXT("mcu_mk50")});
	Add(TEXT("mcu_mk54"), TEXT("Mark LIV"), TEXT("Hydro Nanotech"), 2019, TEXT("MCU Phase 3"), 6,7,6,5, 12000, TEXT("Mark L adapted for deep sea. Crushing pressure resistant. Sonar array. Abyssal rated."), {TEXT("vibranium")}, {TEXT("mcu_mk50")});
	Add(TEXT("mcu_mk55"), TEXT("Mark LV"), TEXT("Stealth Nanotech"), 2019, TEXT("MCU Phase 3"), 5,8,6,10, 15000, TEXT("Mark L adapted for stealth. Full light-bending nanotech camouflage. Completely invisible."), {TEXT("ai_chip")}, {TEXT("mcu_mk50")});
	Add(TEXT("mcu_mk56"), TEXT("Mark LVI"), TEXT("Medical Nanotech"), 2019, TEXT("MCU Phase 3"), 4,6,5,4, 10000, TEXT("Mark L adapted for medical response. Nanite healers. Can stabilise critical patients in field."), {TEXT("healing_factor")}, {TEXT("mcu_mk50")});

	// Marks LVII-LXXX: Development variants
	for (int32 i = 57; i <= 60; ++i)
		Add(*FString::Printf(TEXT("mcu_mk%d"), i), *FString::Printf(TEXT("Mark %s"), *FString::FromInt(i).ToUpper()), *FString::Printf(TEXT("Endgame Development Mk %d"), i), 2019, TEXT("MCU Phase 3"), 6,7,7,3, 8000, TEXT("Endgame-era experimental variant. Pushing nanotech limits. Tony never stopped iterating."), {}, {TEXT("mcu_mk50")});

	for (int32 i = 61; i <= 70; ++i)
		Add(*FString::Printf(TEXT("mcu_mk%d"), i), *FString::Printf(TEXT("Mark %s"), *FString::FromInt(i)), *FString::Printf(TEXT("Post-Snap Prototype Mk %d"), i), 2023, TEXT("MCU Phase 3"), 6,7,7,3, 8000, TEXT("Post-Snap era prototype. Built during the 5-year gap. Tony grieved through engineering. Each suit carries a piece of that."), {}, {TEXT("mcu_mk50")});

	for (int32 i = 71; i <= 80; ++i)
		Add(*FString::Printf(TEXT("mcu_mk%d"), i), *FString::Printf(TEXT("Mark %s"), *FString::FromInt(i)), *FString::Printf(TEXT("Time Heist Prototype Mk %d"), i), 2023, TEXT("MCU Phase 3"), 7,7,7,3, 9000, TEXT("Time Heist era prototype. Quantum-tunnelling resistant. Built for the impossible mission."), {TEXT("pym_particle")}, {TEXT("mcu_mk50")});

	for (int32 i = 81; i <= 84; ++i)
		Add(*FString::Printf(TEXT("mcu_mk%d"), i), *FString::Printf(TEXT("Mark %s"), *FString::FromInt(i)), *FString::Printf(TEXT("Final Battle Prototype Mk %d"), i), 2023, TEXT("MCU Phase 3"), 8,8,8,4, 10000, TEXT("Final battle prototype. Each one a stepping stone to the Mark LXXXV. Tony's last year of engineering."), {}, {TEXT("mcu_mk50")});

	// Mark LXXXV: The Snap
	Add(TEXT("mcu_mk85"), TEXT("Mark LXXXV"), TEXT("The Snap"), 2019, TEXT("MCU Phase 3"), 9,10,10,6, 250000, TEXT("The final MCU armor. Integrated Infinity Gauntlet. Thor's lightning absorption. Fought Thanos army solo. 'And I... am... Iron Man.' Snap. 3,000."), {TEXT("arc_reactor"),TEXT("nanotube_alloy"),TEXT("ai_chip"),TEXT("power_stone"),TEXT("infinity_gauntlet")}, {TEXT("mcu_mk50")});

	// Remove null entries from loop-generated armors (ids for 57-60, 61-70, 71-80, 81-84)
	// These were added with placeholder names
	for (auto& Arm : A) {
		if (Arm.SpecialAbility.IsEmpty()) Arm.SpecialAbility = TEXT("Experimental development variant. Tony's engineering legacy.");
	}
}
