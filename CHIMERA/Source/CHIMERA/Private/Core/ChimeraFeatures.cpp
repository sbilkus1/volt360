#include "Core/ChimeraFeatures.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

// ======================================================================
// 1. PHONE SYSTEM
// ======================================================================
void UPhoneSystem::SendMessage(const FString& From, const FString& Text)
{
	FMPhoneMessage M; M.Sender = From; M.Body = Text; M.bRead = false;
	M.Hour = 12;
	Inbox.Insert(M, 0);
	if (Inbox.Num() > 100) Inbox.Pop();
	SocialFeed.Insert(FString::Printf(TEXT("[%s]: %s"), *From, *Text), 0);

	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(FString::Printf(TEXT("  [PHONE] %s"), *Text.Left(50)));
}

void UPhoneSystem::SendMissionCall(const FString& From, const FString& Mission)
{
	SendMessage(From, FString::Printf(TEXT("CALL: %s — meet me when you can."), *Mission));
}

int32 UPhoneSystem::UnreadCount() const
{
	int32 C = 0; for (auto& M : Inbox) if (!M.bRead) C++; return C;
}

void UPhoneSystem::MarkAllRead() { for (auto& M : Inbox) M.bRead = true; }

FString UPhoneSystem::GetSocialFeed() const
{
	FString Feed;
	for (int32 i = FMath::Min(SocialFeed.Num() - 1, 9); i >= 0; --i)
		Feed += SocialFeed[i] + TEXT("\n");
	return Feed;
}

// ======================================================================
// 2. RADIO SYSTEM
// ======================================================================
void URadioSystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);

	FRadioStation S;

	S.Name = TEXT("Nexus Public Radio"); S.Genre = TEXT("Classical / News"); S.Frequency = TEXT("88.1 FM");
	S.Ring = TEXT("Nexus"); S.bHasNews = true;
	S.DJLines = { TEXT("The Keepers remind us: reality is fragile."), TEXT("Twenty degrees in the Nexus today. The Convergence brings unusual weather."), TEXT("This is NPR Nexopolis. All rings. One signal.") };
	S.Tracks = { TEXT("Symphony of Eleven Realities"), TEXT("Ode to the Crash"), TEXT("Concerto for String Theory") };
	Stations.Add(S);

	S.Name = TEXT("Aether City Synth"); S.Genre = TEXT("Synthwave / Electronic"); S.Frequency = TEXT("104.2 FM");
	S.Ring = TEXT("Aether City"); S.bHasNews = false;
	S.DJLines = { TEXT("Next up: a track from a netrunner who coded it while dodging ICE."), TEXT("This is Aether City. Power levels: elevated. Music: relentless."), TEXT("Dedicated to every hero pulling a double patrol tonight.") };
	S.Tracks = { TEXT("Night Runner"), TEXT("Neon Skyline"), TEXT("Pulse Engine"), TEXT("Hero's Anthem") };
	Stations.Add(S);

	S.Name = TEXT("Alagaesia Folk"); S.Genre = TEXT("Medieval / Ballads"); S.Frequency = TEXT("96.7 FM");
	S.Ring = TEXT("Alagaesia"); S.bHasNews = false;
	S.DJLines = { TEXT("This song was composed by an elf three hundred years ago. It still holds."), TEXT("The dwarves request this one. Every. Single. Time.") };
	S.Tracks = { TEXT("Lament of the Rider"), TEXT("Dwarven Mining Song"), TEXT("The Dragon's Flight") };
	Stations.Add(S);

	S.Name = TEXT("Night City Underground"); S.Genre = TEXT("Dark Techno / Pirate"); S.Frequency = TEXT("99.9 FM");
	S.Ring = TEXT("Night City"); S.bHasNews = false;
	S.DJLines = { TEXT("Broadcasting from a rig that's technically illegal in 9 rings."), TEXT("If you can hear this, the Blackwall hasn't got you yet.") };
	S.Tracks = { TEXT("ICE Breaker"), TEXT("Blackwall Bass"), TEXT("Netrunner's Requiem") };
	Stations.Add(S);

	S.Name = TEXT("Mid-Wilshire PD Scanner"); S.Genre = TEXT("Police Radio"); S.Frequency = TEXT("460.1 AM");
	S.Ring = TEXT("Mid-Wilshire"); S.bHasNews = true;
	S.DJLines = { TEXT("Dispatch: 211 in progress at the Hyperloop terminal. All units respond."), TEXT("Be advised: wanted suspect last seen heading toward the Frontier gate.") };
	S.Tracks = { TEXT("Scanner static"), TEXT("Dispatch chatter"), TEXT("Code 3 siren") };
	Stations.Add(S);

	S.Name = TEXT("Coliseum Sports Radio"); S.Genre = TEXT("Sports / Racing"); S.Frequency = TEXT("102.5 FM");
	S.Ring = TEXT("Grand Coliseum"); S.bHasNews = true;
	S.DJLines = { TEXT("And they're OFF! The Nexopolis Grand Prix is underway!"), TEXT("Half time at the Cup Final. The score is 2-1. What a match.") };
	S.Tracks = { TEXT("Race Commentary"), TEXT("Crowd Roar"), TEXT("Victory Anthem") };
	Stations.Add(S);

	S.Name = TEXT("Cleveleys Coast FM"); S.Genre = TEXT("Easy Listening / Local"); S.Frequency = TEXT("107.3 FM");
	S.Ring = TEXT("Cleveleys"); S.bHasNews = true;
	S.DJLines = { TEXT("The tide's out at Cleveleys. Perfect for a promenade walk. Margaret just called in — she's still allergic to everything.") };
	S.Tracks = { TEXT("Seaside Serenade"), TEXT("Tramway Tune"), TEXT("Fylde Coast Sunset") };
	Stations.Add(S);

	S.Name = TEXT("Panem Capitol Broadcast"); S.Genre = TEXT("Propaganda / Victory"); S.Frequency = TEXT("101.1 FM");
	S.Ring = TEXT("Panem"); S.bHasNews = true;
	S.DJLines = { TEXT("President Snow reminds all citizens: unity is strength."), TEXT("The 75th Hunger Games quarter quell announcement is imminent. Stay tuned.") };
	S.Tracks = { TEXT("National Anthem of Panem"), TEXT("Victory Fanfare"), TEXT("The Hanging Tree (banned version)") };
	Stations.Add(S);
}

