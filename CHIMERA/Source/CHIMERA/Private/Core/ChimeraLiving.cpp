#include "Core/ChimeraLiving.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraEconomy.h"
#include "Core/ChimeraRecords.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

// ====================================================================== 1. SLEEP ======================================================================
void USleepSystem::Sleep(int32 Hours)
{
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return;
	Sess->HourOfDay = FMath::Fmod(Sess->HourOfDay + Hours, 24.f);
	if (Sess->HourOfDay < Hours) Sess->GameDay++;
	Sess->ShowMessage(FString::Printf(TEXT("Sleeping for %d hours... %s"), Hours, *GetTimeOfDay()));
	// Heal player, recover fatigue
	if (auto* Ars = GetGameInstance()->GetSubsystem<UArsenalSubsystem>()) {} // future: heal
}

bool USleepSystem::CanSleep() const
{
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	// Can sleep if at a base (owned property) or hotel (any ring)
	return !Sess->OwnedProperties.IsEmpty() || true; // hotels everywhere
}

FString USleepSystem::GetTimeOfDay() const
{
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return TEXT("Unknown");
	float H = Sess->HourOfDay;
	if (H < 6) return TEXT("Night — stars over Nexopolis");
	if (H < 12) return TEXT("Morning — the rings wake");
	if (H < 18) return TEXT("Afternoon — peak activity across the supercontinent");
	if (H < 21) return TEXT("Evening — sunset over the Convergence");
	return TEXT("Night — the city never truly sleeps");
}

// ====================================================================== 2. FAST TRAVEL ======================================================================
void UFastTravel::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C);
	auto R = [&](const TCHAR* F, const TCHAR* T, int32 Cost, float Min) { Routes.Add({F, T, Cost, Min}); };
	R(TEXT("Nexus"),TEXT("Aether City"),200,2); R(TEXT("Nexus"),TEXT("Mid-Wilshire"),200,3);
	R(TEXT("Nexus"),TEXT("Alagaesia"),500,5); R(TEXT("Nexus"),TEXT("Seattle"),300,4);
	R(TEXT("Nexus"),TEXT("Panem"),600,6); R(TEXT("Nexus"),TEXT("Night City"),400,4);
	R(TEXT("Nexus"),TEXT("Cleveleys"),300,3); R(TEXT("Nexus"),TEXT("VernetLesBains"),500,5);
	R(TEXT("Aether City"),TEXT("Grand Coliseum"),150,1); R(TEXT("Mid-Wilshire"),TEXT("Mercy Heights"),100,1);
	R(TEXT("Mid-Wilshire"),TEXT("Chicago"),250,2); R(TEXT("Seattle"),TEXT("ChastainPark"),100,1);
	R(TEXT("Night City"),TEXT("Panem"),350,3); R(TEXT("Alagaesia"),TEXT("Minecraft Frontier"),400,4);
}
bool UFastTravel::Travel(const FString& Destination, ETravelType Type)
{
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	for (auto& R : Routes) {
		if ((R.From == StaticEnum<ERing>()->GetNameStringByValue((int64)Sess->CurrentRing) && R.To == Destination) ||
			(R.To == StaticEnum<ERing>()->GetNameStringByValue((int64)Sess->CurrentRing) && R.From == Destination))
		{
			int32 Cost = Type == ETravelType::Air ? R.Cost * 3 : Type == ETravelType::Taxi ? R.Cost * 2 : R.Cost;
			if (Sess->GetCurrency(ECurrency::Credits) < Cost) return false;
			Sess->AddCurrency(ECurrency::Credits, -Cost);
			ERing DestRing = (ERing)StaticEnum<ERing>()->GetValueByName(FName(*Destination));
			Sess->CurrentRing = DestRing;
			Sess->HourOfDay += R.Minutes / 60.f;
			Sess->ShowMessage(FString::Printf(TEXT("Fast travel to %s — %s. %.0f minutes."), *Destination, Type == ETravelType::Train ? TEXT("Train") : Type == ETravelType::Taxi ? TEXT("Taxi") : TEXT("Air"), R.Minutes));
			return true;
		}
	}
	return false;
}
FString UFastTravel::GetTrainStation(const FString& Ring) { return Ring + TEXT(" Central Station"); }
FString UFastTravel::GetAirport(const FString& Ring) { return Ring + TEXT(" International Airport"); }

