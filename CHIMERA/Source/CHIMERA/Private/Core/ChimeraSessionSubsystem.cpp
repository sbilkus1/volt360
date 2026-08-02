#include "Core/ChimeraSessionSubsystem.h"
#include "Core/ChimeraRecords.h"
#include "CHIMERA.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "UObject/Enum.h"

void UChimeraSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedDefaults();
}

void UChimeraSessionSubsystem::SeedDefaults()
{
	Skills.Reset();
	for (const FSkillDef& Def : FChimeraData::Skills())
	{
		FSkillState S;
		S.Id = Def.Id;
		Skills.Add(S);
	}
	Attributes.SetNum((int32)EAttribute::COUNT);
	for (int32 i = 0; i < (int32)EAttribute::COUNT; ++i) Attributes[i] = 5;
	Currencies.SetNum((int32)ECurrency::COUNT);
	Currencies[(int32)ECurrency::Credits] = 1000;
	Currencies[(int32)ECurrency::Emeralds] = 16;
	Currencies[(int32)ECurrency::Crowns] = 50;
	Currencies[(int32)ECurrency::Eddies] = 500;
	FactionRep.SetNum((int32)EFaction::COUNT);
	for (int32 i = 0; i < (int32)EFaction::COUNT; ++i) FactionRep[i] = 0;
	FactionTerritory.SetNum((int32)EFaction::COUNT);
	for (int32 i = 0; i < (int32)EFaction::COUNT; ++i) FactionTerritory[i] = 8;
}

void UChimeraSessionSubsystem::AddPlayTime(float Seconds)
{
	HoursPlayed += Seconds / 3600.f;
	TickClock(Seconds);
}

// ---------------------------------------------------------------------------
// GDD 1.3 - Day/Night + Weather clock.
// ---------------------------------------------------------------------------
// Prototype pacing: 1 real second = 1 in-game minute. A full 24h day = 24 real minutes.
void UChimeraSessionSubsystem::TickClock(float DeltaSeconds)
{
	float OldHour = HourOfDay;
	HourOfDay += DeltaSeconds / 60.f;
	if (HourOfDay >= 24.f)
	{
		HourOfDay = FMath::Fmod(HourOfDay, 24.f);
		GameDay++;
		// GDD 1.3 - seasons last 14 in-game days.
		int32 SeasonIdx = (GameDay / 14) % 4;
		Season = (SeasonIdx == 0) ? TEXT("Spring") : (SeasonIdx == 1) ? TEXT("Summer") : (SeasonIdx == 2) ? TEXT("Autumn") : TEXT("Winter");
		// GDD 14.11 - seasonal festivals fire on day 7 of each season.
		bFestivalActive = (GameDay % 14 == 7);
		FestivalXPBonus = bFestivalActive ? 1.5f : 1.f;
		if (bFestivalActive)
		{
			switch (SeasonIdx)
			{
			case 0: ActiveFestival = TEXT("Spring Carnival"); break;
			case 1: ActiveFestival = TEXT("The Grand Tournament"); break;
			case 2: ActiveFestival = TEXT("Hallowed Nights"); break;
			case 3: ActiveFestival = TEXT("Frostfall Festival"); break;
			}
			RecordEvent(FString::Printf(TEXT("%s begins! Double XP across all rings today."), *ActiveFestival));
			ShowMessage(FString::Printf(TEXT("%s is here! All skill XP boosted 1.5x."), *ActiveFestival));
		}
		GenerateNews();
	}
	if ((int32)OldHour != (int32)HourOfDay)
	{
		TickHourly();
	}
}

void UChimeraSessionSubsystem::TickHourly()
{
	// GDD 9.3/9.4 - passive income + insurance premium on the hour.
	if (GetPassiveIncomePerHour() > 0)
	{
		AddCurrency(ECurrency::Credits, GetPassiveIncomePerHour());
		RecordEvent(FString::Printf(TEXT("Property income: +%d credits (hourly)"), GetPassiveIncomePerHour()));
	}
	if (GetInsurancePremiumPerHour() > 0) PayInsurancePremium();
	// Weather changes a few times per day (GDD 1.3).
	if (FMath::RandRange(0, 5) == 0) GenerateWeather();
	// GDD 14.8 - fame/infamy decay without reinforcement.
	if (FMath::Abs(Fame) > 0) { Fame += (Fame > 0) ? -1 : 1; }
	if (FMath::Abs(Infamy) > 0) { Infamy += (Infamy > 0) ? -1 : 1; }
	// GDD 14.3/6.9 - pets and dragons grow bonds by spending time together.
	if (HasPet()) BondWithPet(1);
	if (bHasDragon) BondWithDragon(1);
}

void UChimeraSessionSubsystem::GenerateWeather()
{
	const TArray<FString> Conditions = { TEXT("Clear"), TEXT("Cloudy"), TEXT("Rain"), TEXT("Neon Rain"),
		TEXT("Magical Storm"), TEXT("Fog"), TEXT("Snow"), TEXT("Ashfall") };
	Weather = Conditions[FMath::RandRange(0, Conditions.Num() - 1)];
	RecordEvent(FString::Printf(TEXT("Weather shifts: %s across the rings."), *Weather));
}

