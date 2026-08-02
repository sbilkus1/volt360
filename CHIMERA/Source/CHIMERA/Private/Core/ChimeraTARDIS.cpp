#include "Core/ChimeraTARDIS.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

void UTARDISSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// All Doctors + War Doctor + Fugitive.
	FDoctorDef D;
	D.Number = 1; D.Actor = TEXT("William Hartnell"); D.Era = TEXT("1963-1966"); D.Catchphrase = TEXT("One day, I shall come back."); D.ScrewdriverStyle = TEXT("None — used cunning"); D.CompanionExample = TEXT("Susan, Barbara, Ian"); Doctors.Add(D);
	D.Number = 2; D.Actor = TEXT("Patrick Troughton"); D.Era = TEXT("1966-1969"); D.Catchphrase = TEXT("When I say run, run."); D.ScrewdriverStyle = TEXT("None — used recorder"); D.CompanionExample = TEXT("Jamie, Zoe"); Doctors.Add(D);
	D.Number = 3; D.Actor = TEXT("Jon Pertwee"); D.Era = TEXT("1970-1974"); D.Catchphrase = TEXT("Reverse the polarity of the neutron flow."); D.ScrewdriverStyle = TEXT("Classic silver"); D.CompanionExample = TEXT("Jo Grant, Sarah Jane"); Doctors.Add(D);
	D.Number = 4; D.Actor = TEXT("Tom Baker"); D.Era = TEXT("1974-1981"); D.Catchphrase = TEXT("Would you like a jelly baby?"); D.ScrewdriverStyle = TEXT("Classic silver"); D.CompanionExample = TEXT("Romana, K9, Sarah Jane"); Doctors.Add(D);
	D.Number = 5; D.Actor = TEXT("Peter Davison"); D.Era = TEXT("1981-1984"); D.Catchphrase = TEXT("Brave heart, Tegan."); D.ScrewdriverStyle = TEXT("Classic silver"); D.CompanionExample = TEXT("Tegan, Nyssa, Adric"); Doctors.Add(D);
	D.Number = 6; D.Actor = TEXT("Colin Baker"); D.Era = TEXT("1984-1986"); D.Catchphrase = TEXT("Change, my dear, and it seems not a moment too soon."); D.ScrewdriverStyle = TEXT("Classic silver"); D.CompanionExample = TEXT("Peri"); Doctors.Add(D);
	D.Number = 7; D.Actor = TEXT("Sylvester McCoy"); D.Era = TEXT("1987-1989, 1996"); D.Catchphrase = TEXT("Time and tide melt the snowman."); D.ScrewdriverStyle = TEXT("Classic silver"); D.CompanionExample = TEXT("Ace"); Doctors.Add(D);
	D.Number = 8; D.Actor = TEXT("Paul McGann"); D.Era = TEXT("1996, Night of the Doctor"); D.Catchphrase = TEXT("I'm a Doctor, but probably not the one you were expecting."); D.ScrewdriverStyle = TEXT("Victorian brass"); D.CompanionExample = TEXT("Charley, Lucie"); Doctors.Add(D);
	D.Number = -1; D.Actor = TEXT("John Hurt"); D.Era = TEXT("50th Anniversary"); D.Catchphrase = TEXT("Great men are forged in fire."); D.ScrewdriverStyle = TEXT("War Doctor sonic"); D.CompanionExample = TEXT("None"); Doctors.Add(D);
	D.Number = 9; D.Actor = TEXT("Christopher Eccleston"); D.Era = TEXT("2005"); D.Catchphrase = TEXT("Fantastic!"); D.ScrewdriverStyle = TEXT("Slender blue"); D.CompanionExample = TEXT("Rose Tyler"); Doctors.Add(D);
	D.Number = 10; D.Actor = TEXT("David Tennant"); D.Era = TEXT("2005-2010, 2023"); D.Catchphrase = TEXT("Allons-y!"); D.ScrewdriverStyle = TEXT("Green-blue Mark III"); D.CompanionExample = TEXT("Rose, Martha, Donna"); Doctors.Add(D);
	D.Number = 11; D.Actor = TEXT("Matt Smith"); D.Era = TEXT("2010-2013"); D.Catchphrase = TEXT("Geronimo!"); D.ScrewdriverStyle = TEXT("Green Mark IV"); D.CompanionExample = TEXT("Amy, Rory, Clara, River"); Doctors.Add(D);
	D.Number = 12; D.Actor = TEXT("Peter Capaldi"); D.Era = TEXT("2013-2017"); D.Catchphrase = TEXT("Shut up. I am being extremely clever."); D.ScrewdriverStyle = TEXT("Blue glowing"); D.CompanionExample = TEXT("Clara, Bill, Nardole"); Doctors.Add(D);
	D.Number = 13; D.Actor = TEXT("Jodie Whittaker"); D.Era = TEXT("2017-2022"); D.Catchphrase = TEXT("Oh, brilliant!"); D.ScrewdriverStyle = TEXT("Crystal amber"); D.CompanionExample = TEXT("Yaz, Ryan, Graham, Dan"); Doctors.Add(D);
	D.Number = 14; D.Actor = TEXT("David Tennant"); D.Era = TEXT("2023"); D.Catchphrase = TEXT("I don't know who I am anymore."); D.ScrewdriverStyle = TEXT("New green sonic"); D.CompanionExample = TEXT("Donna Noble"); Doctors.Add(D);
	D.Number = 15; D.Actor = TEXT("Ncuti Gatwa"); D.Era = TEXT("2023-"); D.Catchphrase = TEXT("There's no such thing as an ordinary day."); D.ScrewdriverStyle = TEXT("Ruby-red sonic"); D.CompanionExample = TEXT("Ruby Sunday"); Doctors.Add(D);

	// Sonic screwdrivers.
	FSonicDef S;
	S.Name = TEXT("Mark I Sonic"); S.Description = TEXT("The original. Unlocks doors, scans energy signatures, confused by deadlock seals."); S.PowerLevel = 4; Sonics.Add(S);
	S.Name = TEXT("Mark VII Sonic"); S.Description = TEXT("River Song's. Red setting. Extra neural relay. Can disable weapons."); S.PowerLevel = 7; Sonics.Add(S);
	S.Name = TEXT("Sonic Sunglasses"); S.Description = TEXT("12's invention. Scans, hacks, looks cool. Can't open wooden doors."); S.PowerLevel = 5; Sonics.Add(S);
	S.Name = TEXT("Sonic Trowel"); S.Description = TEXT("For archaeological investigations. Also opens doors. River Song approved."); S.PowerLevel = 3; Sonics.Add(S);
	S.Name = TEXT("Ruby-red Sonic"); S.Description = TEXT("15's custom. Programmable, organic circuitry, glows when danger is near."); S.PowerLevel = 8; Sonics.Add(S);
	S.Name = TEXT("Custom CHIMERA Sonic"); S.Description = TEXT("You built this. Hybrid of Gallifreyan tech and ring engineering."); S.PowerLevel = 9; Sonics.Add(S);
}