// ====================================================================== 3. MINIMAP ======================================================================
TArray<UMinimapSystem::FMinimapBlip> UMinimapSystem::GetBlips() const { return CachedBlips; }
void UMinimapSystem::UpdateBlips(const FString& CurrentRing)
{
	CachedBlips.Reset();
	// Simulate blips: nearby interactables, mission objectives
	CachedBlips.Add({15.f, 80.f, FLinearColor::Yellow, TEXT("Mission")});
	CachedBlips.Add({-40.f, 120.f, FLinearColor::Green, TEXT("Shop")});
	CachedBlips.Add({90.f, 60.f, FLinearColor::Red, TEXT("Wanted")});
}
bool UMinimapSystem::IsNearCollectible() const
{
	auto* Coll = GetGameInstance()->GetSubsystem<UCollectibleSystem>();
	if (!Coll) return false;
	return FMath::RandBool(); // simulated proximity
}

// ====================================================================== 4. BAR / NIGHTLIFE ======================================================================
void UBarSubsystem::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C);
	Menu = {
		{TEXT("Fylde Coast Ale"), EDrinkType::Beer, 5, 8.f, TEXT("Local Lancashire brew. Slightly salty. Pairs with fish and chips.")},
		{TEXT("Vernet-les-Bains Rosé"), EDrinkType::Wine, 12, 12.f, TEXT("From the Pyrenees foothills. Dry. Sun-warmed. Kipling approved.")},
		{TEXT("Dauntless Shot"), EDrinkType::Shot, 8, 25.f, TEXT("Pure adrenaline. Dauntless initiation rite. Burns going down. Burns coming up.")},
		{TEXT("Nexus Nebula"), EDrinkType::Cocktail, 15, 18.f, TEXT("Blue and glowing. Contains a dissolved Reality Anchor fragment. Slightly illegal.")},
		{TEXT("Sparkling Water"), EDrinkType::Water, 0, 0.f, TEXT("From the thermal springs of Vernet. Purer than anything. Sober up instantly.")},
	};
}
bool UBarSubsystem::BuyDrink(const FString& Name)
{
	for (auto& D : Menu) {
		if (D.Name == Name) {
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (!Sess || Sess->GetCurrency(ECurrency::Credits) < D.Cost) return false;
			Sess->AddCurrency(ECurrency::Credits, -D.Cost);
			Drunkenness = FMath::Min(100, Drunkenness + (int32)D.Strength + FMath::RandRange(-3,5));
			if (D.Type == EDrinkType::Water) Drunkenness = 0;
			Sess->ShowMessage(FString::Printf(TEXT("Drinking: %s. %s Drunkenness: %d%%"), *D.Name, *D.Description, Drunkenness));
			if (Drunkenness > 80) Sess->ShowMessage(TEXT("The room is spinning. You should probably sit down."));
			return true;
		}
	}
	return false;
}
void UBarSubsystem::SoberUp(float Amount) { Drunkenness = FMath::Max(0, Drunkenness - (int32)Amount); }
FString UBarSubsystem::BarFight()
{
	bInBarFight = true;
	int32 Skill = 0;
	if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>()) Skill = Sess->GetSkillLevel(FName("Combat_Brawling"));
	bool bWon = FMath::RandRange(0, Skill + Drunkenness/2) >= 40;
	bInBarFight = false;
	if (bWon) {
		if (auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>()) { Sess->AddFame(2); Sess->WantedLevel = FMath::Min(2, Sess->WantedLevel + 1); }
		return TEXT("BAR FIGHT: You win. The other guy is on the floor. The bartender sighs. 'Not again.'");
	}
	return TEXT("BAR FIGHT: You wake up outside. Your wallet is gone. Your dignity is bruised. The seagulls are laughing.");
}
FString UBarSubsystem::KaraokeSong()
{
	static const TCHAR* Songs[] = { TEXT("'Bohemian Rhapsody' — the entire bar joins in. Even the seagulls harmonise."),
		TEXT("'Wonderwall' — the bartender cries. 'Every time,' they whisper."),
		TEXT("'Sweet Caroline' — BA BA BAAAA. The Venue erupts. Dougie does the actions."),
		TEXT("'My Heart Will Go On' — you hold the note for 12 seconds. Ronnie the Fisherman gives you a standing ovation.") };
	return Songs[FMath::RandRange(0, 3)];
}

