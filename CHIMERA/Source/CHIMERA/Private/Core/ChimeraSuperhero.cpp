#include "Core/ChimeraSuperhero.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

// Helper macro for blueprint entries.
#define BP(id,name,uni,alias,type,desc,level,...) \
	B.Id = id; B.Name = name; B.Universe = uni; B.Alias = alias; B.Type = type; B.Description = desc; \
	B.CraftLevel = level; B.Ingredients = { __VA_ARGS__ }; Blueprints.Add(B);

void USuperheroSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedPowers();
	SeedSuits();
	SeedWeapons();
	SeedBlueprints();
}

// ... (keep all existing SeedPowers, SeedSuits, SeedWeapons functions)

void USuperheroSubsystem::SeedBlueprints()
{
	FHeroBlueprint B;
	// ==================== MARVEL HEROES ====================
	BP("iron_man", "Iron Man", "Marvel", "Tony Stark", "Suit",
		"Craft the Mark LXXXV nanotech armour. Flight, repulsors, AI assistant Friday.", 8,
		"ArcReactor", "NanotubeAlloy", "AI_Chip", "RepulsorCoil");
	BP("captain_america", "Captain America", "Marvel", "Steve Rogers", "Suit",
		"Vibranium-weave tactical suit with vibranium shield. The star-spangled man with a plan.", 5,
		"Vibranium", "PolymerWeave", "StarSpangledBanner", "SuperSoldierSerum");
	BP("thor", "Thor", "Marvel", "Thor Odinson", "Weapon",
		"Mjolnir — only the worthy may lift it. Summons lightning, returns when thrown.", 10,
		"UruMetal", "StormEssence", "AsgardianRune", "LightningCore");
	BP("hulk", "Hulk", "Marvel", "Bruce Banner", "Power",
		"Gamma radiation transformation. The angrier you get, the stronger you get.", 7,
		"GammaIsotope", "AdrenalineSurge", "MolecularStabiliser", "RageCatalyst");
	BP("black_widow", "Black Widow", "Marvel", "Natasha Romanoff", "Suit",
		"Widow's Bite gauntlets and stealth suit. Red Room trained. No super-soldier serum needed.", 4,
		"KevlarWeave", "ElectroShockCoil", "WidowBiteModules", "StealthPolymer");
	BP("hawkeye", "Hawkeye", "Marvel", "Clint Barton", "Weapon",
		"Trick arrow quiver with 18 payload types. Explosive, grapple, EMP, sonic, net, thermite.", 4,
		"CarbonFibreBow", "TrickArrowKit", "ExplosiveTips", "PrecisionSight");
	BP("spider_man", "Spider-Man", "Marvel", "Peter Parker", "Suit",
		"Iron Spider nanotech suit. Wall-crawling, web-shooters, spider-sense, articulated waldoes.", 6,
		"NanotubeAlloy", "WebFluidFormula", "SpiderTracer", "AI_Chip");
	BP("black_panther", "Black Panther", "Marvel", "T'Challa", "Suit",
		"Vibranium-weave kinetic absorption suit. Stored energy released as purple blast wave.", 7,
		"Vibranium", "KineticAbsorber", "HeartShapedHerb", "PantherHabitMesh");
	BP("doctor_strange", "Doctor Strange", "Marvel", "Stephen Strange", "Power",
		"Sling Ring + Eye of Agamotto. Open portals, bend time, mirror dimension access.", 9,
		"SlingRing", "EyeOfAgamotto", "MysticArtsGrimoire", "TimeStoneShard");
	BP("captain_marvel", "Captain Marvel", "Marvel", "Carol Danvers", "Power",
		"Binary form. Absorb and redirect energy. Fly at light-speed. Cosmic-level power.", 9,
		"TesseractEnergy", "KreeBloodSample", "BinaryCatalyst", "PhotonAbsorber");
	BP("scarlet_witch", "Scarlet Witch", "Marvel", "Wanda Maximoff", "Power",
		"Chaos magic and reality warping. Probability manipulation. The Scarlet Witch is a nexus being.", 10,
		"MindStoneFragment", "ChaosMagicTome", "HexCastingFocus", "DarkholdPage");
	BP("vision", "Vision", "Marvel", "Vision", "Suit",
		"Vibranium-synthoid body. Density shifting, solar beam, flight, intangibility.", 8,
		"Vibranium", "MindStoneFragment", "SynthoidCore", "SolarResonator");
	BP("ant_man", "Ant-Man", "Marvel", "Scott Lang", "Suit",
		"Pym particle suit. Shrink to subatomic or grow to 20 metres. Ant communication.", 6,
		"PymParticles", "QuantumRegulator", "SizeModulator", "AntCommLink");
	BP("wasp", "Wasp", "Marvel", "Hope van Dyne", "Suit",
		"Wasp suit with blasters and wings. Shrink, fly, and sting with bio-electric blasts.", 6,
		"PymParticles", "BioElectricStinger", "WingMembrane", "FlightStabiliser");
	BP("winter_soldier", "Winter Soldier", "Marvel", "Bucky Barnes", "Weapon",
		"Vibranium arm with kinetic absorption. Hydra-trained assassin turned hero.", 5,
		"Vibranium", "HydraSerum", "AssassinTrainingMod", "BionicActuator");
	BP("falcon", "Falcon", "Marvel", "Sam Wilson", "Suit",
		"EXO-7 flight harness with articulated wings. Redwing drone companion.", 5,
		"FlightHarness", "CarbonFibreWings", "DroneAI", "VibraniumShield");
	BP("war_machine", "War Machine", "Marvel", "James Rhodes", "Suit",
		"Heavy artillery suit. Missile pods, minigun, shoulder cannon, reinforced armour.", 7,
		"ArcReactor", "TitaniumAlloy", "MissilePodKit", "HeavyOrdnanceCore");
	BP("star_lord", "Star-Lord", "Marvel", "Peter Quill", "Suit",
		"Ravager jet-boots and helmet. Element gun blasters. Walkman with Awesome Mix included.", 4,
		"JetBoots", "ElementGun", "RavagerHelmet", "Walkman");
	BP("gamora", "Gamora", "Marvel", "Gamora", "Weapon",
		"Godslayer sword. Collapsible blade forged from star-metal. Deadliest woman in the galaxy.", 5,
		"StarMetal", "CollapsibleHilt", "AssassinsEdge", "ThanosBlood");
	BP("drax", "Drax the Destroyer", "Marvel", "Drax", "Power",
		"Incredibly durable. Literally invisible when standing still (he swears). Twin knives.", 3,
		"KreeSkinGraft", "TwinDaggers", "DestroyersRage", "TrollHideArmour");
	BP("rocket", "Rocket Raccoon", "Marvel", "Rocket", "Weapon",
		"Custom heavy weapons platform. Explosives expert. Builds planet-cracking bombs from scrap.", 6,
		"PlasmaCore", "ScrapMetal", "ExplosiveCompound", "CyberEnhancement");
	BP("groot", "Groot", "Marvel", "Groot", "Power",
		"Flora colossus regeneration. Grow instantly into any shape. Roots entangle enemies.", 4,
		"FloraColossusSapling", "RegenerationMoss", "SolarNutrient", "EntanglingVines");
	BP("nebula", "Nebula", "Marvel", "Nebula", "Suit",
		"Cybernetic combat chassis. Self-repairing, retractable blade arms, pain suppression.", 5,
		"CyberwareCore", "BladeArmModules", "PainSuppressor", "SelfRepairNanites");
	BP("mantis", "Mantis", "Marvel", "Mantis", "Power",
		"Empathic touch. Sense and manipulate emotions. Put enemies to sleep with a touch.", 3,
		"EmpathicResonator", "AntennaeImplant", "SleepInducer", "EmotionalSpectrumPrism");
	BP("daredevil", "Daredevil", "Marvel", "Matt Murdock", "Suit",
		"Radar-sense billy club and armoured suit. Blind lawyer by day, Hell's Kitchen's devil by night.", 4,
		"BillyClub", "ArmouredSuitMesh", "RadarSenseAmplifier", "SmokePelletKit");
	BP("punisher", "Punisher", "Marvel", "Frank Castle", "Weapon",
		"Custom battle van full arsenal. Skull-vest body armour. No mercy.", 5,
		"ArsenalCrate", "SkullVestArmour", "BattleVanKeys", "MilitaryGradeAmmo");
	BP("ghost_rider", "Ghost Rider", "Marvel", "Johnny Blaze", "Power",
		"Spirit of Vengeance. Hellfire chain, penance stare, flaming motorcycle.", 9,
		"SpiritOfVengeance", "HellfireChain", "InfernalMotorcycle", "PenanceStareEssence");
	BP("blade", "Blade", "Marvel", "Eric Brooks", "Weapon",
		"Daywalker sword forged to kill vampires. UV grenades, silver stakes, serum injection.", 5,
		"SilverAlloyBlade", "UVGrenadeKit", "VampireBlood", "DaywalkerSerum");
	BP("deadpool", "Deadpool", "Marvel", "Wade Wilson", "Suit",
		"Regenerating suit with maximum katanas. Fourth-wall-breaking included free.", 6,
		"RegenerationFactor", "DualKatana", "Chimichanga", "ComedianImplant");
	BP("wolverine", "Wolverine", "Marvel", "Logan", "Power",
		"Adamantium skeleton and claws. Healing factor. Animal-keen senses. The best there is.", 8,
		"Adamantium", "HealingFactor", "BerserkerRage", "AnimalSenseImplant");
	BP("storm", "Storm", "Marvel", "Ororo Munroe", "Power",
		"Weather manipulation. Summon lightning, blizzards, tornadoes, and monsoons.", 7,
		"WeatherControlModule", "AtmosphericResonator", "LightningRodCore", "GoddessBlessing");
	BP("cyclops", "Cyclops", "Marvel", "Scott Summers", "Suit",
		"Ruby quartz visor with optic blast modulation. Punch dimension energy beam.", 5,
		"RubyQuartz", "OpticVisor", "PunchDimensionAperture", "TacticalHeadset");
	BP("jean_grey", "Phoenix", "Marvel", "Jean Grey", "Power",
		"The Phoenix Force. Telepathy, telekinesis, cosmic fire. The most powerful psychic.", 10,
		"PhoenixForceShard", "TelepathyAmplifier", "CosmicFireCore", "DarkPhoenixRestraint");
	BP("professor_x", "Professor X", "Marvel", "Charles Xavier", "Power",
		"Cerebro helmet. Planetary-scale telepathy. Find any mutant on Earth.", 8,
		"CerebroHelmet", "PsychicAmplifier", "Hoverchair", "MutantGeneScanner");
	BP("magneto", "Magneto", "Marvel", "Erik Lehnsherr", "Power",
		"Master of magnetism. Move mountains of metal. Create electromagnetic shields.", 9,
		"MagneticHelmet", "ElectroMagneticCore", "MutantGeneActivator", "MetalControlAmplifier");
	BP("rogue", "Rogue", "Marvel", "Anna Marie", "Power",
		"Absorb powers, memories, and life force with a touch. Flight and super strength (permanent).", 6,
		"PowerAbsorbGloves", "MsMarvelFlightHarness", "ControlModule", "MutantGeneStabiliser");
	BP("gambit", "Gambit", "Marvel", "Remy LeBeau", "Weapon",
		"Kinetic-charged playing cards. Charged bo staff. Cajun charm included.", 4,
		"KineticChargedDeck", "BoStaff", "ThrowingAccuracyMod", "CajunCharm");
	BP("beast", "Beast", "Marvel", "Hank McCoy", "Power",
		"Enhanced strength, agility, and genius intellect. Blue fur, sharp claws, gentle heart.", 5,
		"MutagenicFormula", "StrengthEnhancer", "AgilityBooster", "GeniusSerum");
	BP("iceman", "Iceman", "Marvel", "Bobby Drake", "Power",
		"Omega-level cryokinesis. Create ice slides, freeze entire city blocks, become living ice.", 7,
		"CryoCore", "IceSlideGenerator", "DeepFreezeAmplifier", "LivingIceFormula");
	BP("colossus", "Colossus", "Marvel", "Piotr Rasputin", "Power",
		"Organic steel transformation. Impervious to conventional weapons. Gentle giant.", 6,
		"OrganicSteelFormula", "OsmiumInfusion", "StrengthDoubler", "ArtistSoul");
	BP("nightcrawler", "Nightcrawler", "Marvel", "Kurt Wagner", "Power",
		"Teleportation through brimstone dimension. Prehensile tail. Acrobat. Catholic faith.", 6,
		"BrimstoneTransporter", "TeleportStabiliser", "TailAmplifier", "HolyWaterBlessing");
	BP("silver_surfer", "Silver Surfer", "Marvel", "Norrin Radd", "Power",
		"The Power Cosmic. Surf interstellar space. Matter manipulation. Herald of Galactus.", 10,
		"PowerCosmic", "SilverSurfboard", "CosmicAwareness", "GalactusHeraldSigil");
	BP("loki", "Loki", "Marvel", "Loki Laufeyson", "Power",
		"Illusion magic and trickery. Shapeshift, duplicate, and deceive. God of Mischief.", 7,
		"JotunheimIceDagger", "IllusionTome", "TesseractFragment", "TricksterEssence");
	BP("thanos", "Thanos", "Marvel", "Thanos", "Weapon",
		"Infinity Gauntlet (requires 6 stones). Snap changes reality. Balanced, as all things should be.", 10,
		"InfinityGauntlet", "PowerStone", "SpaceStone", "RealityStone", "SoulStone", "TimeStone", "MindStone");
	BP("ultron", "Ultron", "Marvel", "Ultron", "Suit",
		"Vibranium body with AI hive-mind. No strings on him. Self-replicating drone army.", 9,
		"VibraniumChassis", "UltronAI_Chip", "DroneFactoryModule", "AntiGravityCore");
	BP("hela", "Hela", "Marvel", "Hela", "Weapon",
		"Necroswords. Summon blades from thin air. Death incarnate. Goddess of the underworld.", 9,
		"NecroswordBlueprint", "DeathEnergy", "AsgardianSoulFragment", "FenrisWolfToken");
	BP("venom", "Venom", "Marvel", "Eddie Brock", "Suit",
		"Symbiote suit. Shapeshifting, regeneration, enhanced everything. We are Venom.", 7,
		"SymbioteSample", "AdrenalineBooster", "Chocolate", "HostDNA");
	BP("miles_morales", "Spider-Man (Miles)", "Marvel", "Miles Morales", "Suit",
		"Venom strike and invisibility camouflage. Graffiti-style web design. What's up, danger?", 6,
		"WebFluidFormula", "VenomStrikeModule", "InvisibilityCloak", "SprayCanKit");
	BP("shang_chi", "Shang-Chi", "Marvel", "Shaun", "Weapon",
		"The Ten Rings. Each ring grants a unique power. Dragon-scale armour. Master of Kung Fu.", 8,
		"TenRings", "DragonScale", "TaiLoWater", "GreatProtectorBlessing");
	BP("ms_marvel", "Ms. Marvel", "Marvel", "Kamala Khan", "Power",
		"Embigen! Stretch, shrink, and enlarge any body part. Hard light constructs.", 5,
		"NoorDimensionBangle", "HardLightEmitter", "StretchPolymer", "FanficJournal");
	BP("america_chavez", "America Chavez", "Marvel", "America Chavez", "Power",
		"Star-shaped portal punches. Punch through dimensions. Fly. Super strength.", 7,
		"StarPortalPunch", "DimensionHopper", "SuperStrengthSerum", "UtopianParallelToken");
	BP("she_hulk", "She-Hulk", "Marvel", "Jennifer Walters", "Power",
		"Gamma transformation with full intelligence retained. Law degree not included.", 6,
		"GammaBlood", "ControlModule", "LegalBrief", "ExerciseRegimen");

	// ==================== DC HEROES ====================
	BP("superman", "Superman", "DC", "Clark Kent", "Suit",
		"Kryptonian solar suit. Flight, heat vision, freeze breath, invulnerability under yellow sun.", 10,
		"KryptonianCrystal", "SolarAbsorptionMesh", "CapeOfEl", "YellowSunBattery");
	BP("batman", "Batman", "DC", "Bruce Wayne", "Suit",
		"Batsuit Mark XI. Graphene cowl, utility belt with 50 gadgets, cape glider, voice modulator.", 7,
		"GrapheneWeave", "UtilityBelt", "BatarangKit", "BatmobileRemote");
	BP("wonder_woman", "Wonder Woman", "DC", "Diana Prince", "Suit",
		"Amazonian armour. Bracelets of Submission. Lasso of Truth. Tiara. Sword of Athena.", 9,
		"AmazonianSteel", "BraceletsSubmission", "LassoOfTruth", "SwordOfAthena");
	BP("flash", "The Flash", "DC", "Barry Allen", "Suit",
		"Speed Force suit. Move at light speed, phase through matter, time travel via Cosmic Treadmill.", 8,
		"SpeedForceConduit", "FrictionlessFabric", "LightningRod", "CosmicTreadmillChip");
	BP("aquaman", "Aquaman", "DC", "Arthur Curry", "Weapon",
		"Trident of Atlan. Command sea life. Hydrokinetic blasts. King of the Seven Seas.", 7,
		"AtlanteanSteel", "TridentOfAtlan", "SeaLifeWhistle", "HydrokineticCore");
	BP("green_lantern", "Green Lantern", "DC", "Hal Jordan", "Weapon",
		"Green Lantern power ring and battery. Hard light constructs limited only by willpower.", 8,
		"PowerRing", "LanternBattery", "OathOfWill", "OanEnergyCell");
	BP("martian_manhunter", "Martian Manhunter", "DC", "J'onn J'onzz", "Power",
		"Shapeshifting, telepathy, intangibility, flight, super strength. Weakness: fire.", 8,
		"MartianDNA", "ShapeshiftAmplifier", "TelepathyModule", "FireResistanceChip");
	BP("cyborg", "Cyborg", "DC", "Victor Stone", "Suit",
		"Mother Box-powered cybernetic body. Sonic cannon. Boom Tube access. Hack anything.", 7,
		"MotherBox", "CyberChassis", "SonicCannonArm", "BoomTubeGenerator");
	BP("shazam", "Shazam", "DC", "Billy Batson", "Power",
		"Wisdom of Solomon. Strength of Hercules. Stamina of Atlas. Power of Zeus. Courage of Achilles. Speed of Mercury.", 8,
		"WizardStaff", "LightningBoltCatalyst", "RockOfEternityShard", "ChampionEssence");
	BP("green_arrow", "Green Arrow", "DC", "Oliver Queen", "Weapon",
		"Compound bow with 50+ trick arrows. Boxing glove arrow included.", 4,
		"CompoundBow", "TrickArrowSet", "BoxingGloveArrow", "PrecisionScope");
	BP("nightwing", "Nightwing", "DC", "Dick Grayson", "Suit",
		"Escrima sticks with taser function. Acrobatic suit. The original Robin, grown.", 5,
		"EscrimaSticks", "TaserMod", "AcrobaticMesh", "BatTrainingManual");
	BP("supergirl", "Supergirl", "DC", "Kara Zor-El", "Suit",
		"Kryptonian suit. All the powers of Superman. Arrived on Earth last.", 8,
		"KryptonianCrystal", "SolarAbsorptionMesh", "HouseOfElSigil", "ArgoCityToken");
	BP("batgirl", "Batgirl", "DC", "Barbara Gordon", "Suit",
		"Oracle's tactical suit. Hacking interface, data uplink, batarangs, wheelchair-accessible.", 5,
		"HackInterface", "DataUplink", "BatarangKit", "OracleDatabase");
	BP("robin", "Robin", "DC", "Damian Wayne", "Suit",
		"Katana and tactical armour. League of Assassins training meets Bat-discipline.", 4,
		"Katana", "TacticalArmour", "LeagueTrainingScroll", "GrapplingGun");
	BP("red_hood", "Red Hood", "DC", "Jason Todd", "Suit",
		"Dual pistols and armoured helmet. The Robin who died. Came back angry.", 5,
		"ArmouredHelmet", "DualPistolKit", "LazarusPitWater", "LeatherJacket");
	BP("batwoman", "Batwoman", "DC", "Kate Kane", "Suit",
		"Military-grade Bat-suit. Red wig and red bat symbol. Trained at West Point.", 5,
		"MilitaryTactical", "RedBatEmblem", "ElectrifiedBatarang", "NightVisionMask");
	BP("john_stewart", "Green Lantern (Stewart)", "DC", "John Stewart", "Weapon",
		"Power ring wielded by a Marine. Constructs favour architecture, shields, and heavy artillery.", 7,
		"PowerRing", "LanternBattery", "MarineTraining", "OanEnergyCell");
	BP("hawkgirl", "Hawkgirl", "DC", "Shayera Hol", "Weapon",
		"Nth metal mace. Anti-magic. Flight via wings. Thanagarian warrior.", 6,
		"NthMetalMace", "ThanagarianWings", "AntiMagicCore", "WarriorHelmet");
	BP("zatanna", "Zatanna", "DC", "Zatanna Zatara", "Power",
		"Backwards magic. Speak spells in reverse. Stage magician turned Justice League Dark.", 7,
		"TopHat", "Wand", "SpellbookInReverse", "StagePresenceModule");
	BP("constantine", "John Constantine", "DC", "John Constantine", "Power",
		"Demonology and hellfire magic. Synchronicity wave. Cigarette lighter hex. Bargains with demons.", 7,
		"HellfireLighter", "DemonologyGrimoire", "SynchronicityAmplifier", "Trenchcoat");
	BP("swamp_thing", "Swamp Thing", "DC", "Alec Holland", "Power",
		"The Green incarnate. Control plant life across the planet. Regenerate from any plant.", 8,
		"TheGreenConnection", "PlantControlAmplifier", "SwampMatter", "PhotosynthesisCore");
	BP("doctor_fate", "Doctor Fate", "DC", "Kent Nelson", "Power",
		"Helmet of Nabu. Lord of Order. Ankh blasts, flight, magic shields, clairvoyance.", 9,
		"HelmetOfNabu", "AnkhAmulet", "CloakOfDestiny", "OrderMagicTome");
	BP("spectre", "Spectre", "DC", "Jim Corrigan", "Power",
		"The Wrath of God. Cosmic-scale punishment. Intangibility, size manipulation, reality warping.", 10,
		"WrathOfGod", "CosmicJudgement", "IntangibleForm", "DivineIntervention");
	BP("black_canary", "Black Canary", "DC", "Dinah Lance", "Power",
		"Canary Cry. Sonic scream shatters concrete. Expert martial artist.", 5,
		"SonicVocalCord", "CanaryCryAmplifier", "MartialArtsManual", "FishnetSuit");
	BP("deadshot", "Deadshot", "DC", "Floyd Lawton", "Weapon",
		"Wrist-mounted guns with aimbot precision. Never misses unless he wants to.", 5,
		"WristGunMount", "AimbotModule", "PrecisionSight", "SuicideSquadToken");
	BP("harley_quinn", "Harley Quinn", "DC", "Harleen Quinzel", "Weapon",
		"Oversized mallet and pop-gun. Gymnastics training. Immune to Joker toxin.", 4,
		"OversizedMallet", "PopGun", "GymnasticsTraining", "JokerToxinAntidote");
	BP("catwoman", "Catwoman", "DC", "Selina Kyle", "Suit",
		"Whip and clawed gloves. Burglary expert. Nine lives. Complicated relationship with Batman.", 4,
		"Whip", "ClawedGloves", "Catsuit", "DiamondHeistPlan");
	BP("lex_luthor", "Lex Luthor", "DC", "Lex Luthor", "Suit",
		"Warsuit with kryptonite core. Genius-level intellect. 12 PhDs. Xenophobia included.", 8,
		"WarsuitChassis", "KryptoniteCore", "GeniusIntellect", "BusinessEmpire");
	BP("joker", "Joker", "DC", "Unknown", "Weapon",
		"Joker toxin flowers and joy buzzer. Unpredictable, chaotic, no known cure for his laughter.", 7,
		"JokerToxin", "JoyBuzzer", "LaughingGas", "PurpleSuit");
	BP("darkseid", "Darkseid", "DC", "Uxas", "Power",
		"Omega Beams. The Anti-Life Equation. Ruler of Apokolips. Darkseid is.", 10,
		"OmegaBeam", "AntiLifeEquation", "ApokolipsThrone", "MotherBox");
	BP("ras_al_ghul", "Ra's al Ghul", "DC", "Ra's al Ghul", "Power",
		"Lazarus Pit immortality. League of Assassins mastery. Sword of the Demon's Head.", 8,
		"LazarusPitWater", "SwordOfDemonHead", "LeagueTrainingScroll", "ImmortalityFormula");
	BP("deathstroke", "Deathstroke", "DC", "Slade Wilson", "Suit",
		"Super-soldier serum and Ikon suit. Enhanced everything. Mercenary. Tactical genius.", 6,
		"SuperSoldierSerum", "IkonSuit", "PromethiumSword", "TacticalVisor");
	BP("bane", "Bane", "DC", "Unknown prison", "Power",
		"Venom steroid pump. Breaks backs. Born in darkness. The man who broke the Bat.", 6,
		"VenomSteroid", "MaskedRespirator", "LuchadorTraining", "TriggerPhrase");

	// ==================== MORE MARVEL ====================
	BP("moon_knight", "Moon Knight", "Marvel", "Marc Spector", "Suit",
		"Khonshu's ceremonial armour. Crescent darts. Multiple personalities. 'Random bullshit, go!'", 6,
		"CrescentDarts", "CeremonialArmour", "KhonshuBlessing", "PersonalityStabiliser");
	BP("echo", "Echo", "Marvel", "Maya Lopez", "Power",
		"Photographic reflexes. Copy any physical action perfectly. Deaf since birth. Enhanced other senses.", 5,
		"PhotographicReflexes", "SensoryAmplifier", "HandprintFace", "SignLanguageManual");
	BP("ironheart", "Ironheart", "Marvel", "Riri Williams", "Suit",
		"Self-built Iron Man armour at 15. Reverse-engineered from scrap. MIT genius.", 7,
		"ArcReactor", "ReverseEngineeredChassis", "MITBlueprint", "AI_Natalie");
	BP("wong", "Wong", "Marvel", "Wong", "Power",
		"Sorcerer Supreme accessories. Sling ring, Wand of Watoomb, tuna melt conjuration.", 6,
		"SlingRing", "WandOfWatoomb", "SanctumSanctorumKeys", "TunaMelt");
	BP("yelenas", "Yelena Belova", "Marvel", "Yelena Belova", "Suit",
		"Black Widow tactical suit with vest pockets (so many pockets). Poser stance included.", 4,
		"TacticalVest", "WidowBiteModules", "ElectroShockCoil", "PocketKit");
	BP("kate_bishop", "Hawkeye (Kate)", "Marvel", "Kate Bishop", "Weapon",
		"Trick arrows with purple flair. Olympic-level archery. No powers, all skill (and sass).", 4,
		"TrickArrowKit", "CompoundBow", "PurpleFlairMesh", "FencingTraining");
	BP("quicksilver", "Quicksilver", "Marvel", "Pietro Maximoff", "Power",
		"Supersonic speed. Did not see that coming. Silver hair in motion blur.", 7,
		"SpeedForceSerum", "SilverHairDye", "MotionBlurStabiliser", "SiblingBond");
	BP("valkyrie", "Valkyrie", "Marvel", "Brunnhilde", "Weapon",
		"Dragonfang sword. Asgardian battle axe. Pegasus mount. King of New Asgard.", 7,
		"Dragonfang", "AsgardianAxe", "PegasusWhistle", "KingsCrown");

	// ==================== MORE DC ====================
	BP("blue_beetle", "Blue Beetle", "DC", "Jaime Reyes", "Suit",
		"Scarab armour. Alien tech. Energy cannons, blades, wings, and full-body shield.", 7,
		"ScarabCarapace", "AlienTechCore", "EnergyCannon", "WingModule");
	BP("booster_gold", "Booster Gold", "DC", "Michael Carter", "Suit",
		"Future tech suit. Force field, wrist blasters, flight ring. Skeets drone companion.", 5,
		"ForceFieldBelt", "WristBlasters", "FlightRing", "SkeetsAI");
	BP("plastick_man", "Plastic Man", "DC", "Eel O'Brian", "Power",
		"Ultimate elasticity. Stretch miles, reshape into anything. Immune to telepathy.", 6,
		"ElasticityFormula", "StretchStabiliser", "ShapeMemoryCore", "ComedianImplant");
	BP("firestorm", "Firestorm", "DC", "Ronnie Raymond+Martin Stein", "Power",
		"Nuclear transmutation. Rearrange atomic structures. Flight and energy blasts.", 8,
		"FirestormMatrix", "NuclearCore", "TransmutationAmplifier", "DualConsciousnessLink");
	BP("atom", "Atom", "DC", "Ray Palmer", "Suit",
		"Size-control belt. Shrink to subatomic scale. White dwarf star matter.", 7,
		"SizeControlBelt", "WhiteDwarfMatter", "QuantumRegulator", "MicroscaleVisor");
	BP("hawkman", "Hawkman", "DC", "Carter Hall", "Weapon",
		"Nth metal mace and wings. Reincarnated across millennia. Thanagarian metal mastery.", 7,
		"NthMetalMace", "HawkWings", "ReincarnationSeal", "ThanagarianArmour");
	BP("starfire", "Starfire", "DC", "Koriand'r", "Power",
		"Starbolts. Solar-powered flight. Tamaranean physiology — learns via lip contact.", 7,
		"StarboltEmitter", "TamaraneanDNA", "SolarFlightCore", "EmotionalSpectrum");
	BP("raven", "Raven", "DC", "Rachel Roth", "Power",
		"Daughter of Trigon. Soul-self projection, empathy, telekinesis, dark magic. Azarath Metrion Zinthos.", 9,
		"SoulSelfGem", "DarkMagicTome", "EmpathyAmplifier", "TrigonRestraintSigil");
	BP("beast_boy", "Beast Boy", "DC", "Garfield Logan", "Power",
		"Animal shapeshifting. Transform into any animal — extinct, alien, or mythological.", 6,
		"AnimalDNALibrary", "ShapeshiftStabiliser", "GreenSkinFormula", "DoomPatrolToken");
	BP("brainiac", "Brainiac", "DC", "Vril Dox", "Suit",
		"Collector of worlds. Alien AI with shrink-ray and tentacle arms. Bottles cities.", 9,
		"BrainiacAI_Chip", "ShrinkRay", "TentacleHarness", "CityBottle");
	BP("doomsday", "Doomsday", "DC", "The Ultimate", "Power",
		"Adaptive regeneration. Cannot die the same way twice. Killed Superman.", 10,
		"AdaptiveRegeneration", "KryptonianDNA", "DoomsdayFlesh", "DeathImmunity");
	BP("lobo", "Lobo", "DC", "The Main Man", "Power",
		"Czarnian regeneration. Super strength. Space hog motorcycle. Chain hook.", 8,
		"CzarnianDNA", "ChainHook", "SpaceHogBike", "BastichAttitude");
	BP("reverse_flash", "Reverse-Flash", "DC", "Eobard Thawne", "Suit",
		"Negative Speed Force. Time remnants, vibrating murder hands. 'It was me, Barry.'", 9,
		"NegativeSpeedForce", "VibratingKillHand", "TimeRemnantAnchor", "YellowSuit");
};