void UChimeraSessionSubsystem::GenerateNews()
{
	TArray<FString> Pool = {
		TEXT("Nexopolis City News: hyperloop ridership at an all-time high."),
		TEXT("Mid-Wilshire PD unveils new predictive-policing unit."),
		TEXT("Mercy Heights announces 'Mystery Illness Week' - volunteers needed."),
		TEXT("Grand Coliseum to host cross-sport exhibition: F1 drivers vs footballers."),
		TEXT("Aether City reports a sudden surge in drag-race tourism."),
		TEXT("Alagaesia: dragon sightings confirmed near the Keeper's tower."),
		TEXT("Frontier miners strike voxel-gold vein; market reacts."),
		TEXT("Neo-Kingdom potion guild warns of counterfeit healing draughts."),
		TEXT("Night City: Blackwall anomaly detected; netrunners advised to patch."),
		TEXT("The Keepers remind citizens: the Crash is not a joke.")
	};
	FString Headline = Pool[FMath::RandRange(0, Pool.Num() - 1)];
	News.Insert(Headline, 0);
	if (News.Num() > 8) News.Pop();
	RecordEvent(FString::Printf(TEXT("[News] %s"), *Headline));
}

// GDD 14.19 - NPC relationships.
void UChimeraSessionSubsystem::AddRelationship(FString NpcName, int32 Amt)
{
	int32& V = Relationships.FindOrAdd(NpcName);
	V = FMath::Clamp(V + Amt, -100, 100);
}

// ---------------------------------------------------------------------------
// GDD 14.3 - Companion & Pet system.
// ---------------------------------------------------------------------------
bool UChimeraSessionSubsystem::AdoptPet(const FString& Type, const FString& Name)
{
	if (HasPet()) return false;
	PetType = Type;
	PetName = Name;
	PetBond = 10;
	PetLevel = 1;
	AddStat(TEXT("pet_adopted"), 1);
	RecordEvent(FString::Printf(TEXT("You adopt %s, a %s. Bond begins at %d."), *Name, *Type, PetBond));
	return true;
}

void UChimeraSessionSubsystem::BondWithPet(int32 Amt)
{
	if (!HasPet()) return;
	PetBond = FMath::Clamp(PetBond + Amt, 0, 100);
	if (PetBond % 25 == 0)
	{
		PetLevel = 1 + PetBond / 25;
		RecordEvent(FString::Printf(TEXT("%s reaches bond level %d."), *PetName, PetLevel));
	}
}

void UChimeraSessionSubsystem::TrainPet(int32 Amt)
{
	if (!HasPet()) return;
	PetLevel = FMath::Min(PetLevel + 1, 20);
	BondWithPet(Amt);
	RecordEvent(FString::Printf(TEXT("%s trained. Pet level %d."), *PetName, PetLevel));
}

// ---------------------------------------------------------------------------
// GDD 6.9 - Dragon system.
// ---------------------------------------------------------------------------
bool UChimeraSessionSubsystem::BondDragon()
{
	if (!bHasDragon) return false;
	DragonBond = FMath::Max(DragonBond, 10);
	RecordEvent(FString::Printf(TEXT("Dragon %s bonds with you. Rider's Bond established."), *DragonName));
	return true;
}

void UChimeraSessionSubsystem::BondWithDragon(int32 Amt)
{
	if (!bHasDragon) return;
	DragonBond = FMath::Clamp(DragonBond + Amt, 0, 100);
}

void UChimeraSessionSubsystem::FeedDragon(int32 Meat)
{
	if (!bHasDragon) return;
	if (!HasItem(FName("RawMeat")) || !RemoveItem(FName("RawMeat"), Meat)) return;
	BondWithDragon(10 * Meat);
	RecordEvent(FString::Printf(TEXT("You feed %s meat. The bond strengthens (+%d)."), *DragonName, 10 * Meat));
}

// --- skills ---
FSkillState* UChimeraSessionSubsystem::GetSkill(FName Id)
{
	for (FSkillState& S : Skills)
	{
		if (S.Id == Id) return &S;
	}
	return nullptr;
}

int32 UChimeraSessionSubsystem::GetSkillLevel(FName Id)
{
	FSkillState* S = GetSkill(Id);
	return S ? S->Level : 1;
}

void UChimeraSessionSubsystem::AddSkillXP(FName Id, int32 XP)
{
	FSkillState* S = GetSkill(Id);
	if (!S) return;
	int32 Bonus = XP * FestivalXPBonus;
	S->XP += FMath::Max(XP, FMath::RoundToInt(Bonus));
	// GDD 5.3 - no level cap; level when XP threshold crossed.
	int32 Need = S->Level * 100;
	while (S->XP >= Need)
	{
		S->XP -= Need;
		S->Level++;
		S->Perks++;
		Need = S->Level * 100;
		RecordEvent(FString::Printf(TEXT("Skill leveled: %s -> %d"), *Id.ToString(), S->Level));
	}
}

void UChimeraSessionSubsystem::SpendPerk(FName Id)
{
	FSkillState* S = GetSkill(Id);
	if (S && S->Perks > 0) S->Perks--;
}

