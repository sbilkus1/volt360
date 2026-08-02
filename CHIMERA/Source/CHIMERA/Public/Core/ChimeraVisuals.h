// GDD 4 / Appendix — Visual Reference Guide & Character Model Database
// For 3D artists: architectural styles, character appearances, vehicle designs.
// Every entry references real-world images searchable on Google Images.
//
// SEARCH REFERENCE IMAGES:
//   "Cleveleys Victoria Road street view"
//   "Vernet-les-Bains thermal baths architecture"
//   "Grey Sloan Memorial Hospital exterior"
//   "Chicago Med Gaffney Medical Center building"
//   "Chastain Park Memorial Hospital The Resident"
//   "The Rookie Mid-Wilshire station exterior"
//   "Afterlife Casino Cyberpunk interior"
//   "Dauntless compound Divergent set design"
//   "Panem Capitol building Hunger Games"
//   "Alagaesia Farthen Dur concept art"
//   "Nexus Tower fantasy hub architecture"

#pragma once
#include "CoreMinimal.h"
#include "ChimeraVisuals.generated.h"

USTRUCT()
struct FCharacterModel
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() FString Show;          // which show/book/film
	UPROPERTY() FString Actor;         // real actor for likeness reference
	UPROPERTY() FString Appearance;    // key visual traits
	UPROPERTY() FString Outfit;        // signature clothing/armour
	UPROPERTY() FString SearchTerms;   // Google Images search string
};

USTRUCT()
struct FBuildingStyle
{
	GENERATED_BODY()
	UPROPERTY() FString Ring;
	UPROPERTY() FString Style;         // architectural period/style
	UPROPERTY() FString Materials;     // stone, glass, steel, timber, etc
	UPROPERTY() FString Roof;          // flat, pitched, dome, spire, thatched
	UPROPERTY() FString Windows;       // mullioned, floor-to-ceiling, arched, bay
	UPROPERTY() FString Colors;        // dominant colour palette
	UPROPERTY() FString Landmarks;     // distinctive features visible from distance
	UPROPERTY() FString RealReference; // what real place to look at for reference
};