#undef BP

void USuperheroSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedPowers();
	SeedSuits();
	SeedWeapons();
}

void USuperheroSubsystem::SeedPowers()
{
	FSuperPowerDef P;
	P.Id = TEXT("flight"); P.Name = TEXT("Flight"); P.Type = EPowerArchetype::Flight;
	P.ActiveDesc = TEXT("Soar at Mach 2. Bypass terrain. Dive-bomb enemies from above.");
	P.PassiveDesc = TEXT("+10% movement speed per level. Ignore fall damage."); P.UnlockLevel = 1; P.CooldownSecs = 0; PowerCatalog.Add(P);

	P.Id = TEXT("super_strength"); P.Name = TEXT("Super Strength"); P.Type = EPowerArchetype::SuperStrength;
	P.ActiveDesc = TEXT("Lift 50 tonnes. Shatter walls. Melee hits deal 3x damage for 10s.");
	P.PassiveDesc = TEXT("+5 melee damage per level. Carry weight unlimited."); P.UnlockLevel = 1; P.CooldownSecs = 15; PowerCatalog.Add(P);

	P.Id = TEXT("super_speed"); P.Name = TEXT("Super Speed"); P.Type = EPowerArchetype::SuperSpeed;
	P.ActiveDesc = TEXT("Move at blurred speed. Auto-dodge all attacks for 5s. Strike 6 times in one action.");
	P.PassiveDesc = TEXT("+3% attack speed per level. Dodge chance increased."); P.UnlockLevel = 3; P.CooldownSecs = 20; PowerCatalog.Add(P);

	P.Id = TEXT("telekinesis"); P.Name = TEXT("Telekinesis"); P.Type = EPowerArchetype::Telekinesis;
	P.ActiveDesc = TEXT("Lift and throw objects or enemies. Crush, hurl, or restrain from a distance.");
	P.PassiveDesc = TEXT("+5% ranged damage per level. Can open locks without touching."); P.UnlockLevel = 1; P.CooldownSecs = 8; PowerCatalog.Add(P);

	P.Id = TEXT("telepathy"); P.Name = TEXT("Telepathy"); P.Type = EPowerArchetype::Telepathy;
	P.ActiveDesc = TEXT("Read an NPC's surface thoughts. Predict enemy attacks for 10s (+50% dodge).");
	P.PassiveDesc = TEXT("+3 to all dialogue skill checks per level. Detect lies automatically."); P.UnlockLevel = 5; P.CooldownSecs = 12; PowerCatalog.Add(P);

	P.Id = TEXT("energy_blast"); P.Name = TEXT("Energy Blast"); P.Type = EPowerArchetype::EnergyBlast;
	P.ActiveDesc = TEXT("Unleash a concentrated beam. 80-200 damage. Pierce through cover.");
	P.PassiveDesc = TEXT("+5% energy damage per level. Charged melee hits stun."); P.UnlockLevel = 2; P.CooldownSecs = 10; PowerCatalog.Add(P);

	P.Id = TEXT("force_field"); P.Name = TEXT("Force Field"); P.Type = EPowerArchetype::ForceField;
	P.ActiveDesc = TEXT("Project a shield bubble. Absorb 200 damage. Allies inside gain 50% armour.");
	P.PassiveDesc = TEXT("+10 shield HP per level. Regenerates 5% shield/sec out of combat."); P.UnlockLevel = 3; P.CooldownSecs = 25; PowerCatalog.Add(P);

	P.Id = TEXT("shapeshift"); P.Name = TEXT("Shapeshifting"); P.Type = EPowerArchetype::Shapeshift;
	P.ActiveDesc = TEXT("Morph into any NPC form. Infiltrate hostile areas unrecognised. Duration 30s.");
	P.PassiveDesc = TEXT("+5% disguise effectiveness per level. Guards less suspicious."); P.UnlockLevel = 7; P.CooldownSecs = 30; PowerCatalog.Add(P);

	P.Id = TEXT("invisibility"); P.Name = TEXT("Invisibility"); P.Type = EPowerArchetype::Invisibility;
	P.ActiveDesc = TEXT("Become unseen. Stealth attacks auto-crit. Duration 15s. Attacking breaks cloak.");
	P.PassiveDesc = TEXT("+3% stealth per level. Harder to detect when stationary."); P.UnlockLevel = 4; P.CooldownSecs = 18; PowerCatalog.Add(P);

	P.Id = TEXT("regeneration"); P.Name = TEXT("Regeneration"); P.Type = EPowerArchetype::Regeneration;
	P.ActiveDesc = TEXT("Heal 60% HP instantly. Cures poison, bleeding, and broken bones.");
	P.PassiveDesc = TEXT("+1 HP/sec per level. Cannot be permanently crippled."); P.UnlockLevel = 2; P.CooldownSecs = 30; PowerCatalog.Add(P);

	P.Id = TEXT("pyrokinesis"); P.Name = TEXT("Pyrokinesis"); P.Type = EPowerArchetype::Pyrokinesis;
	P.ActiveDesc = TEXT("Erupt in flames. 50-150 AoE fire damage. Sets environment on fire.");
	P.PassiveDesc = TEXT("+5% fire damage per level. Immune to burn effects."); P.UnlockLevel = 3; P.CooldownSecs = 12; PowerCatalog.Add(P);

	P.Id = TEXT("cryokinesis"); P.Name = TEXT("Cryokinesis"); P.Type = EPowerArchetype::Cryokinesis;
	P.ActiveDesc = TEXT("Freeze enemies solid for 4s. 40-100 ice damage. Creates ice walls for cover.");
	P.PassiveDesc = TEXT("+3% ice damage per level. Immune to freezing."); P.UnlockLevel = 3; P.CooldownSecs = 14; PowerCatalog.Add(P);

	P.Id = TEXT("electrokinesis"); P.Name = TEXT("Electrokinesis"); P.Type = EPowerArchetype::Electrokinesis;
	P.ActiveDesc = TEXT("Summon lightning. 60-180 damage with chain effect. Stun machines and cyborgs.");
	P.PassiveDesc = TEXT("+5% lightning damage per level. Overcharge tech devices."); P.UnlockLevel = 4; P.CooldownSecs = 10; PowerCatalog.Add(P);

	P.Id = TEXT("technopathy"); P.Name = TEXT("Technopathy"); P.Type = EPowerArchetype::Technopathy;
	P.ActiveDesc = TEXT("Control any machine: hack drones, disable turrets, hijack vehicles.");
	P.PassiveDesc = TEXT("+5 to all hacking checks per level. Machines talk to you."); P.UnlockLevel = 5; P.CooldownSecs = 15; PowerCatalog.Add(P);

	P.Id = TEXT("portals"); P.Name = TEXT("Portal Creation"); P.Type = EPowerArchetype::Portals;
	P.ActiveDesc = TEXT("Open a portal to any visited location. Instant travel. Can redirect projectiles.");
	P.PassiveDesc = TEXT("+1 ring fast-travel per 10 levels. Can portal small objects free."); P.UnlockLevel = 8; P.CooldownSecs = 45; PowerCatalog.Add(P);

	P.Id = TEXT("size_shift"); P.Name = TEXT("Size Manipulation"); P.Type = EPowerArchetype::SizeShift;
	P.ActiveDesc = TEXT("Shrink to insect size (stealth) or grow to building size (3x damage, 3x armour).");
	P.PassiveDesc = TEXT("+1% size change range per level. Can fit through small gaps."); P.UnlockLevel = 6; P.CooldownSecs = 20; PowerCatalog.Add(P);

	P.Id = TEXT("gravity_control"); P.Name = TEXT("Gravity Control"); P.Type = EPowerArchetype::GravityControl;
	P.ActiveDesc = TEXT("Crush enemies with 10x gravity or make them weightless. AoE 15m radius.");
	P.PassiveDesc = TEXT("+3% jump height per level. Can walk on walls/ceilings at level 50+."); P.UnlockLevel = 7; P.CooldownSecs = 22; PowerCatalog.Add(P);

	P.Id = TEXT("time_dilation"); P.Name = TEXT("Time Dilation"); P.Type = EPowerArchetype::TimeDilation;
	P.ActiveDesc = TEXT("Slow time to 10% for 4s. You move at full speed. Everything else crawls.");
	P.PassiveDesc = TEXT("+2% reaction speed per level. See bullet trails at level 75+."); P.UnlockLevel = 10; P.CooldownSecs = 60; PowerCatalog.Add(P);
}