void UChimeraSessionSubsystem::LegendaryReset(FName Id)
{
	FSkillState* S = GetSkill(Id);
	if (S && S->Level >= 100)
	{
		S->LegendaryStars++;
		S->Level = 15;
		S->XP = 0;
		RecordEvent(FString::Printf(TEXT("LEGENDARY: %s is now Legendary %d"), *Id.ToString(), S->LegendaryStars));
		if (auto* Rec = GetGameInstance()->GetSubsystem<UHallOfRecords>()) Rec->Tally(TEXT("skill_100"), 1);
	}
}

// --- economy / rep ---
void UChimeraSessionSubsystem::AddCurrency(ECurrency C, int32 Amt)
{
	if ((int32)C < Currencies.Num())
	{
		Currencies[(int32)C] += Amt;
		if (Currencies[(int32)C] < 0) Currencies[(int32)C] = 0;
	}
	if (C == ECurrency::Credits && Amt > 0)
	{
		if (auto* Rec = GetGameInstance()->GetSubsystem<UHallOfRecords>()) Rec->Tally(TEXT("credits_earned"), Amt);
	}
}

void UChimeraSessionSubsystem::AddRep(EFaction F, int32 Amt)
{
	if ((int32)F >= FactionRep.Num()) return;
	FactionRep[(int32)F] = FMath::Clamp(FactionRep[(int32)F] + Amt, -100, 100);
}

void UChimeraSessionSubsystem::RecordEvent(const FString& What)
{
	EventLog.Insert(What, 0);
	if (EventLog.Num() > 64) EventLog.Pop();
	UE_LOG(LogChimera, Log, TEXT("[Chronicle] %s"), *What);
}

void UChimeraSessionSubsystem::AddStat(FName Key, int32 Amt)
{
	int32& V = Stats.FindOrAdd(Key);
	V += Amt;
	if (auto* Rec = GetGameInstance()->GetSubsystem<UHallOfRecords>()) Rec->Tally(Key, Amt);
}

// --- quests ---
void UChimeraSessionSubsystem::SetActiveQuest(const FQuestDef& Q)
{
	ActiveQuest = Q;
	CurrentObjective = Q.Objectives.Num() > 0 ? Q.Objectives[0].Text : FString();
	RecordEvent(FString::Printf(TEXT("Quest started: %s"), *Q.Title));
}

void UChimeraSessionSubsystem::CompleteActiveQuest()
{
	AddCurrency(ECurrency::Credits, ActiveQuest.RewardCredits);
	CompletedQuests.AddUnique(ActiveQuest.Id);
	RecordEvent(FString::Printf(TEXT("Quest completed: %s (+%d credits, rating %d)"),
		*ActiveQuest.Title, ActiveQuest.RewardCredits, ActiveQuest.Rating));
	ActiveQuest = FQuestDef();
	CurrentObjective.Empty();
}

// GDD 3.6 - Legacy System.
void UChimeraSessionSubsystem::RetireCharacter()
{
	if (!bCharacterCreated) return;
	RecordEvent(TEXT("Your character retired and became a mentor NPC in other worlds."));
	Generations++;
	// Heir inherits 20% of skill levels.
	for (FSkillState& S : Skills)
	{
		S.Level = FMath::Clamp(S.Level / 5, 1, 100);
		S.XP = 0;
		S.Perks = 0;
	}
	// Positive reputation carries; negative fades (GDD 3.6).
	for (int32 i = 0; i < FactionRep.Num(); ++i)
	{
		FactionRep[i] = FMath::Clamp(FactionRep[i] / 2, -10, 100);
	}
	Currencies[(int32)ECurrency::Credits] = 200;
	Currencies[(int32)ECurrency::Emeralds] = 4;
	Currencies[(int32)ECurrency::Crowns] = 10;
	Currencies[(int32)ECurrency::Eddies] = 100;
	WantedLevel = 0;
	bHeir = true;
}

void UChimeraSessionSubsystem::StartHeir()
{
	bCharacterCreated = true;
	RecordEvent(TEXT("The next generation begins: your child/apprentice/protege inherits the world state."));
}

// ---------------------------------------------------------------------------
// GDD 6.4 - Inventory + crafting.
// ---------------------------------------------------------------------------
void UChimeraSessionSubsystem::AddItem(FName Id, int32 Count)
{
	if (Count <= 0) return;
	Inventory.FindOrAdd(Id) += Count;
	RecordEvent(FString::Printf(TEXT("Gained item: %s x%d"), *Id.ToString(), Count));
}

bool UChimeraSessionSubsystem::RemoveItem(FName Id, int32 Count)
{
	int32& Have = Inventory.FindOrAdd(Id);
	if (Have < Count) return false;
	Have -= Count;
	if (Have <= 0) Inventory.Remove(Id);
	return true;
}