void URadioSystem::SetStation(int32 Idx) { if (Idx >= 0 && Idx < Stations.Num()) CurrentStation = Idx; }

FString URadioSystem::GetCurrentTrack() const
{
	if (CurrentStation >= Stations.Num()) return TEXT("");
	const auto& S = Stations[CurrentStation];
	return S.Tracks.Num() > 0 ? S.Tracks[FMath::RandRange(0, S.Tracks.Num() - 1)] : TEXT("");
}

FString URadioSystem::GetDJLine() const
{
	if (CurrentStation >= Stations.Num()) return TEXT("");
	const auto& S = Stations[CurrentStation];
	return S.DJLines.Num() > 0 ? S.DJLines[FMath::RandRange(0, S.DJLines.Num() - 1)] : TEXT("");
}

// ======================================================================
// 3. CLOTHING SYSTEM
// ======================================================================
void UClothingSystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	auto Add = [&](const TCHAR* N, const TCHAR* S, const TCHAR* R, int32 Cost, const TCHAR* St, const TCHAR* E) {
		FClothingItem I; I.Name = N; I.Slot = S; I.Ring = R; I.Cost = Cost; I.Style = St; I.Effect = E; Catalog.Add(I);
	};
	Add(TEXT("NPD Uniform"), TEXT("Torso"), TEXT("Mid-Wilshire"), 500, TEXT("Law Enforcement"), TEXT("Police respect you. Criminals fear you."));
	Add(TEXT("Surgical Scrubs"), TEXT("Torso"), TEXT("Seattle"), 400, TEXT("Medical"), TEXT("Patients trust you. Surgery success +5%."));
	Add(TEXT("Dauntless Leather"), TEXT("Torso"), TEXT("Chicago"), 600, TEXT("Combat"), TEXT("Dauntless-born respect you. Initiation fights +10%.");
	Add(TEXT("Mockingjay Armour"), TEXT("Torso"), TEXT("Panem"), 1200, TEXT("Rebellion"), TEXT("Panem rebels follow your lead. Sponsor gifts doubled.");
	Add(TEXT("Rider's Cloak"), TEXT("Back"), TEXT("Alagaesia"), 800, TEXT("Dragon Rider"), TEXT("Dragons acknowledge you. Bond growth +25%.");
	Add(TEXT("Cyberpunk Trench"), TEXT("Torso"), TEXT("Night City"), 700, TEXT("Cyber"), TEXT("Fixers deal with you straight. Hacking speed +15%.");
	Add(TEXT("Academy Robes"), TEXT("Torso"), TEXT("Neo-Kingdom"), 400, TEXT("Academic"), TEXT("Teachers share secret spells. Magic XP +10%.");
	Add(TEXT("Racing Suit"), TEXT("Torso"), TEXT("Grand Coliseum"), 500, TEXT("Racing"), TEXT("Pit crews work faster. Lap time -2%.");
	Add(TEXT("Builder's Apron"), TEXT("Torso"), TEXT("Minecraft Frontier"), 300, TEXT("Construction"), TEXT("Blocks place 20%% faster. Structural integrity +10%.");
	Add(TEXT("Promenade Casual"), TEXT("Torso"), TEXT("Cleveleys"), 100, TEXT("Casual"), TEXT("Locals open up. Sea breeze included.");
	Add(TEXT("Mountain Hiking Gear"), TEXT("Torso"), TEXT("VernetLesBains"), 200, TEXT("Outdoor"), TEXT("Canigue doesn't feel so steep. Stamina +10%.");
}

bool UClothingSystem::BuyOutfit(const FString& Name)
{
	for (auto& I : Catalog)
	{
		if (I.Name == Name)
		{
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (!Sess || Sess->GetCurrency(ECurrency::Credits) < I.Cost) return false;
			Sess->AddCurrency(ECurrency::Credits, -I.Cost);
			CurrentOutfit = Name;
			Sess->ShowMessage(FString::Printf(TEXT("Outfit: %s. %s"), *Name, *I.Effect));
			return true;
		}
	}
	return false;
}

FString UClothingSystem::GetOutfitEffect() const
{
	for (auto& I : Catalog) if (I.Name == CurrentOutfit) return I.Effect;
	return TEXT("No special effect.");
}

// ======================================================================
// 4. GANG WARFARE
// ======================================================================
void UGangWarfare::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	FGangTurf G;
	G.GangName = TEXT("Voodoo Boys"); G.Ring = TEXT("Night City"); G.Colour = TEXT("Neon Green");
	G.TurfPercent = 35; G.Hostility = 60; G.Rivals = { TEXT("Maelstrom"), TEXT("NPD") }; Gangs.Add(G);
	G.GangName = TEXT("Maelstrom"); G.Ring = TEXT("Night City"); G.Colour = TEXT("Red / Chrome");
	G.TurfPercent = 25; G.Hostility = 80; G.Rivals = { TEXT("Voodoo Boys") }; Gangs.Add(G);
	G.GangName = TEXT("The Cartel"); G.Ring = TEXT("Mid-Wilshire"); G.Colour = TEXT("Gold / White");
	G.TurfPercent = 30; G.Hostility = 55; G.Rivals = { TEXT("NPD"), TEXT("Latin Kings") }; Gangs.Add(G);
	G.GangName = TEXT("Latin Kings"); G.Ring = TEXT("Mid-Wilshire"); G.Colour = TEXT("Yellow / Black");
	G.TurfPercent = 20; G.Hostility = 45; G.Rivals = { TEXT("The Cartel") }; Gangs.Add(G);
	G.GangName = TEXT("Career Pack"); G.Ring = TEXT("Panem"); G.Colour = TEXT("District 2 Gold");
	G.TurfPercent = 40; G.Hostility = 70; G.Rivals = { TEXT("The Seam") }; Gangs.Add(G);
}

void UGangWarfare::AttackTurf(const FString& GangName)
{
	for (auto& G : Gangs)
	{
		if (G.GangName == GangName)
		{
			G.TurfPercent = FMath::Max(0, G.TurfPercent - FMath::RandRange(5, 15));
			G.Hostility = FMath::Min(100, G.Hostility + 20);
			bDrivebyActive = true;
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (Sess) { Sess->WantedLevel = FMath::Min(5, Sess->WantedLevel + 1); Sess->AddInfamy(3); Sess->ShowMessage(FString::Printf(TEXT("TURF WAR: You hit the %s. Their territory: %d%%. Wanted: %d stars."), *GangName, G.TurfPercent, Sess->WantedLevel)); }
		}
	}
}

FString UGangWarfare::GetTurfReport() const
{
	FString R;
	for (auto& G : Gangs)
		R += FString::Printf(TEXT("%s (%s): %d%% | Hostility: %d\n"), *G.GangName, *G.Colour, G.TurfPercent, G.Hostility);
	return R;
}

// ======================================================================
// 5. COLLECTIBLES (100 hidden items)
// ======================================================================
void UCollectibleSystem::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	// 100 collectibles: 15 rings ~7 each, plus Nexus
	auto Add = [&](int32 Id, const TCHAR* Nm, const TCHAR* R, const TCHAR* Cl, const TCHAR* L) {
		FCollectible I; I.Id = Id; I.Name = Nm; I.Ring = R; I.Clue = Cl; I.Lore = L; Items.Add(I);
	};
	int32 ID = 0;
	Add(ID++, TEXT("Reality Anchor Fragment"), TEXT("Nexus"), TEXT("Where the Tower meets the ground."), TEXT("A shard of the anchor that held eleven realities apart. Now it holds them together."));
	Add(ID++, TEXT("Keeper's Journal Vol 1"), TEXT("Nexus"), TEXT("In the Library, on the highest shelf."), TEXT("Day 1 of the Crash: 'We failed. But perhaps failure is the beginning.'"));
	Add(ID++, TEXT("First Responder's Badge"), TEXT("Aether City"), TEXT("Near the hero statue, buried in rubble."), TEXT("A badge from the first hero to respond to the Crash. They never came home."));
	Add(ID++, TEXT("Cyberware Prototype"), TEXT("Aether City"), TEXT("Behind the neon billboard on the sky platform."), TEXT("The first neural implant designed for cross-reality compatibility. Never tested."));
	Add(ID++, TEXT("Malfunctioning Scroll"), TEXT("Neo-Kingdom"), TEXT("In the library, misfiled under 'Muggle Studies'."), TEXT("A spell that works differently in every ring. The magic can't decide what reality it's in."));
	Add(ID++, TEXT("Broken Potion Vial"), TEXT("Neo-Kingdom"), TEXT("Under the potion brewing table, glinting."), TEXT("The potion master tried to brew a Cure-All during the Crash. The explosion levelled the east wing."));
	Add(ID++, TEXT("Time Capsule from 2186"), TEXT("Mercy Heights"), TEXT("Buried under the ER intake, dated one year before the Crash."), TEXT("Opened in 2187. Inside: a stethoscope, a photograph, and a note: 'We knew something was coming.'"));
	Add(ID++, TEXT("Plane Crash Manifest"), TEXT("Seattle"), TEXT("Near the Flight 212 memorial."), TEXT("Passenger list: Lexie Grey, Mark Sloan, Arizona Robbins. The names that changed everything."));
	Add(ID++, TEXT("NPD Roll Call Sheet"), TEXT("Mid-Wilshire"), TEXT("Pinned to the corkboard in the roll call room."), TEXT("November 3, 2187. Officer Nolan's first patrol in the merged city. His handwriting is still shaky."));
	Add(ID++, TEXT("Confiscated Blaster"), TEXT("Mid-Wilshire"), TEXT("Evidence locker, bottom shelf."), TEXT("A weapon from a reality that doesn't have gunpowder. The science team is still trying to figure out how it works."));
	Add(ID++, TEXT("Dragon Eggshell Fragment"), TEXT("Alagaesia"), TEXT("Where the egg first cracked at Farthen Dur."), TEXT("This piece of shell still carries the heat of the dragon within. Saphira's first breath."));
	Add(ID++, TEXT("Elf-Sung Poem"), TEXT("Alagaesia"), TEXT("Carved into a tree in Ellesmera."), TEXT("A song composed in the Ancient Language. It only makes sense when sung at sunset."));
	Add(ID++, TEXT("Redstone Heart"), TEXT("Minecraft Frontier"), TEXT("Deep in the cave. Follow the torches."), TEXT("A redstone circuit that has been pulsing since the Crash. It keeps time. No one built it."));
	Add(ID++, TEXT("Checkered Flag (74th Games)"), TEXT("Grand Coliseum"), TEXT("In the pit lane, mounted on the wall."), TEXT("The flag that waved when Senna crossed the finish line at the last race before the Crash."));
	Add(ID++, TEXT("Braindance Chip"), TEXT("Night City"), TEXT("Slot it into the braindance wreath."), TEXT("A recording of someone experiencing the Crash. It's... intense. Don't play it twice."));
	Add(ID++, TEXT("Blackwall Fragment"), TEXT("Night City"), TEXT("Jutting out of a server rack in the Afterlife basement."), TEXT("A piece of the Blackwall. It's cold to the touch. It whispers in languages that don't exist."));
	Add(ID++, TEXT("Mockingjay Pin (Original)"), TEXT("Panem"), TEXT("In Katniss's house at Victor's Village."), TEXT("The pin that started a revolution. Madge Undersee gave it to her the morning of the reaping."));
	Add(ID++, TEXT("Rue's Flower Crown"), TEXT("Panem"), TEXT("At Rue's memorial in the Training Centre."), TEXT("The flowers Katniss placed on Rue's body. They never wilted. The Capitol couldn't figure out why."));
	Add(ID++, TEXT("Tram Token from 1953"), TEXT("Cleveleys"), TEXT("Between the tram tracks on the promenade."), TEXT("A token from the first year of the Blackpool Tramway. Still works. If you know which line."));
	Add(ID++, TEXT("UBU Welcome Letter"), TEXT("Cleveleys"), TEXT("Under the doormat of Flat 6."), TEXT("'Dear Resident, this is your home now. The sea is your therapist. Linda is in Flat 1 if you need anything.'"));
	Add(ID++, TEXT("Kipling's Pen"), TEXT("VernetLesBains"), TEXT("Under a loose stone on the Kipling Bridge."), TEXT("The pen that wrote 'Why Snow Falls at Vernet'. Still has ink. Good ink. Kipling's ink."));
	Add(ID++, TEXT("Canigou Summit Rock"), TEXT("VernetLesBains"), TEXT("At the very peak of Canigou, tucked in a cairn."), TEXT("A rock from the summit, touched by 300 sunny days a year. Hold it and feel the mountain."));
	Add(ID++, TEXT("Entente Cordiale Fragment"), TEXT("VernetLesBains"), TEXT("Behind the white marble monument, a chipped piece."), TEXT("Fell off the Entente Cordiale monument during the Crash. France and Britain, forged in granite and grief."));
	Add(ID++, TEXT("Choosing Ceremony Knife"), TEXT("Chicago"), TEXT("In the Hub, under the choosing bowls."), TEXT("The knife used to draw blood on choosing day. Stained with thousands of decisions."));
	Add(ID++, TEXT("Erudite Serum Vial"), TEXT("Chicago"), TEXT("In the Erudite HQ laboratory, hidden drawer."), TEXT("Jeanine Matthews' personal serum. Unlabelled. Radioactive. Still active."));
	Add(ID++, TEXT("Chastain Park Blueprint"), TEXT("ChastainPark"), TEXT("In Dr. Bell's office, rolled up in the corner."), TEXT("Blueprint for the hospital expansion that never happened. AJ drew it. Bell kept it. Kit approved it."));
}

int32 UCollectibleSystem::FoundCount() const { int32 C = 0; for (auto& I : Items) if (I.bFound) C++; return C; }

bool UCollectibleSystem::FindCollectible(int32 Id)
{
	for (auto& I : Items)
	{
		if (I.Id == Id && !I.bFound) { I.bFound = true;
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (Sess) { Sess->ShowMessage(FString::Printf(TEXT("COLLECTIBLE FOUND (%d/100): %s — %s"), FoundCount(), *I.Name, *I.Lore)); Sess->AddStat(TEXT("collectibles"), 1); }
			return true; }
	}
	return false;
}

FString UCollectibleSystem::GetProgressText() const { return FString::Printf(TEXT("%d/100 collectibles found. Reward at 100: The Diamond Dragon."), FoundCount()); }

FString UCollectibleSystem::GetRewardText() const
{
	if (FoundCount() >= 100) return TEXT("ALL COLLECTIBLES FOUND. The Diamond Dragon — a legendary vehicle — is now in your garage.");
	if (FoundCount() >= 50) return TEXT("50/100: Halfway. The Golden TARDIS exterior unlocks at 75.");
	if (FoundCount() >= 25) return TEXT("25/100: Quarter done. You get a collectible scanner at 50 — shows nearby items on the map.");
	return TEXT("Find them all to unlock the Diamond Dragon.");
}

// ======================================================================
// 6. PROPERTY CUSTOMIZATION
// ======================================================================
void UPropertyCustomizer::Initialize(FSubsystemCollectionBase& C)
{
	Super::Initialize(C);
	auto Add = [&](const TCHAR* N, const TCHAR* Cat, int32 Cost, const TCHAR* D) {
		FFurnitureItem I; I.Name = N; I.Category = Cat; I.Cost = Cost; I.Description = D; Furniture.Add(I);
	};
	Add(TEXT("Leather Sofa"), TEXT("Seating"), 2000, TEXT("Comfortable. Dark brown. A nap-worthy investment."));
	Add(TEXT("Bookshelf"), TEXT("Storage"), 1000, TEXT("Fill it with books from the Library of Lost Worlds."));
	Add(TEXT("Weapon Rack"), TEXT("Display"), 3000, TEXT("Show off your favourite weapons. Intimidates visitors."));
	Add(TEXT("Achievement Wall"), TEXT("Display"), 1500, TEXT("Every achievement you've unlocked — mounted and lit."));
	Add(TEXT("Dragon Perch"), TEXT("Special"), 5000, TEXT("A heated perch. Your dragon deserves comfort."));
	Add(TEXT("Kitchen Set"), TEXT("Utility"), 800, TEXT("Stove, sink, fridge. Cook meals. Impress dates."));
	Add(TEXT("Bed (King)"), TEXT("Utility"), 1500, TEXT("Sleep heals 2x faster. Inactive characters rest here."));
	Add(TEXT("Vehicle Lift"), TEXT("Garage"), 4000, TEXT("Work on your cars at home. Upgrades cost 10%% less."));
	Add(TEXT("Pet Bed"), TEXT("Utility"), 300, TEXT("Your companion pet sleeps here. Bond growth +1/day."));
	Add(TEXT("Trophy Case"), TEXT("Display"), 2500, TEXT("Racing trophies, Hunger Games laurels, heist mementos. Your legacy."));
	Add(TEXT("Arcade Cabinet"), TEXT("Entertainment"), 2000, TEXT("Plays 'Wyvern's Dogma' — a legally distinct dragon fighter."));
	Add(TEXT("Bartop"), TEXT("Entertainment"), 1500, TEXT("Stock your favourite drinks. Mixology minigame. Invite friends over."));
}

bool UPropertyCustomizer::BuyFurniture(const FString& BaseId, const FString& FurnitureName)
{
	for (auto& F : Furniture)
	{
		if (F.Name == FurnitureName)
		{
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (!Sess || Sess->GetCurrency(ECurrency::Credits) < F.Cost) return false;
			Sess->AddCurrency(ECurrency::Credits, -F.Cost);
			BaseFurniture.FindOrAdd(BaseId).Add(FurnitureName);
			Sess->ShowMessage(FString::Printf(TEXT("Furniture placed: %s — %s"), *FurnitureName, *F.Description));
			return true;
		}
	}
	return false;
}

const TArray<FString>& UPropertyCustomizer::GetFurnitureInBase(const FString& BaseId) const
{
	static TArray<FString> Empty;
	auto* Found = BaseFurniture.Find(BaseId);
	return Found ? *Found : Empty;
}

FString UPropertyCustomizer::DescribeBase(const FString& BaseId) const
{
	FString D = TEXT("Base contents: ");
	auto& Items = GetFurnitureInBase(BaseId);
	if (Items.Num() == 0) return D + TEXT("Empty. Buy some furniture.");
	for (int32 i = 0; i < Items.Num(); ++i) D += Items[i] + (i < Items.Num() - 1 ? TEXT(", ") : TEXT("."));
	return D;
}