void UTARDISSubsystem::SetFavouriteDoctor(int32 Num)
{
	FavouriteDoctor = Num;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
	{
		for (const FDoctorDef& D : Doctors)
			if (D.Number == Num) { Sess->ShowMessage(FString::Printf(TEXT("The %d%s Doctor (%s) is your favourite. %s"), Num, Num == 1 ? TEXT("st") : Num == 2 ? TEXT("nd") : TEXT("th"), *D.Actor, *D.Catchphrase)); break; }
	}
}

void UTARDISSubsystem::SetSonic(int32 Index)
{
	if (Index >= 0 && Index < Sonics.Num()) SonicIndex = Index;
}

FString UTARDISSubsystem::ExploreRoom()
{
	static const TCHAR* Rooms[] = {
		TEXT("The Library — infinite shelves. You find a book titled 'The History of the CHIMERA Rings' written in Gallifreyan."),
		TEXT("The Swimming Pool — it's in the library. The Doctor was redecorating. You don't ask."),
		TEXT("The Wardrobe — everything from Victorian gowns to spacesuits. You find a fez. Fezzes are cool."),
		TEXT("The Zero Room — total silence. Time doesn't pass here. Perfect for healing."),
		TEXT("The Cloister Room — the Cloister Bell tolls softly. Something is wrong somewhere in the rings."),
		TEXT("The Engine Room — the Eye of Harmony pulses. The heart of the TARDIS. You feel it look back at you."),
		TEXT("The Console Room (old) — a white roundelled wall, the classic console. Grumpy sensors."),
		TEXT("A corridor that shouldn't exist. It leads to a door marked 'Rassilon.' You choose not to open it."),
	};
	return Rooms[FMath::RandRange(0, 7)];
}

FString UTARDISSubsystem::ScanWithSonic()
{
	const FSonicDef* Sonic = GetActiveSonic();
	if (!Sonic) return TEXT("No sonic screwdriver equipped. Your pockets feel empty.");
	int32 Power = Sonic->PowerLevel;
	if (bPoliceMode) Power += 3;
	if (FMath::RandRange(0, Power + 5) >= 5)
		return FString::Printf(TEXT("%s buzzes: 'Life signs detected. Temporal anomaly at the nearest ring gate.'"), *Sonic->Name);
	return FString::Printf(TEXT("%s hums but finds nothing unusual. At least, nothing yet."), *Sonic->Name);
}

void UTARDISSubsystem::ActivatePoliceMode()
{
	bPoliceMode = true; TARDISExterior = TEXT("Metropolitan Police Box — blue beacon active, siren ready");
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>())
		Sess->ShowMessage(TEXT("Police lights flash atop the TARDIS. The blue box is now an official patrol vehicle. Mid-Wilshire PD won't look twice."));
}

void UTARDISSubsystem::DeactivatePoliceMode()
{
	bPoliceMode = false; TARDISExterior = TEXT("Classic 1963 Police Box");
}

void UTARDISSubsystem::AddCompanion(const FString& Name) { if (!Companions.Contains(Name)) Companions.Add(Name); }