bool UChimeraSessionSubsystem::Craft(FName RecipeId)
{
	const TArray<FRecipe>& All = FChimeraData::Recipes();
	const FRecipe* Recipe = nullptr;
	for (const FRecipe& R : All) if (FName(*R.Name) == RecipeId) { Recipe = &R; break; }
	if (!Recipe) return false;
	for (const FString& Ing : Recipe->Ingredients)
	{
		if (!HasItem(FName(*Ing))) return false;
	}
	for (const FString& Ing : Recipe->Ingredients) RemoveItem(FName(*Ing));
	AddItem(Recipe->ResultItem);
	AddSkillXP(FName("Tech_Crafting"), 15 + Recipe->Difficulty * 5);
	AddStat(TEXT("craft_done"), 1);
	RecordEvent(FString::Printf(TEXT("Crafted: %s"), *Recipe->ResultItem.ToString()));
	return true;
}

// ---------------------------------------------------------------------------
// GDD 9.3 - Real estate.
// ---------------------------------------------------------------------------
bool UChimeraSessionSubsystem::BuyProperty(const FString& Name)
{
	for (const FPropertyDef& P : FChimeraData::Properties())
	{
		if (P.Name == Name)
		{
			if (OwnedProperties.Contains(Name)) return false;
			if (GetCurrency(ECurrency::Credits) < P.Cost) return false;
			AddCurrency(ECurrency::Credits, -P.Cost);
			OwnedProperties.Add(Name);
			AddSkillXP(FName("Biz_RealEstate"), 20);
			RecordEvent(FString::Printf(TEXT("Bought property: %s (+%d credits/hour)"), *Name, P.IncomePerHour));
			return true;
		}
	}
	return false;
}

bool UChimeraSessionSubsystem::SellProperty(const FString& Name)
{
	if (!OwnedProperties.Contains(Name)) return false;
	OwnedProperties.Remove(Name);
	for (const FPropertyDef& P : FChimeraData::Properties())
	{
		if (P.Name == Name)
		{
			AddCurrency(ECurrency::Credits, P.Cost / 2);
			RecordEvent(FString::Printf(TEXT("Sold property: %s (recouped %d)"), *Name, P.Cost / 2));
			break;
		}
	}
	return true;
}

int32 UChimeraSessionSubsystem::GetPassiveIncomePerHour() const
{
	int32 Total = 0;
	for (const FPropertyDef& P : FChimeraData::Properties())
	{
		if (OwnedProperties.Contains(P.Name)) Total += P.IncomePerHour;
	}
	return Total;
}

bool UChimeraSessionSubsystem::IsPropertyOwned(const FString& Name) const
{
	return OwnedProperties.Contains(Name);
}

// ---------------------------------------------------------------------------
// GDD 9.4 - Insurance.
// ---------------------------------------------------------------------------
bool UChimeraSessionSubsystem::BuyInsurance(const FString& Name)
{
	for (const FInsuranceDef& I : FChimeraData::Insurances())
	{
		if (I.Name == Name)
		{
			if (ActiveInsurances.Contains(Name)) return false;
			if (GetCurrency(ECurrency::Credits) < I.PremiumPerHour) return false;
			ActiveInsurances.Add(Name);
			RecordEvent(FString::Printf(TEXT("Insurance taken out: %s (-%d credits/hour)"), *Name, I.PremiumPerHour));
			return true;
		}
	}
	return false;
}

bool UChimeraSessionSubsystem::CancelInsurance(const FString& Name)
{
	if (!ActiveInsurances.Contains(Name)) return false;
	ActiveInsurances.Remove(Name);
	RecordEvent(FString::Printf(TEXT("Insurance cancelled: %s"), *Name));
	return true;
}

int32 UChimeraSessionSubsystem::GetInsurancePremiumPerHour() const
{
	int32 Total = 0;
	for (const FInsuranceDef& I : FChimeraData::Insurances())
	{
		if (ActiveInsurances.Contains(I.Name)) Total += I.PremiumPerHour;
	}
	return Total;
}

void UChimeraSessionSubsystem::PayInsurancePremium()
{
	int32 Premium = GetInsurancePremiumPerHour();
	if (Premium <= 0) return;
	AddCurrency(ECurrency::Credits, -Premium);
	RecordEvent(FString::Printf(TEXT("Insurance premium paid: -%d credits"), Premium));
}