// ====================================================================== 5. STOCK TRADING ======================================================================
void UStockTrading::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C); }
TArray<UStockTrading::FStockQuote> UStockTrading::GetQuotes() const
{
	TArray<FStockQuote> Q;
	if (auto* BQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>()) {
		for (auto& S : BQ->GetStocks()) {
			float Change = FMath::FRandRange(-5, 5);
			FString Tip = FMath::RandBool() ? TEXT("Buy — positive earnings report from ring trade data") : TEXT("Hold — market volatility expected after the Convergence cycle");
			Q.Add({S.Symbol, S.Name, S.Price, Change, Tip});
		}
	}
	return Q;
}
bool UStockTrading::BuyStock(const FString& Symbol, int32 Shares)
{
	auto* BQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>();
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!BQ || !Sess) return false;
	for (auto& S : BQ->GetStocks()) {
		if (S.Symbol == Symbol) {
			int32 Cost = FMath::RoundToInt(S.Price * Shares);
			if (Sess->GetCurrency(ECurrency::Credits) < Cost) return false;
			Sess->AddCurrency(ECurrency::Credits, -Cost);
			Portfolio.FindOrAdd(Symbol) += Shares;
			Sess->ShowMessage(FString::Printf(TEXT("BUY: %d %s @ %.0f = %d credits"), Shares, *Symbol, S.Price, Cost));
			return true;
		}
	}
	return false;
}
bool UStockTrading::SellStock(const FString& Symbol, int32 Shares)
{
	auto* BQ = GetGameInstance()->GetSubsystem<UBAWSAQSubsystem>();
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!BQ || !Sess || Portfolio.FindRef(Symbol) < Shares) return false;
	for (auto& S : BQ->GetStocks()) {
		if (S.Symbol == Symbol) {
			int32 Value = FMath::RoundToInt(S.Price * Shares);
			Sess->AddCurrency(ECurrency::Credits, Value);
			Portfolio[Symbol] -= Shares;
			Sess->ShowMessage(FString::Printf(TEXT("SELL: %d %s @ %.0f = +%d credits"), Shares, *Symbol, S.Price, Value));
			return true;
		}
	}
	return false;
}
int32 UStockTrading::GetPortfolioValue() const { int32 V = 0; return V; }
TArray<FString> UStockTrading::GetTips() const { return { TEXT("Buy low, sell high."), TEXT("Dragon scale imports are up. Alagaesia mining stocks."), TEXT("Aether City power grid stocks volatile — kaiju attacks affect supply.") }; }