namespace ChimeraVisualGuide
{
	// ==================================================================
	// CHARACTER MODEL DATABASE — every major NPC with visual reference
	// ==================================================================
	inline TArray<FCharacterModel> GetCharacters()
	{
		TArray<FCharacterModel> C;

		// === THE ROOKIE (Mid-Wilshire) ===
		C.Add({ TEXT("John Nolan"), TEXT("The Rookie"), TEXT("Nathan Fillion"),
			TEXT("Grey hair, mid-50s, kind eyes, fit build, 6'1\""),
			TEXT("NPD uniform: navy blue short-sleeve shirt, badge, duty belt with Glock, body cam"),
			TEXT("\"Nathan Fillion The Rookie John Nolan uniform\""), });
		C.Add({ TEXT("Lucy Chen"), TEXT("The Rookie"), TEXT("Melissa O'Neil"),
			TEXT("Asian-Canadian, early 30s, short dark hair, athletic, 5'5\""),
			TEXT("NPD uniform, patrol boots, hair in tight bun on duty"),
			TEXT("\"Melissa O'Neil The Rookie Lucy Chen\""), });
		C.Add({ TEXT("Tim Bradford"), TEXT("The Rookie"), TEXT("Eric Winter"),
			TEXT("Late 30s, tall, buzz cut, intense expression, ex-Marine bearing, 6'2\""),
			TEXT("NPD uniform with sergeant stripes, mirrored sunglasses, duty vest"),
			TEXT("\"Eric Winter Tim Bradford The Rookie\""), });
		C.Add({ TEXT("Angela Lopez"), TEXT("The Rookie"), TEXT("Alyssa Diaz"),
			TEXT("Latina, dark hair, confident, detective badge on belt"),
			TEXT("Plainclothes detective: blazer over blouse, jeans, service weapon visible"),
			TEXT("\"Alyssa Diaz Angela Lopez The Rookie\""), });
		C.Add({ TEXT("Nyla Harper"), TEXT("The Rookie"), TEXT("Mekia Cox"),
			TEXT("Black woman, late 30s, athletic build, sharp eyes, 5'7\""),
			TEXT("Undercover detective: leather jacket, tactical pants, radio earpiece"),
			TEXT("\"Mekia Cox Nyla Harper The Rookie\""), });
		C.Add({ TEXT("Wade Grey"), TEXT("The Rookie"), TEXT("Richard T. Jones"),
			TEXT("Black man, 60s, distinguished, greying, authoritative, 6'0\""),
			TEXT("Watch Commander: white uniform shirt with gold badge, tie"),
			TEXT("\"Richard T Jones Wade Grey The Rookie\""), });

		// === GREY'S ANATOMY (Seattle) ===
		C.Add({ TEXT("Meredith Grey"), TEXT("Grey's Anatomy"), TEXT("Ellen Pompeo"),
			TEXT("Early 50s, blonde, blue surgical scrubs or white coat, determined expression"),
			TEXT("Navy surgical scrubs with 'Grey-Sloan Memorial' patch, stethoscope, long white coat"),
			TEXT("\"Ellen Pompeo Meredith Grey surgical scrubs\""), });
		C.Add({ TEXT("Miranda Bailey"), TEXT("Grey's Anatomy"), TEXT("Chandra Wilson"),
			TEXT("Short, stocky, intense, dark skin, sharp gaze, 5'0\""),
			TEXT("Surgical scrubs, short white coat, clipboard always in hand"),
			TEXT("\"Chandra Wilson Miranda Bailey Grey's Anatomy\""), });
		C.Add({ TEXT("Richard Webber"), TEXT("Grey's Anatomy"), TEXT("James Pickens Jr."),
			TEXT("Late 60s, bald, greying beard, wise, calming presence"),
			TEXT("Long white attending coat over dress shirt, tie, gold watch"),
			TEXT("\"James Pickens Richard Webber Grey's Anatomy\""), });
		C.Add({ TEXT("Amelia Shepherd"), TEXT("Grey's Anatomy"), TEXT("Caterina Scorsone"),
			TEXT("Early 40s, dark wavy hair, expressive, neurosurgeon loupes on head"),
			TEXT("Surgical scrubs, long white coat, neuro loupes pushed up on forehead"),
			TEXT("\"Caterina Scorsone Amelia Shepherd\""), });
		C.Add({ TEXT("Owen Hunt"), TEXT("Grey's Anatomy"), TEXT("Kevin McKidd"),
			TEXT("Mid 40s, red-brown hair, military bearing, trauma vest over scrubs"),
			TEXT("Muted trauma scrubs, military dog tags, combat boots"),
			TEXT("\"Kevin McKidd Owen Hunt trauma surgeon\""), });

		// === CHICAGO MED ===
		C.Add({ TEXT("Will Halstead"), TEXT("Chicago Med"), TEXT("Nick Gehlfuss"),
			TEXT("Early 40s, brown hair, athletic, Chicago ED scrubs, 6'1\""),
			TEXT("ED scrubs, stethoscope, tired eyes from double shift"),
			TEXT("\"Nick Gehlfuss Will Halstead Chicago Med\""), });
		C.Add({ TEXT("Dr. Charles"), TEXT("Chicago Med"), TEXT("Oliver Platt"),
			TEXT("60s, white hair, glasses, cardigan under white coat, gentle smile"),
			TEXT("Coat over sweater vest, glasses on chain, calm demeanour"),
			TEXT("\"Oliver Platt Dr Charles Chicago Med\""), });
		C.Add({ TEXT("Ethan Choi"), TEXT("Chicago Med"), TEXT("Brian Tee"),
			TEXT("Early 40s, Asian-American, military bearing, navy veteran tattoo"),
			TEXT("ED attending scrubs, running shoes ready to sprint to trauma"),
			TEXT("\"Brian Tee Ethan Choi Chicago Med\""), });

		// === THE RESIDENT (Chastain Park, Atlanta) ===
		C.Add({ TEXT("Conrad Hawkins"), TEXT("The Resident"), TEXT("Matt Czuchry"),
			TEXT("Late 30s, brown hair, intense, sleeves rolled up, 5'9\""),
			TEXT("Scrubs with sleeves rolled, stethoscope, tablet in back pocket"),
			TEXT("\"Matt Czuchry Conrad Hawkins\""), });
		C.Add({ TEXT("Randolph Bell"), TEXT("The Resident"), TEXT("Bruce Greenwood"),
			TEXT("60s, silver hair, tall, commanding, tremor in left hand (character arc)"),
			TEXT("Chief of Surgery coat, gold-rimmed glasses, tremor brace"),
			TEXT("\"Bruce Greenwood Randolph Bell The Resident\""), });
		C.Add({ TEXT("AJ Austin"), TEXT("The Resident"), TEXT("Malcolm-Jamal Warner"),
			TEXT("Early 50s, black, bald, intense, cardio-thoracic, 'The Raptor'"),
			TEXT("Bold-patterned surgical cap, designer scrubs, supreme confidence"),
			TEXT("\"Malcolm-Jamal Warner AJ Austin The Raptor\""), });

		// === HUNGER GAMES (Panem) ===
		C.Add({ TEXT("Katniss Everdeen"), TEXT("The Hunger Games"), TEXT("Jennifer Lawrence"),
			TEXT("Early 20s, dark braided hair, olive skin, grey eyes, lean build, 5'7\""),
			TEXT("Mockingjay armour (black with red accents), bow, arrow quiver, side braid"),
			TEXT("\"Jennifer Lawrence Katniss Mockingjay costume\""), });
		C.Add({ TEXT("Peeta Mellark"), TEXT("The Hunger Games"), TEXT("Josh Hutcherson"),
			TEXT("Early 20s, blonde, kind face, baker's build, 5'7\""),
			TEXT("Capitol-issue jacket or baker's apron, camouflage-painted face (book arc)"),
			TEXT("\"Josh Hutcherson Peeta Mellark\""), });
		C.Add({ TEXT("President Snow"), TEXT("The Hunger Games"), TEXT("Donald Sutherland"),
			TEXT("70s, white hair, white beard, blood-tainted breath (roses), cold smile"),
			TEXT("Capitol suit (white/grey with rose in lapel), ornate presidential chair"),
			TEXT("\"Donald Sutherland President Snow white rose\""), });
		C.Add({ TEXT("Effie Trinket"), TEXT("The Hunger Games"), TEXT("Elizabeth Banks"),
			TEXT("50s, elaborate Capitol fashion, pink wig, exaggerated makeup"),
			TEXT("Flamboyant Capitol dress, matching shoes, parasol, 'May the odds' expression"),
			TEXT("\"Elizabeth Banks Effie Trinket Capitol fashion\""), });

		// === DIVERGENT (Chicago) ===
		C.Add({ TEXT("Tris Prior"), TEXT("Divergent"), TEXT("Shailene Woodley"),
			TEXT("Late teens, blonde, short pixie cut (after choosing Dauntless), 5'8\""),
			TEXT("Dauntless black clothing, tattoos (3 ravens on collarbone), combat boots"),
			TEXT("\"Shailene Woodley Tris Prior Dauntless\""), });
		C.Add({ TEXT("Four"), TEXT("Divergent"), TEXT("Theo James"),
			TEXT("20s, dark hair, tattooed (faction symbols on back), intense eyes, 6'0\""),
			TEXT("Dauntless instructor gear: black top, combat pants, back tattoo visible"),
			TEXT("\"Theo James Four Tobias Eaton Divergent\""), });
		C.Add({ TEXT("Jeanine Matthews"), TEXT("Divergent"), TEXT("Kate Winslet"),
			TEXT("40s, blonde bob, cold blue eyes, Erudite blue clothing, calculating"),
			TEXT("Erudite blue suit, glasses, tablet in hand, serum vials at waist"),
			TEXT("\"Kate Winslet Jeanine Matthews Erudite blue\""), });

		// === ERAGON (Alagaësia) ===
		C.Add({ TEXT("Eragon"), TEXT("Eragon"), TEXT("Ed Speleers"),
			TEXT("Late teens, brown hair, Rider's sword (Zar'roc/Brisingr), Rider tattoo on palm"),
			TEXT("Leather armour, worn cloak, Rider's ring, elven-made sword at hip"),
			TEXT("\"Ed Speleers Eragon Rider armour\""), });
		C.Add({ TEXT("Arya"), TEXT("Eragon"), TEXT("Sienna Guillory"),
			TEXT("Elf, raven-black hair, pointed ears, ageless beauty, green eyes"),
			TEXT("Elven leather armour, bow, cloak, no footwear (elves walk silently)"),
			TEXT("\"Sienna Guillory Arya Eragon elf\""), });
		C.Add({ TEXT("Murtagh"), TEXT("Eragon"), TEXT("Garrett Hedlund"),
			TEXT("20s, dark hair, scarred, Thorn's Rider, conflicted expression, Zar'roc"),
			TEXT("Dark armour, red dragon-scale cloak, Rider's sword, haunted eyes"),
			TEXT("\"Garrett Hedlund Murtagh red armour\""), });

		return C;
	}