// ---------------------------------------------------------------------------
// GDD 3.5 - Save / Load (JSON slots).
// ---------------------------------------------------------------------------
void UChimeraSessionSubsystem::WriteInt(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, int32 Value) { Out->SetNumberField(Key, Value); }
void UChimeraSessionSubsystem::WriteFloat(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, float Value) { Out->SetNumberField(Key, Value); }
void UChimeraSessionSubsystem::WriteString(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, const FString& Value) { Out->SetStringField(Key, Value); }
void UChimeraSessionSubsystem::WriteName(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, FName Value) { Out->SetStringField(Key, Value.ToString()); }
void UChimeraSessionSubsystem::WriteIntArray(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, const TArray<int32>& Values)
{
	TArray<TSharedPtr<FJsonValue>> Arr;
	for (int32 V : Values) Arr.Add(MakeShareable(new FJsonValueNumber(V)));
	Out->SetArrayField(Key, Arr);
}
void UChimeraSessionSubsystem::WriteStringArray(TSharedPtr<FJsonObject>& Out, const TCHAR* Key, const TArray<FString>& Values)
{
	TArray<TSharedPtr<FJsonValue>> Arr;
	for (const FString& V : Values) Arr.Add(MakeShareable(new FJsonValueString(V)));
	Out->SetArrayField(Key, Arr);
}
int32 UChimeraSessionSubsystem::ReadInt(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, int32 Fallback) { return In->HasField(Key) ? (int32)In->GetNumberField(Key) : Fallback; }
float UChimeraSessionSubsystem::ReadFloat(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, float Fallback) { return In->HasField(Key) ? In->GetNumberField(Key) : Fallback; }
FString UChimeraSessionSubsystem::ReadString(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, const FString& Fallback) { return In->HasField(Key) ? In->GetStringField(Key) : Fallback; }
FName UChimeraSessionSubsystem::ReadName(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, FName Fallback) { return In->HasField(Key) ? FName(*In->GetStringField(Key)) : Fallback; }
void UChimeraSessionSubsystem::ReadIntArray(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, TArray<int32>& Out)
{
	Out.Reset();
	const TArray<TSharedPtr<FJsonValue>>& Arr = In->GetArrayField(Key);
	for (const TSharedPtr<FJsonValue>& V : Arr) Out.Add((int32)V->AsNumber());
}
void UChimeraSessionSubsystem::ReadStringArray(const TSharedPtr<FJsonObject>& In, const TCHAR* Key, TArray<FString>& Out)
{
	Out.Reset();
	const TArray<TSharedPtr<FJsonValue>>& Arr = In->GetArrayField(Key);
	for (const TSharedPtr<FJsonValue>& V : Arr) Out.Add(V->AsString());
}

bool UChimeraSessionSubsystem::SaveGame(const FString& Slot)
{
	TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject());
	Root->SetNumberField(TEXT("version"), 1);
	WriteString(Root, TEXT("ring"), StaticEnum<ERing>()->GetNameStringByValue((int64)CurrentRing));
	WriteString(Root, TEXT("lifepath"), LifePath);
	Root->SetBoolField(TEXT("created"), bCharacterCreated);
	Root->SetBoolField(TEXT("heir"), bHeir);
	WriteInt(Root, TEXT("generations"), Generations);
	WriteFloat(Root, TEXT("hours"), HoursPlayed);
	WriteInt(Root, TEXT("wanted"), WantedLevel);
	WriteInt(Root, TEXT("hero"), HeroVillainMeter);
	WriteString(Root, TEXT("governor"), Governor);
	Root->SetBoolField(TEXT("dragon"), bHasDragon);
	// GDD 14.8 / 14.9 - fame axes + player bounty.
	Root->SetNumberField(TEXT("fame"), Fame);
	Root->SetNumberField(TEXT("infamy"), Infamy);
	Root->SetNumberField(TEXT("generosity"), Generosity);
	Root->SetNumberField(TEXT("bounty"), PlayerBounty);
	// GDD 14.3 - companion.
	WriteString(Root, TEXT("pettype"), PetType);
	WriteString(Root, TEXT("petname"), PetName);
	Root->SetNumberField(TEXT("petbond"), PetBond);
	Root->SetNumberField(TEXT("petlevel"), PetLevel);
	// GDD 6.9 - dragon.
	WriteString(Root, TEXT("dragonname"), DragonName);
	Root->SetNumberField(TEXT("dragonbond"), DragonBond);
	WriteIntArray(Root, TEXT("attrs"), Attributes);
	WriteIntArray(Root, TEXT("currency"), Currencies);
	WriteIntArray(Root, TEXT("rep"), FactionRep);
	WriteIntArray(Root, TEXT("territory"), FactionTerritory);
	WriteStringArray(Root, TEXT("chronicle"), EventLog);
	TArray<FString> CompletedNames;
	for (const FName& C : CompletedQuests) CompletedNames.Add(C.ToString());
	WriteStringArray(Root, TEXT("completed"), CompletedNames);
	// Skills: { id, level, xp, perks, stars }[].
	TArray<TSharedPtr<FJsonValue>> SkillArr;
	for (const FSkillState& S : Skills)
	{
		TSharedPtr<FJsonObject> O = MakeShareable(new FJsonObject());
		O->SetStringField(TEXT("id"), S.Id.ToString());
		O->SetNumberField(TEXT("level"), S.Level);
		O->SetNumberField(TEXT("xp"), S.XP);
		O->SetNumberField(TEXT("perks"), S.Perks);
		O->SetNumberField(TEXT("stars"), S.LegendaryStars);
		SkillArr.Add(MakeShareable(new FJsonValueObject(O)));
	}
	Root->SetArrayField(TEXT("skills"), SkillArr);
	// Inventory: { id, count }[].
	TArray<TSharedPtr<FJsonValue>> InvArr;
	for (const TPair<FName, int32>& KV : Inventory)
	{
		TSharedPtr<FJsonObject> O = MakeShareable(new FJsonObject());
		O->SetStringField(TEXT("id"), KV.Key.ToString());
		O->SetNumberField(TEXT("count"), KV.Value);
		InvArr.Add(MakeShareable(new FJsonValueObject(O)));
	}
	Root->SetArrayField(TEXT("inventory"), InvArr);
	WriteStringArray(Root, TEXT("properties"), OwnedProperties);
	WriteStringArray(Root, TEXT("insurances"), ActiveInsurances);
	WriteStringArray(Root, TEXT("news"), News);
	WriteString(Root, TEXT("weather"), Weather);
	WriteString(Root, TEXT("season"), Season);
	Root->SetNumberField(TEXT("day"), GameDay);
	Root->SetNumberField(TEXT("hour"), HourOfDay);
	Root->SetBoolField(TEXT("festival"), bFestivalActive);
	WriteString(Root, TEXT("festival_name"), ActiveFestival);
	WriteString(Root, TEXT("difficulty"), StaticEnum<EDifficultyPreset>()->GetNameStringByValue((int64)Difficulty));
	// Relationships: { name, value }[].
	TArray<TSharedPtr<FJsonValue>> RelArr;
	for (const TPair<FString, int32>& KV : Relationships)
	{
		TSharedPtr<FJsonObject> O = MakeShareable(new FJsonObject());
		O->SetStringField(TEXT("name"), KV.Key);
		O->SetNumberField(TEXT("value"), KV.Value);
		RelArr.Add(MakeShareable(new FJsonValueObject(O)));
	}
	Root->SetArrayField(TEXT("relationships"), RelArr);

	FString Out;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	if (!FJsonSerializer::Serialize(Root.ToSharedRef(), Writer)) return false;
	Writer->Close();

	FString Dir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	IFileManager::Get().MakeDirectory(*Dir, true);
	return FFileHelper::SaveStringToFile(Out, *(Dir / Slot + TEXT(".json")));
}