// ====================================================================== 6. UNDERWATER ======================================================================
void UUnderwaterSystem::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C);
	Sites = {
		{TEXT("Sunken Ferry"), TEXT("Cleveleys"), 30, TEXT("The Riverdance ferry, beached in 2008. Now fully submerged after the Crash."), TEXT("Ferry Manifest + 500 credits")},
		{TEXT("Coral Cathedral"), TEXT("Aether City"), 80, TEXT("Bioluminescent coral grown over a submerged temple. Glows at night."), TEXT("Glowing Coral Sample (alchemy ingredient)")},
		{TEXT("Drowned District Ruins"), TEXT("Night City"), 200, TEXT("The fragment of a water-world reality. Streets. Houses. All underwater."), TEXT("Water-World Tech Fragment (rare)")},
		{TEXT("Shipwreck Cove"), TEXT("Panem"), 50, TEXT("District 4 fishing boats, sunk during a Capitol raid. Treasure still aboard."), TEXT("District 4 Gold Doubloons")},
		{TEXT("Abyssal Trench"), TEXT("Seattle"), 500, TEXT("The deepest point near Nexopolis. No light. Something moves down there."), TEXT("Abyssal Pearl (legendary)")},
	};
}
bool UUnderwaterSystem::BuyGear(EDivingGear Gear)
{
	int32 Costs[] = {0, 500, 5000, 50000};
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Costs[(int32)Gear]) return false;
	Sess->AddCurrency(ECurrency::Credits, -Costs[(int32)Gear]);
	CurrentGear = Gear;
	Sess->ShowMessage(FString::Printf(TEXT("Diving gear: %s. Max depth: %dm"), Gear == EDivingGear::Submarine ? TEXT("Submarine") : Gear == EDivingGear::Scuba ? TEXT("Scuba") : TEXT("Snorkel"), GetMaxDepth()));
	return true;
}
int32 UUnderwaterSystem::GetMaxDepth() const
{ switch (CurrentGear) { case EDivingGear::Submarine: return 1000; case EDivingGear::Scuba: return 200; case EDivingGear::Snorkel: return 10; default: return 0; } }
FString UUnderwaterSystem::ExploreSite(const FString& SiteName)
{
	for (auto& S : Sites) {
		if (S.Name == SiteName) {
			if (CurrentGear == EDivingGear::None || GetMaxDepth() < S.Depth) return FString::Printf(TEXT("Too deep! Need better diving gear. Max depth: %dm. Site depth: %dm."), GetMaxDepth(), S.Depth);
			auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
			if (Sess) { Sess->AddCurrency(ECurrency::Credits, 500); Sess->ShowMessage(FString::Printf(TEXT("Underwater: %s. Found: %s"), *S.Discovery, *S.Treasure)); }
			return S.Discovery + TEXT(" — ") + S.Treasure;
		}
	}
	return TEXT("Site not found.");
}

// ====================================================================== 7. EMERGENCY JOBS ======================================================================
void UEmergencyJobs::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C); }
void UEmergencyJobs::StartJob(EEmergencyJob Job)
{
	CurrentJob = Job; GenerateCall();
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) Sess->ShowMessage(FString::Printf(TEXT("%s CALL: %s at %s. %ds to respond."), Job == EEmergencyJob::Paramedic ? TEXT("AMBULANCE") : TEXT("FIRE"), *CurrentCall.Victim, *CurrentCall.Location, CurrentCall.TimeLimit));
}
void UEmergencyJobs::GenerateCall()
{
	FEmergencyCall C;
	C.Location = TEXT("Mercy Heights ER");
	C.Distance = FMath::RandRange(1, 10);
	C.TimeLimit = C.Distance * 15 + FMath::RandRange(0, 30);
	C.Victim = FMath::RandBool() ? TEXT("civilian with chest pain") : TEXT("car accident victim");
	C.Reward = FString::Printf(TEXT("+%d credits +Medical XP"), C.Distance * 200);
	CurrentCall = C;
}
FEmergencyCall UEmergencyJobs::GetCall() const { return CurrentCall; }
bool UEmergencyJobs::CompleteCall()
{
	JobsCompleted++;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) { Sess->AddCurrency(ECurrency::Credits, CurrentCall.Distance * 200); Sess->AddSkillXP(FName("Med_Surgery"), 15); Sess->ShowMessage(FString::Printf(TEXT("Call complete! %d jobs done. %s"), JobsCompleted, *GetRewardUnlock())); }
	GenerateCall();
	return true;
}
FString UEmergencyJobs::GetRewardUnlock() const
{
	if (JobsCompleted >= 50) return TEXT("UNLOCKED: Custom Ambulance vehicle.");
	if (JobsCompleted >= 25) return TEXT("UNLOCKED: Paramedic outfit (+Medical XP)");
	if (JobsCompleted >= 10) return TEXT("UNLOCKED: EMT badge. Free medical care at all hospitals.");
	return TEXT("Next reward: 10 jobs.");
}