	// ==================================================================
	// ARCHITECTURAL STYLE GUIDE — how each ring should look
	// ==================================================================
	inline TArray<FBuildingStyle> GetBuildingStyles()
	{
		TArray<FBuildingStyle> B;

		B.Add({ TEXT("Nexus"), TEXT("Neoclassical monumental"),
			TEXT("White marble, gold filigree, obsidian floors"),
			TEXT("Central tower spire (200m), dome with glass oculus"),
			TEXT("Floor-to-ceiling arched, stained glass inserts"),
			TEXT("White, gold, obsidian black, cyan glow"),
			TEXT("Reality Anchor spire, Tower of Convergence, floating books in Library"),
			TEXT("Google: \"Washington DC neoclassical\" + \"fantasy tower concept art\"") });

		B.Add({ TEXT("Aether City"), TEXT("Futurist skyscraper megacity"),
			TEXT("Glass curtain walls, titanium frame, holographic billboards, carbon fibre"),
			TEXT("Flat with helipads and sky gardens"),
			TEXT("Floor-to-ceiling tinted glass, hexagonal grid pattern"),
			TEXT("Silver, cyan, magenta neon, chrome, holographic ads"),
			TEXT("Sky platforms (2000m altitude), hero statues, power grid pylons"),
			TEXT("Google: \"Cyberpunk 2077 city\" + \"futurist architecture skyscraper\"") });

		B.Add({ TEXT("Neo-Kingdom"), TEXT("Fantasy academic castle"),
			TEXT("Limestone, copper roofing (green patina), stained glass, oak beams"),
			TEXT("Steep pitched copper-green, dormer windows, turrets with pennants"),
			TEXT("Stone mullioned gothic, rose window in great hall"),
			TEXT("Cream limestone, verdigris green, royal purple flags, warm gold"),
			TEXT("Flying buttresses, enchanted floating candles, spell rings overhead"),
			TEXT("Google: \"Hogwarts architecture\" + \"medieval castle courtyard\"") });

		B.Add({ TEXT("Mercy Heights"), TEXT("Modernist hospital complex"),
			TEXT("White concrete, glass, stainless steel, rubber flooring"),
			TEXT("Flat with helipad, rooftop garden, solar panels"),
			TEXT("Large horizontal ribbon windows, privacy frosted glass"),
			TEXT("White, hospital green, stainless steel, blue accent strips"),
			TEXT("Ambulance bay with canopy, helicopter pad, rooftop greenhouse"),
			TEXT("Google: \"modern hospital architecture\" + \"Grey Sloan Memorial exterior\"") });

		B.Add({ TEXT("Mid-Wilshire"), TEXT("Brutalist civic + Art Deco downtown"),
			TEXT("Precast concrete (precinct), brick + limestone (downtown), asphalt roads"),
			TEXT("Flat (precinct), stepped Art Deco crowns (downtown highrises)"),
			TEXT("Narrow vertical slits (precinct), large commercial storefronts (street level)"),
			TEXT("Concrete grey, terracotta, yellow street lamps, neon cop-car lights"),
			TEXT("NPD precinct with flagpole, downtown skyline, elevated freeway"),
			TEXT("Google: \"LA police station architecture\" + \"Art Deco downtown Los Angeles\"") });

		B.Add({ TEXT("Alagaësia"), TEXT("Medieval fantasy + Elven organic"),
			TEXT("Granite, timber frame, thatch (human), living wood + crystal (elven)"),
			TEXT("Steep thatch (villages), organic canopy (elf treehouses), stone vault (dwarf halls)"),
			TEXT("Small diamond-paned (human), no glass — open arches (elven), carved stone (dwarf)"),
			TEXT("Grey stone, green moss, warm wood, sapphire blue (elf), gold (dwarf)"),
			TEXT("Farthen Dûr: hollow mountain. Ellesméra: trees grown into halls. Dragon peaks."),
			TEXT("Google: \"Lord of the Rings architecture\" + \"elven treehouse\" + \"dwarf mountain hall\"") });

		B.Add({ TEXT("Night City"), TEXT("Cyberpunk dystopian megacity"),
			TEXT("Corrugated steel, exposed conduit, neon tubes, holographic projections, rain-streaked concrete"),
			TEXT("Flat utilitarian, occasionally crowned with neon megastructure signs"),
			TEXT("Vertical strip (corpo towers), shuttered/smashed (slums), floor-ceiling (corpo penthouses)"),
			TEXT("Dark grey, acid rain stains, neon pink/cyan/yellow, black carbon scoring"),
			TEXT("Afterlife Casino (underground), corpo towers (Arasaka-style), street market stalls, Blackwall rift"),
			TEXT("Google: \"Cyberpunk 2077 Night City architecture\" + \"Blade Runner cityscape\"") });

		B.Add({ TEXT("Panem"), TEXT("Capitol excess + District poverty"),
			TEXT("CAPITOL: marble, gold, oversized columns, holographic displays; DISTRICTS: corrugated iron, weathered wood, coal dust"),
			TEXT("CAPITOL: extravagant domes and spires; DISTRICTS: corrugated lean-to or flat"),
			TEXT("CAPITOL: massive arched, tinted purple/gold; DISTRICTS: small, boarded, patched"),
			TEXT("CAPITOL: violet, gold, white, holographic; DISTRICTS: grey, rust, coal black, faded"),
			TEXT("CAPITOL: President's mansion (rose garden), training centre, tribute apartments; DISTRICT 12: the Seam, the Hob, Victor's Village, the fence"),
			TEXT("Google: \"Hunger Games Capitol architecture\" + \"District 12 Seam\"") });

		B.Add({ TEXT("Cleveleys"), TEXT("Victorian seaside terrace + modern retail"),
			TEXT("Red brick, white render, slate roofs, Victorian cast iron details, modern shopfronts"),
			TEXT("Steep pitched slate, occasional flat (modern extensions)"),
			TEXT("Victorian sash, bay windows on seafront, large retail glass on Victoria Road"),
			TEXT("Red brick, white render, black iron, blue sea, grey slate"),
			TEXT("Promenade wall with wave-shaped sea defences, Blackpool Tower visible, tramway tracks"),
			TEXT("Google: \"Cleveleys Victoria Road street view\" + \"Blackpool promenade architecture\"") });

		B.Add({ TEXT("VernetLesBains"), TEXT("Catalan mountain village + Belle Époque spa"),
			TEXT("Local granite, white stucco render, ceramic roof tiles (terracotta), wrought iron"),
			TEXT("Low-pitched terracotta tile, Catalan style"),
			TEXT("Small shuttered, Juliet balconies with iron railings"),
			TEXT("Terracotta, cream stucco, mountain green, Canigó blue sky"),
			TEXT("Thermal baths (Belle Époque), Entente Cordiale monument (white marble), St Saturnin church on hill, Kipling bridge over River Cady"),
			TEXT("Google: \"Vernet-les-Bains village architecture\" + \"French Pyrenees mountain town\"") });

		B.Add({ TEXT("Minecraft Frontier"), TEXT("Voxel-crafted everything"),
			TEXT("16x16 pixel blocks, pixel-art textures, redstone wiring, nether brick, prismarine"),
			TEXT("Flat block layers, occasional stepped pyramid"),
			TEXT("Glass blocks (1m cube), iron bar grilles, trapdoor shutters"),
			TEXT("Green grass blocks, oak brown, stone grey, white wool, glowing redstone"),
			TEXT("Giant oak trees, pixelated mountains, floating island biomes, portal frame"),
			TEXT("Google: \"Minecraft village\" + \"Minecraft castle build\"") });

		B.Add({ TEXT("Grand Coliseum"), TEXT("Modernist stadium + Art Deco speedway"),
			TEXT("Glass, steel truss, precast concrete, polished granite, racing tarmac"),
			TEXT("Retractable glass roof, flat canopy over pit lane"),
			TEXT("Massive ribbon windows around stadium bowl, pit garage roll-up doors"),
			TEXT("Race red, white, silver, tarmac black, grass green"),
			TEXT("80,000 seat stadium, F1 pit lane, podium, victory arch, scoreboard"),
			TEXT("Google: \"Monza circuit\" + \"Silverstone paddock\" + \"Wembley stadium exterior\"") });

		B.Add({ TEXT("Chicago"), TEXT("American urban grid + Dauntless industrial"),
			TEXT("Brick + limestone downtown, steel frame skyscrapers, concrete brutalist for Dauntless"),
			TEXT("Flat (warehouse), gothic crown (tribune tower style), glass box (modernist)"),
			TEXT("Chicago-style three-part (large fixed, two operable sashes), Dauntless: industrial high bay"),
			TEXT("Brick red, limestone cream, Chicago river green, Dauntless black, Erudite blue"),
			TEXT("The Hub (choosing ceremony), Dauntless Pit, Ferris wheel (Navy Pier), elevated L train"),
			TEXT("Google: \"Chicago architecture skyline\" + \"Divergent Dauntless compound set design\"") });

		B.Add({ TEXT("Seattle"), TEXT("Pacific Northwest contemporary + hospital modern"),
			TEXT("Cedar siding, exposed steel, glass curtain wall, rain-screen panels"),
			TEXT("Low-pitched with deep overhangs (NW style), flat with helipad (hospital)"),
			TEXT("Floor-to-ceiling (hospital rooms), cantilevered with mountain view (residential)"),
			TEXT("Evergreen, grey sky, cedar warm brown, Space Needle white, Puget Sound blue"),
			TEXT("Space Needle visible, Grey-Sloan sky bridge, ferry terminals, pine-covered hills"),
			TEXT("Google: \"Seattle skyline\" + \"modern hospital campus architecture\"") });

		B.Add({ TEXT("ChastainPark"), TEXT("Southern neoclassical revival + modern medical wing"),
			TEXT("Red brick, white columns, magnolia trees, Spanish moss, modern glass extension"),
			TEXT("Steep pitched with dormers (original building), flat modern (new wing)"),
			TEXT("Tall sash windows with shutters, glass curtain wall on modern wing"),
			TEXT("Brick red, white trim, magnolia green, Atlanta skyline distant"),
			TEXT("Columned entry portico, grand staircase, courtyard with fountain"),
			TEXT("Google: \"The Resident Chastain Park hospital exterior\" + \"Atlanta neoclassical building\"") });

		return B;
	}