bool UChimeraSessionSubsystem::LoadGame(const FString& Slot)
{
	FString Dir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	FString In;
	if (!FFileHelper::LoadFileToString(In, *(Dir / Slot + TEXT(".json")))) return false;
	TSharedPtr<FJsonObject> Root;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(In);
	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) return false;

	SeedDefaults();
	CurrentRing = (ERing)StaticEnum<ERing>()->GetValueByName(FName(*ReadString(Root, TEXT("ring"), TEXT("Nexus"))));
	LifePath = ReadString(Root, TEXT("lifepath"));
	bCharacterCreated = Root->GetBoolField(TEXT("created"));
	bHeir = Root->GetBoolField(TEXT("heir"));
	Generations = ReadInt(Root, TEXT("generations"));
	HoursPlayed = ReadFloat(Root, TEXT("hours"));
	WantedLevel = ReadInt(Root, TEXT("wanted"));
	HeroVillainMeter = ReadInt(Root, TEXT("hero"));
	Governor = ReadString(Root, TEXT("governor"));
	bHasDragon = Root->GetBoolField(TEXT("dragon"));
	Fame = ReadInt(Root, TEXT("fame"));
	Infamy = ReadInt(Root, TEXT("infamy"));
	Generosity = ReadInt(Root, TEXT("generosity"));
	PlayerBounty = ReadInt(Root, TEXT("bounty"));
	PetType = ReadString(Root, TEXT("pettype"));
	PetName = ReadString(Root, TEXT("petname"));
	PetBond = ReadInt(Root, TEXT("petbond"));
	PetLevel = ReadInt(Root, TEXT("petlevel"));
	DragonName = ReadString(Root, TEXT("dragonname"), TEXT("Ember"));
	DragonBond = ReadInt(Root, TEXT("dragonbond"));
	ReadIntArray(Root, TEXT("attrs"), Attributes);
	ReadIntArray(Root, TEXT("currency"), Currencies);
	ReadIntArray(Root, TEXT("rep"), FactionRep);
	ReadIntArray(Root, TEXT("territory"), FactionTerritory);
	ReadStringArray(Root, TEXT("chronicle"), EventLog);
	TArray<FString> Completed;
	ReadStringArray(Root, TEXT("completed"), Completed);
	CompletedQuests.Reset();
	for (const FString& C : Completed) CompletedQuests.Add(FName(*C));
	const TArray<TSharedPtr<FJsonValue>>& SkillArr = Root->GetArrayField(TEXT("skills"));
	for (const TSharedPtr<FJsonValue>& V : SkillArr)
	{
		const TSharedPtr<FJsonObject>& O = V->AsObject();
		FName Id(*O->GetStringField(TEXT("id")));
		FSkillState* S = GetSkill(Id);
		if (!S) continue;
		S->Level = (int32)O->GetNumberField(TEXT("level"));
		S->XP = (int32)O->GetNumberField(TEXT("xp"));
		S->Perks = (int32)O->GetNumberField(TEXT("perks"));
		S->LegendaryStars = (int32)O->GetNumberField(TEXT("stars"));
	}
	Inventory.Reset();
	const TArray<TSharedPtr<FJsonValue>>& InvArr = Root->GetArrayField(TEXT("inventory"));
	for (const TSharedPtr<FJsonValue>& V : InvArr)
	{
		const TSharedPtr<FJsonObject>& O = V->AsObject();
		Inventory.Add(FName(*O->GetStringField(TEXT("id"))), (int32)O->GetNumberField(TEXT("count")));
	}
	ReadStringArray(Root, TEXT("properties"), OwnedProperties);
	ReadStringArray(Root, TEXT("insurances"), ActiveInsurances);
	ReadStringArray(Root, TEXT("news"), News);
	Weather = ReadString(Root, TEXT("weather"), TEXT("Clear"));
	Season = ReadString(Root, TEXT("season"), TEXT("Spring"));
	GameDay = ReadInt(Root, TEXT("day"), 1);
	HourOfDay = ReadFloat(Root, TEXT("hour"), 6.f);
	bFestivalActive = Root->GetBoolField(TEXT("festival"));
	ActiveFestival = ReadString(Root, TEXT("festival_name"));
	FestivalXPBonus = bFestivalActive ? 1.5f : 1.f;
	FString DiffStr = ReadString(Root, TEXT("difficulty"), TEXT("Normal"));
	Difficulty = (EDifficultyPreset)StaticEnum<EDifficultyPreset>()->GetValueByName(FName(*DiffStr));
	Relationships.Reset();
	const TArray<TSharedPtr<FJsonValue>>& RelArr = Root->GetArrayField(TEXT("relationships"));
	for (const TSharedPtr<FJsonValue>& V : RelArr)
	{
		const TSharedPtr<FJsonObject>& O = V->AsObject();
		Relationships.Add(O->GetStringField(TEXT("name")), (int32)O->GetNumberField(TEXT("value")));
	}
	RecordEvent(TEXT("World state loaded from save slot."));
	return true;
}