// ====================================================================== 8. WILDLIFE ======================================================================
void UWildlifeSystem::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C);
	Animals = {
		{TEXT("Fylde Fox"), EAnimalType::Deer, TEXT("Cleveleys"), TEXT("Dunes"), false, TEXT("Venison + hide")},
		{TEXT("Mountain Boar"), EAnimalType::Boar, TEXT("Alagaesia"), TEXT("Forests"), true, TEXT("Boar meat + tusks")},
		{TEXT("Shadow Wolf"), EAnimalType::Wolf, TEXT("Alagaesia"), TEXT("Mountains"), true, TEXT("Wolf pelt + fangs")},
		{TEXT("Grizzly"), EAnimalType::Bear, TEXT("VernetLesBains"), TEXT("Pyrenees"), true, TEXT("Bear hide + claws")},
		{TEXT("Tracker Jacker"), EAnimalType::Mutt, TEXT("Panem"), TEXT("Arena"), true, TEXT("Tracker jacker venom (serum ingredient)")},
		{TEXT("Cyber-Crow"), EAnimalType::Bird, TEXT("Night City"), TEXT("Rooftops"), false, TEXT("Cyber-feathers + data chip")},
		{TEXT("Voxel Rabbit"), EAnimalType::Rabbit, TEXT("Minecraft Frontier"), TEXT("Plains"), false, TEXT("Raw meat + hide")},
	};
	LegendaryFish = {
		{TEXT("The Fylde Leviathan"), TEXT("Cleveleys"), TEXT("Rossall Beach at high tide, moonlight"), TEXT("Mackerel strip"), 28.5f},
		{TEXT("Canigou Golden Trout"), TEXT("VernetLesBains"), TEXT("River Cady, under Kipling Bridge"), TEXT("Cheese (Philippe's Roquefort)"), 4.2f},
		{TEXT("District 4 Deep Marlin"), TEXT("Panem"), TEXT("Off the coast, 200m+ depth"), TEXT("Squid"), 180.f},
	};
}
FString UWildlifeSystem::Hunt(const FString& Ring)
{
	TArray<FAnimalDef> Local; for (auto& A : Animals) if (A.Ring == Ring) Local.Add(A);
	if (Local.Num() == 0) return TEXT("No wildlife here. Try the wilderness between rings.");
	auto& A = Local[FMath::RandRange(0, Local.Num() - 1)];
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	int32 Skill = Sess ? Sess->GetSkillLevel(FName("Combat_Blades")) + Sess->GetSkillLevel(FName("Tech_Survival")) : 10;
	bool bSuccess = FMath::RandRange(0, Skill + 20) >= (A.bHostile ? 30 : 15);
	if (bSuccess) {
		if (Sess) { Sess->AddItem(FName("RawMeat"), 2); Sess->AddSkillXP(FName("Tech_Survival"), 20); }
		return FString::Printf(TEXT("Hunting: %s taken down. %s collected."), *A.Name, *A.Drop);
	}
	return FString::Printf(TEXT("Hunting: %s escaped. It's %s in the %s."), *A.Name, A.bHostile ? TEXT("circling back — RUN") : TEXT("gone"), *A.Habitat);
}
FString UWildlifeSystem::Fish(const FString& Ring, bool bLegendaryAttempt)
{
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (bLegendaryAttempt) {
		for (auto& L : LegendaryFish) {
			if (L.Ring == Ring && Sess && FMath::RandRange(0, 200) == 0) {
				LegendaryCaught++;
				Sess->ShowMessage(FString::Printf(TEXT("LEGENDARY FISH: %s — %.1fkg! This will be remembered."), *L.Name, L.WeightKg));
				Sess->RecordEvent(FString::Printf(TEXT("Caught legendary fish: %s"), *L.Name));
				return FString::Printf(TEXT("LEGENDARY: %s (%.1fkg) at %s using %s. %d/%d legendary fish."), *L.Name, L.WeightKg, *L.Location, *L.Bait, LegendaryCaught, LegendaryFish.Num());
			}
		}
	}
	if (Sess) { Sess->AddItem(FName("RawMeat"), FMath::RandRange(0, 3)); Sess->AddSkillXP(FName("Tech_Farming"), 10); }
	return TEXT("Caught: mackerel, cod, and one very angry crab.");
}