void USuperheroSubsystem::SeedSuits()
{
	FSuperSuitDef S;
	S.Id = TEXT("rookie_spandex"); S.Name = TEXT("Rookie Spandex"); S.Material = TEXT("Polymer weave"); S.VisualStyle = TEXT("Classic bright colours");
	S.Armor = 1; S.Mobility = 7; S.PowerAmp = 1; S.Stealth = 1; S.SpecialTrait = TEXT("Your first suit. Sentimental. Cape included."); SuitCatalog.Add(S);

	S.Id = TEXT("tactical_kevlar"); S.Name = TEXT("Tactical Kevlar"); S.Material = TEXT("Kevlar-nanotube composite"); S.VisualStyle = TEXT("Military black/olive");
	S.Armor = 5; S.Mobility = 4; S.PowerAmp = 2; S.Stealth = 3; S.SpecialTrait = TEXT("Ballistic protection. NPD-approved for street-level hero work."); SuitCatalog.Add(S);

	S.Id = TEXT("arc_reactor_mesh"); S.Name = TEXT("Arc Reactor Mesh"); S.Material = TEXT("Palladium arc core"); S.VisualStyle = TEXT("Glowing chest piece, red/gold");
	S.Armor = 4; S.Mobility = 5; S.PowerAmp = 7; S.Stealth = 1; S.bHasFlightStabilizers = true; S.SpecialTrait = TEXT("Repulsor flight. Energy weapons deal +30% damage."); SuitCatalog.Add(S);

	S.Id = TEXT("vibranium_weave"); S.Name = TEXT("Vibranium Weave"); S.Material = TEXT("Vibranium alloy"); S.VisualStyle = TEXT("Sleek black with silver accents");
	S.Armor = 8; S.Mobility = 6; S.PowerAmp = 3; S.Stealth = 2; S.SpecialTrait = TEXT("Absorbs kinetic energy. Damage taken charges your next attack."); SuitCatalog.Add(S);

	S.Id = TEXT("stealth_suit"); S.Name = TEXT("Phantom Stealth Suit"); S.Material = TEXT("Active camouflage polymer"); S.VisualStyle = TEXT("Shifting pattern, nearly invisible");
	S.Armor = 3; S.Mobility = 8; S.PowerAmp = 2; S.Stealth = 10; S.SpecialTrait = TEXT("Near-perfect invisibility at night. Silent footsteps."); SuitCatalog.Add(S);

	S.Id = TEXT("dragon_scale"); S.Name = TEXT("Dragon Scale Armour"); S.Material = TEXT("Saphira's shed scales"); S.VisualStyle = TEXT("Iridescent blue, scale pattern");
	S.Armor = 7; S.Mobility = 5; S.PowerAmp = 6; S.Stealth = 1; S.bHasFlightStabilizers = true; S.SpecialTrait = TEXT("Fireproof. Ancient Language runes woven in. Dragon bond +10."); SuitCatalog.Add(S);

	S.Id = TEXT("quantum_suit"); S.Name = TEXT("Quantum Suit"); S.Material = TEXT("Pym particle-infused nanotech"); S.VisualStyle = TEXT("Honeycomb pattern, shifts colours");
	S.Armor = 6; S.Mobility = 7; S.PowerAmp = 8; S.Stealth = 4; S.SpecialTrait = TEXT("Size shifts with you. Works in quantum realm and micro-scale."); SuitCatalog.Add(S);

	S.Id = TEXT("symbiote_suit"); S.Name = TEXT("Symbiote Bond"); S.Material = TEXT("Living alien organism"); S.VisualStyle = TEXT("Organic black with shifting white sigil");
	S.Armor = 5; S.Mobility = 9; S.PowerAmp = 6; S.Stealth = 6; S.SpecialTrait = TEXT("Self-repairing. Amplifies aggression. Whispers tactics to you. Weak to sonic/fire."); SuitCatalog.Add(S);

	S.Id = TEXT("iron_spider"); S.Name = TEXT("Iron Spider"); S.Material = TEXT("Nanotech iron alloy"); S.VisualStyle = TEXT("Red and gold with articulated spider legs");
	S.Armor = 7; S.Mobility = 8; S.PowerAmp = 5; S.Stealth = 3; S.SpecialTrait = TEXT("Articulated waldoes (4 extra limbs). Wall-crawling. Parachute."); SuitCatalog.Add(S);

	S.Id = TEXT("mjolnir_armour"); S.Name = TEXT("Asgardian Battle Plate"); S.Material = TEXT("Uru metal and Asgardian leather"); S.VisualStyle = TEXT("Norse designs, winged helm, red cape");
	S.Armor = 9; S.Mobility = 4; S.PowerAmp = 7; S.Stealth = 1; S.SpecialTrait = TEXT("Worthy. Lightning strikes enhance your attacks. Bifrost-compatible."); SuitCatalog.Add(S);

	S.Id = TEXT("speed_force"); S.Name = TEXT("Speed Force Suit"); S.Material = TEXT("Frictionless Speed Force conduit"); S.VisualStyle = TEXT("Red with gold lightning bolt");
	S.Armor = 2; S.Mobility = 10; S.PowerAmp = 4; S.Stealth = 4; S.SpecialTrait = TEXT("Phase through solid objects at full sprint. Time seems slower."); SuitCatalog.Add(S);

	S.Id = TEXT("witch_cloak"); S.Name = TEXT("Mystic Cloak of Levitation"); S.Material = TEXT("Sentient enchanted fabric"); S.VisualStyle = TEXT("Deep crimson with gold trim, floating");
	S.Armor = 2; S.Mobility = 8; S.PowerAmp = 9; S.Stealth = 3; S.bHasFlightStabilizers = true; S.SpecialTrait = TEXT("The cloak has a mind of its own. It protects you autonomously."); SuitCatalog.Add(S);

	S.Id = TEXT("hulk_buster"); S.Name = TEXT("Hulk-Buster Siege Armour"); S.Material = TEXT("Titanium-vibranium reinforced exoskeleton"); S.VisualStyle = TEXT("Massive red and gold war machine");
	S.Armor = 10; S.Mobility = 2; S.PowerAmp = 10; S.Stealth = 0; S.SpecialTrait = TEXT("Kaiju-class strength. Cannot be knocked down. Consumes power cells rapidly."); SuitCatalog.Add(S);

	S.Id = TEXT("neon_cyber"); S.Name = TEXT("Neon Cyber-Suit"); S.Material = TEXT("Chromed cyberware with RGB circuits"); S.VisualStyle = TEXT("Black with pulsing neon trim");
	S.Armor = 4; S.Mobility = 7; S.PowerAmp = 5; S.Stealth = 5; S.SpecialTrait = TEXT("Quickhack boost. Every hack slows enemy reaction time by 30%."); SuitCatalog.Add(S);
}