// ---------------------------------------------------------------------------
// GDD 14.19.5 - Difficulty adaptation.
// ---------------------------------------------------------------------------
float UChimeraSessionSubsystem::GetDamageMultiplier() const
{
	switch (Difficulty)
	{
	case EDifficultyPreset::StoryMode: return 0.2f;
	case EDifficultyPreset::Easy: return 0.5f;
	case EDifficultyPreset::Normal: return 1.f;
	case EDifficultyPreset::Hard: return 1.5f;
	case EDifficultyPreset::Legendary: return 2.5f;
	default: return 1.f;
	}
}

float UChimeraSessionSubsystem::GetHealthMultiplier() const
{
	switch (Difficulty)
	{
	case EDifficultyPreset::StoryMode: return 0.5f;
	case EDifficultyPreset::Easy: return 0.75f;
	case EDifficultyPreset::Normal: return 1.f;
	case EDifficultyPreset::Hard: return 1.25f;
	case EDifficultyPreset::Legendary: return 2.f;
	default: return 1.f;
	}
}

float UChimeraSessionSubsystem::GetMinigameTimeMultiplier() const
{
	switch (Difficulty)
	{
	case EDifficultyPreset::StoryMode: return 2.f;
	case EDifficultyPreset::Easy: return 1.3f;
	case EDifficultyPreset::Normal: return 1.f;
	case EDifficultyPreset::Hard: return 0.8f;
	case EDifficultyPreset::Legendary: return 0.5f;
	default: return 1.f;
	}
}

// ---------------------------------------------------------------------------
// GDD 5.1 — LGBTQIA+ identity, orientation, gender transition.
// ---------------------------------------------------------------------------
void UChimeraSessionSubsystem::SetGender(EGender G)
{
	PlayerGender = G; SaveGame(TEXT("autosave"));
	RecordEvent(FString::Printf(TEXT("Gender set: %s"), *StaticEnum<EGender>()->GetNameStringByValue((int64)G)));
}
void UChimeraSessionSubsystem::SetOrientation(EOrientation O)
{
	PlayerOrientation = O;
	RecordEvent(FString::Printf(TEXT("Orientation set: %s"), *StaticEnum<EOrientation>()->GetNameStringByValue((int64)O)));
}
void UChimeraSessionSubsystem::BeginTransition()
{
	TransitionStage = ETransitionStage::Exploring;
	RecordEvent(TEXT("Began gender transition."));
	ShowMessage(TEXT("You begin your transition. Visit a hospital to continue."));
}
void UChimeraSessionSubsystem::AdvanceTransition()
{
	if (TransitionStage >= ETransitionStage::Complete) return;
	TransitionStage = (ETransitionStage)((int32)TransitionStage + 1);
	if (TransitionStage == ETransitionStage::HRT) ShowMessage(TEXT("HRT prescribed. Changes come over months."));
	else if (TransitionStage == ETransitionStage::PreOp) ShowMessage(TEXT("Pre-op complete. Surgery scheduled."));
	else if (TransitionStage == ETransitionStage::PostOp) ShowMessage(TEXT("Surgery successful. Recovery begins."));
	else if (TransitionStage == ETransitionStage::Complete) ShowMessage(TEXT("Transition complete. You are fully yourself."));
}