	// ==================================================================
	// VEHICLE VISUAL REFERENCE — key silhouettes and features
	// ==================================================================
	// Search Google Images for each vehicle to get accurate 3D reference:
	//   "<Make> <Model> <Year> front side rear view"
	//
	// Key models to reference for sculpting:
	//   - Ferrari F40 (1988): low, wide, pop-up headlights, massive rear wing, red
	//   - Lamborghini Countach LP5000 (1985): wedge shape, scissor doors, rear wing
	//   - Porsche 911 Carrera RS 2.7 (1973): ducktail spoiler, wide rear arches
	//   - Ford Mustang GT500 'Eleanor' (1967): fastback, grey/black stripes, side exhausts
	//   - Bugatti Chiron (2022): horseshoe grille, C-curve side, central exhaust
	//   - McLaren F1 (1994): central driving position, gold-lined engine bay
	//   - Toyota Supra MK4 (1993): long nose, double-bubble roof, massive rear wing (optional)
	//   - Nissan Skyline GT-R R34 (1999): boxy silhouette, quad round taillights
	//   - Mercedes 300SL Gullwing (1954): gullwing doors, front-hinged, long hood
	//   - DeLorean DMC-12 (1981): stainless steel body, gullwing doors, no paint
	//
	// Vehicle material reference:
	//   Paint: metallic flake (sports), matte wrap (tuned), candy gloss (classics)
	//   Glass: slightly tinted, reflective (raytraced in UE5)
	//   Wheels: alloy spokes (factory), deep-dish chrome (custom), steelies (budget)
	//   Interior: leather/alcantara (luxury), carbon bucket (race), cloth (economy)
}