void USuperheroSubsystem::SeedWeapons()
{
	FSuperWeaponDef W;
	W.Id = TEXT("energy_blade"); W.Name = TEXT("Energy Blade"); W.DamageType = TEXT("plasma"); W.BaseDamage = 50; W.Range = 0;
	W.SpecialEffect = TEXT("Ignores 30% armour. Extends on activation from wrist mount."); WeaponCatalog.Add(W);

	W.Id = TEXT("repulsor_gauntlets"); W.Name = TEXT("Repulsor Gauntlets"); W.DamageType = TEXT("energy"); W.BaseDamage = 40; W.Range = 1;
	W.SpecialEffect = TEXT("Knockback on hit. Can be charged for AoE blast."); WeaponCatalog.Add(W);

	W.Id = TEXT("mjolnir"); W.Name = TEXT("Stormbreaker"); W.DamageType = TEXT("lightning"); W.BaseDamage = 80; W.Range = 1;
	W.SpecialEffect = TEXT("Returns when thrown. Summons lightning AoE. Bifrost portal to any ring."); WeaponCatalog.Add(W);

	W.Id = TEXT("vibranium_shield"); W.Name = TEXT("Vibranium Shield"); W.DamageType = TEXT("kinetic"); W.BaseDamage = 30; W.Range = 1;
	W.SpecialEffect = TEXT("Ricochets between enemies (up to 4). Absorbs energy attacks. Always returns."); WeaponCatalog.Add(W);

	W.Id = TEXT("grappling_hook"); W.Name = TEXT("Grappling Hook"); W.DamageType = TEXT("kinetic"); W.BaseDamage = 10; W.Range = 2;
	W.SpecialEffect = TEXT("Zip to any surface. Pull enemies toward you. Traverse the skyline."); WeaponCatalog.Add(W);

	W.Id = TEXT("web_shooters"); W.Name = TEXT("Web Shooters"); W.DamageType = TEXT("immobilise"); W.BaseDamage = 5; W.Range = 2;
	W.SpecialEffect = TEXT("Immobilise enemies for 6s. Create zip lines. Swing between buildings."); WeaponCatalog.Add(W);

	W.Id = TEXT("sonic_disruptor"); W.Name = TEXT("Sonic Disruptor"); W.DamageType = TEXT("sonic"); W.BaseDamage = 35; W.Range = 1;
	W.SpecialEffect = TEXT("Stun in cone AoE. Double damage to symbiotes and constructs."); WeaponCatalog.Add(W);

	W.Id = TEXT("plasma_rifle"); W.Name = TEXT("Plasma Rifle"); W.DamageType = TEXT("plasma"); W.BaseDamage = 55; W.Range = 2;
	W.SpecialEffect = TEXT("Sustained beam. Overheats after 4s continuous fire. Melts armour."); WeaponCatalog.Add(W);

	W.Id = TEXT("cryo_cannon"); W.Name = TEXT("Cryo Cannon"); W.DamageType = TEXT("cryo"); W.BaseDamage = 35; W.Range = 1;
	W.SpecialEffect = TEXT("Freeze cone. Frozen enemies take double damage from next hit."); WeaponCatalog.Add(W);

	W.Id = TEXT("nanotech_blade"); W.Name = TEXT("Nanotech Blade Swarm"); W.DamageType = TEXT("kinetic"); W.BaseDamage = 45; W.Range = 1;
	W.SpecialEffect = TEXT("Blades fly independently, targeting up to 3 enemies at once."); WeaponCatalog.Add(W);

	W.Id = TEXT("dragon_claw"); W.Name = TEXT("Dragon Claw Gauntlet"); W.DamageType = TEXT("fire"); W.BaseDamage = 60; W.Range = 0;
	W.SpecialEffect = TEXT("Ancient Language fire enchantment. Bleed over time. +Dragon bond."); WeaponCatalog.Add(W);

	W.Id = TEXT("pym_discs"); W.Name = TEXT("Pym Particle Discs"); W.DamageType = TEXT("size"); W.BaseDamage = 20; W.Range = 2;
	W.SpecialEffect = TEXT("Shrink enemies (half stats) or grow allies (double stats) for 20s."); WeaponCatalog.Add(W);
}