FString UChimeraSessionSubsystem::GetPrestigeTitle() const
{
	FSkillState Best; Best.Level = 0; Best.LegendaryStars = 0;
	for (const FSkillState& S : Skills)
	{
		if (S.Level >= Best.Level && S.LegendaryStars >= Best.LegendaryStars) Best = S;
	}
	if (Best.LegendaryStars == 0) return FString::Printf(TEXT("Aspiring %s"), *Best.Id.ToString());
	FString Ordinal = Best.LegendaryStars == 1 ? TEXT("Prestige 1") : Best.LegendaryStars == 2 ? TEXT("Prestige 2") :
		Best.LegendaryStars == 3 ? TEXT("Prestige 3") : Best.LegendaryStars == 4 ? TEXT("Prestige 4") :
		FString::Printf(TEXT("Prestige %d"), Best.LegendaryStars);
	return FString::Printf(TEXT("%s %s"), *Ordinal, *Best.Id.ToString());
}

// ---------------------------------------------------------------------------
// GDD 14.19.5 — AI difficulty (player-selectable).
// ---------------------------------------------------------------------------
void UChimeraSessionSubsystem::SetAIDifficulty(EAIDifficulty D)
{
	AIDifficulty = D;
	RecordEvent(FString::Printf(TEXT("AI Difficulty set: %s"), *StaticEnum<EAIDifficulty>()->GetNameStringByValue((int64)D)));
	ShowMessage(FString::Printf(TEXT("AI Difficulty: %s"),
		D == EAIDifficulty::VeryEasy ? TEXT("Very Easy — NPCs are forgiving, slow, rarely aggressive") :
		D == EAIDifficulty::Easy ? TEXT("Easy — NPCs give you time to react") :
		D == EAIDifficulty::Normal ? TEXT("Normal — intended AI experience") :
		D == EAIDifficulty::Hard ? TEXT("Hard — NPCs are accurate, aggressive, use tactics") :
		D == EAIDifficulty::Expert ? TEXT("Expert — minimal reaction time, flanking, rapid learning") :
		TEXT("Realistic — no mercy. One shot, one kill. They learn from every death.")));
}

float UChimeraSessionSubsystem::GetAIAccuracyMultiplier() const
{
	switch (AIDifficulty) {
	case EAIDifficulty::VeryEasy: return 0.3f;
	case EAIDifficulty::Easy: return 0.5f;
	case EAIDifficulty::Normal: return 0.65f;
	case EAIDifficulty::Hard: return 0.8f;
	case EAIDifficulty::Expert: return 0.95f;
	case EAIDifficulty::Realistic: return 1.f;
	default: return 0.65f;
	}
}

float UChimeraSessionSubsystem::GetAIReactionMultiplier() const
{
	switch (AIDifficulty) {
	case EAIDifficulty::VeryEasy: return 2.5f;  // 2.5x slower to react
	case EAIDifficulty::Easy: return 1.5f;
	case EAIDifficulty::Normal: return 1.f;
	case EAIDifficulty::Hard: return 0.7f;
	case EAIDifficulty::Expert: return 0.4f;
	case EAIDifficulty::Realistic: return 0.2f;
	default: return 1.f;
	}
}

float UChimeraSessionSubsystem::GetAIAggressionMultiplier() const
{
	switch (AIDifficulty) {
	case EAIDifficulty::VeryEasy: return 0.2f;
	case EAIDifficulty::Easy: return 0.5f;
	case EAIDifficulty::Normal: return 0.7f;
	case EAIDifficulty::Hard: return 1.f;
	case EAIDifficulty::Expert: return 1.3f;
	case EAIDifficulty::Realistic: return 1.5f;
	default: return 0.7f;
	}
}

float UChimeraSessionSubsystem::GetAILearningMultiplier() const
{
	switch (AIDifficulty) {
	case EAIDifficulty::VeryEasy: return 0.1f;
	case EAIDifficulty::Easy: return 0.3f;
	case EAIDifficulty::Normal: return 0.5f;
	case EAIDifficulty::Hard: return 0.8f;
	case EAIDifficulty::Expert: return 1.2f;
	case EAIDifficulty::Realistic: return 1.5f;
	default: return 0.5f;
	}
}

bool UChimeraSessionSubsystem::UseTeamTactics() const
{
	return AIDifficulty >= EAIDifficulty::Hard;
}

void UChimeraSessionSubsystem::SetAccessibility(EAccessibilityMode Mode)
{
	Accessibility = Mode;
	FString Msg;
	switch (Mode)
	{
	case EAccessibilityMode::None: Msg = TEXT("Accessibility mode cleared."); break;
	case EAccessibilityMode::Wheelchair: Msg = TEXT("Wheelchair mode active. Adaptive vehicles available. Transfer point access enabled."); WheelchairSpeed = 60; break;
	case EAccessibilityMode::WheelchairTransfer: Msg = TEXT("Wheelchair transfer mode. Drive from your chair, or transfer to vehicle seats."); WheelchairSpeed = 60; break;
	case EAccessibilityMode::MobilityScooter: Msg = TEXT("Mobility scooter active. Shop access enabled."); WheelchairSpeed = 30; break;
	case EAccessibilityMode::HearingImpaired: Msg = TEXT("Hearing accessibility on. Visual cues for all audio events."); break;
	case EAccessibilityMode::VisuallyImpaired: Msg = TEXT("Visual accessibility on. Audio cues for all visual events. High contrast mode."); break;
	default: break;
	}
	RecordEvent(Msg); ShowMessage(Msg);
}