FSuperPowerState* USuperheroSubsystem::GetPowerState(const FString& Id)
{
	for (FSuperPowerState& S : Powers) if (S.Id == Id) return &S;
	return nullptr;
}

bool USuperheroSubsystem::UnlockPower(const FString& Id)
{
	if (GetPowerState(Id)) return false;
	for (const FSuperPowerDef& D : PowerCatalog)
	{
		if (D.Id == Id)
		{
			FSuperPowerState S;
			S.Id = Id; S.Level = 1; S.XP = 0;
			Powers.Add(S);
			if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
			{
				Sess->ShowMessage(FString::Printf(TEXT("POWER UNLOCKED: %s! %s"), *D.Name, *D.PassiveDesc));
				Sess->RecordEvent(FString::Printf(TEXT("Power unlocked: %s"), *D.Name));
			}
			return true;
		}
	}
	return false;
}

void USuperheroSubsystem::AddPowerXP(const FString& Id, int32 XP)
{
	FSuperPowerState* S = GetPowerState(Id);
	if (!S) return;
	S->XP += XP;
	int32 Need = S->Level * 50;
	while (S->XP >= Need)
	{
		S->XP -= Need;
		S->Level++;
		Need = S->Level * 50;
		for (const FSuperPowerDef& D : PowerCatalog)
			if (D.Id == Id && S->Level == D.UnlockLevel)
				if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
					Sess->ShowMessage(FString::Printf(TEXT("%s reaches level %d!"), *D.Name, S->Level));
	}
}

int32 USuperheroSubsystem::ActivatePower(const FString& Id)
{
	FSuperPowerState* S = GetPowerState(Id);
	if (!S || S->Cooldown > 0.f) return 0;
	for (const FSuperPowerDef& D : PowerCatalog)
	{
		if (D.Id == Id)
		{
			S->Cooldown = D.CooldownSecs;
			AddPowerXP(Id, 10);
			return 20 + S->Level * 3; // damage scales with level
		}
	}
	return 0;
}

void USuperheroSubsystem::EquipSuit(const FString& Id)
{
	for (int32 i = 0; i < SuitCatalog.Num(); ++i)
	{
		if (SuitCatalog[i].Id == Id)
		{
			EquippedIdx = i;
			if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
				Sess->ShowMessage(FString::Printf(TEXT("Suit equipped: %s. %s"), *SuitCatalog[i].Name, *SuitCatalog[i].SpecialTrait));
			return;
		}
	}
}

void USuperheroSubsystem::EquipWeapon(const FString& Id)
{
	for (int32 i = 0; i < WeaponCatalog.Num(); ++i)
	{
		if (WeaponCatalog[i].Id == Id)
		{
			WeaponIdx = i;
			if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
				Sess->ShowMessage(FString::Printf(TEXT("Weapon equipped: %s. Damage: %d. %s"), *WeaponCatalog[i].Name, WeaponCatalog[i].BaseDamage, *WeaponCatalog[i].SpecialEffect));
			return;
		}
	}
}
