#include "Core/ChimeraWorldLife.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

// ====================================================================== 1. BANKING ======================================================================
void UBankingSystem::Deposit(int32 Amt) {
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Amt) return;
	Sess->AddCurrency(ECurrency::Credits, -Amt); Account.Balance += Amt;
	Sess->ShowMessage(FString::Printf(TEXT("Deposited %d credits. Balance: %d. Interest: %.1f%%"), Amt, Account.Balance, Account.InterestRate*100.f));
}
bool UBankingSystem::Withdraw(int32 Amt) {
	if (Account.Balance < Amt) return false;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) { Sess->AddCurrency(ECurrency::Credits, Amt); Account.Balance -= Amt; Sess->ShowMessage(FString::Printf(TEXT("Withdrew %d. Balance: %d"), Amt, Account.Balance)); }
	return true;
}
void UBankingSystem::ApplyDailyInterest() {
	if (Account.Balance > 0) { int32 Interest = FMath::RoundToInt(Account.Balance * Account.InterestRate / 365.f); Account.Balance += Interest; }
	if (Account.LoanAmount > 0) Account.DaysUntilPayment--;
}
bool UBankingSystem::TakeLoan(ELoanType Type) {
	int32 Amounts[] = {5000, 25000, 100000, 200000, 500000};
	float Rates[] = {0.05f, 0.08f, 0.12f, 0.06f, 0.10f};
	Account.LoanAmount = Amounts[(int32)Type]; Account.LoanRepaid = 0;
	Account.LoanType = Type; Account.DaysUntilPayment = 30;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) { Sess->AddCurrency(ECurrency::Credits, Account.LoanAmount); Sess->ShowMessage(FString::Printf(TEXT("LOAN: %d credits at %.0f%% APR. Payment due in 30 days."), Account.LoanAmount, Rates[(int32)Type]*100.f)); }
	return true;
}
bool UBankingSystem::MakeLoanPayment() {
	int32 Payment = Account.LoanAmount / 10;
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Payment) return false;
	Sess->AddCurrency(ECurrency::Credits, -Payment); Account.LoanRepaid += Payment; Account.DaysUntilPayment = 30;
	Sess->ShowMessage(FString::Printf(TEXT("Loan payment: %d. Remaining: %d."), Payment, GetLoanRemaining()));
	return true;
}
FString UBankingSystem::GetLoanStatus() const {
	if (Account.LoanAmount == 0) return TEXT("No active loan.");
	return FString::Printf(TEXT("Loan: %d/%d paid. %d days until next payment."), Account.LoanRepaid, Account.LoanAmount, Account.DaysUntilPayment);
}

// ====================================================================== 2. AIRCRAFT ======================================================================
void UAircraftSystem::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C);
	auto A = [&](const TCHAR* Nm, EAircraft Tp, int32 Cost, int32 Spd, int32 Cap, const TCHAR* R, const TCHAR* Sp) {
		FAircraftDef D; D.Name = Nm; D.Type = Tp; D.Cost = Cost; D.Speed = Spd; D.Capacity = Cap; D.Ring = R; D.Special = Sp; Aircraft.Add(D);
	};
	A(TEXT("News Chopper"), EAircraft::Helicopter, 50000, 250, 2, TEXT("Grand Coliseum"), TEXT("Press L to hover. Camera drone deploys for aerial photography."));
	A(TEXT("Fylde Coast Seaplane"), EAircraft::Seaplane, 35000, 180, 4, TEXT("Cleveleys"), TEXT("Lands on water. Take off from Cleveleys beach. Short-hop island hopper."));
	A(TEXT("Nexopolis Private Jet"), EAircraft::PrivateJet, 200000, 800, 12, TEXT("Grand Coliseum"), TEXT("Luxury interior. Champagne included. Land at any ring's airport instantly."));
	A(TEXT("Aether City Interceptor"), EAircraft::FighterJet, 500000, 2200, 1, TEXT("Aether City"), TEXT("Supersonic. Afterburners. Weapon hardpoints. Only the finest pilots."));
	A(TEXT("Recon Drone"), EAircraft::Drone, 8000, 80, 0, TEXT("Minecraft Frontier"), TEXT("Remote-control drone. Camera feed. 2km range. Perfect for scouting."));
}
bool UAircraftSystem::BuyAircraft(const FString& Name) {
	for (auto& D : Aircraft) { if (D.Name == Name) {
		auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>(); if (!Sess || Sess->GetCurrency(ECurrency::Credits) < D.Cost) return false;
		Sess->AddCurrency(ECurrency::Credits, -D.Cost); Active = D.Type; CurrentAircraft = Name;
		Sess->ShowMessage(FString::Printf(TEXT("AIRCRAFT: %s — %s. Speed: %d km/h."), *Name, *D.Special, D.Speed)); return true; } }
	return false;
}
FString UAircraftSystem::FlyTo(const FString& Destination) {
	if (Active == EAircraft::None) return TEXT("No aircraft owned. Buy one at Grand Coliseum Airport.");
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess) { ERing Dest = (ERing)StaticEnum<ERing>()->GetValueByName(FName(*Destination)); Sess->CurrentRing = Dest;
		Sess->ShowMessage(FString::Printf(TEXT("Flying to %s in your %s..."), *Destination, *CurrentAircraft)); }
	return TEXT("Airborne. The supercontinent stretches below.");
}

// ====================================================================== 3. TV NEWS ======================================================================
void UTVNewsSystem::ReportPlayerAction(const FString& Action, const FString& Location) {
	FTVNewsSegment S; S.Headline = Action; S.Location = Location;
	S.Reporter = FMath::RandBool() ? TEXT("Lisa Chang, NNN") : TEXT("Marcus Webb, Nexopolis Live");
	S.Footage = TEXT("Helicopter footage captured the scene as it unfolded.");
	Broadcast.Insert(S, 0); if (Broadcast.Num() > 50) Broadcast.Pop();
}
FTVNewsSegment UTVNewsSystem::GetCurrentSegment() const {
	if (Broadcast.Num() == 0) return {TEXT("Nexopolis remains stable. Keepers report Convergence steady."), TEXT("NNN Anchor"), TEXT("Nexus"), TEXT("Live from the Tower")};
	return Broadcast[FMath::RandRange(0, Broadcast.Num()-1)];
}
FString UTVNewsSystem::WatchNews() {
	auto S = GetCurrentSegment();
	return FString::Printf(TEXT("=== NEXOPOLIS NEWS NETWORK ===\n%s — %s, reporting from %s.\n%s\n================================"), *S.Headline, *S.Reporter, *S.Location, *S.Footage);
}

// ====================================================================== 4. EDUCATION ======================================================================
void UEducationSystem::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C);
	auto E = [&](const TCHAR* Nm, const TCHAR* R, const TCHAR* Sk, int32 Cost, int32 Days, int32 XP, const TCHAR* Pre) {
		FCourse Co; Co.Name = Nm; Co.Ring = R; Co.Skill = Sk; Co.Cost = Cost; Co.Days = Days; Co.XPGain = XP; Co.Prerequisite = Pre; Courses.Add(Co);
	};
	E(TEXT("Basic Medicine"), TEXT("Mercy Heights"), TEXT("Med_Surgery"), 5000, 3, 200, TEXT(""));
	E(TEXT("Advanced Surgery"), TEXT("Seattle"), TEXT("Med_Surgery"), 15000, 7, 500, TEXT("Basic Medicine"));
	E(TEXT("Police Academy"), TEXT("Mid-Wilshire"), TEXT("Pol_Investigation"), 3000, 5, 250, TEXT(""));
	E(TEXT("Detective Training"), TEXT("Chicago"), TEXT("Pol_Interrogation"), 8000, 10, 400, TEXT("Police Academy"));
	E(TEXT("Dragon Riding 101"), TEXT("Alagaesia"), TEXT("Tech_DragonRiding"), 2000, 2, 300, TEXT(""));
	E(TEXT("Ancient Language"), TEXT("Neo-Kingdom"), TEXT("Magic_AncientLanguage"), 10000, 14, 600, TEXT(""));
	E(TEXT("Combat Driving"), TEXT("Grand Coliseum"), TEXT("Tech_Driving"), 6000, 4, 250, TEXT(""));
	E(TEXT("Culinary Arts"), TEXT("Cleveleys"), TEXT("Tech_Crafting"), 2000, 2, 150, TEXT(""));
}
bool UEducationSystem::Enroll(const FString& CourseName) {
	for (auto& C : Courses) { if (C.Name == CourseName) {
		if (!C.Prerequisite.IsEmpty()) { bool bHasPre = false; for (auto& C2 : Courses) if (C2.Name == C.Prerequisite) bHasPre = true; if (!bHasPre) {/* simplified check */} }
		auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>(); if (!Sess || Sess->GetCurrency(ECurrency::Credits) < C.Cost) return false;
		Sess->AddCurrency(ECurrency::Credits, -C.Cost); EnrolledCourse = CourseName; DaysStudied = 0;
		Sess->ShowMessage(FString::Printf(TEXT("ENROLLED: %s at %s. %d days. %d XP."), *C.Name, *C.Ring, C.Days, C.XPGain)); return true; } }
	return false;
}
void UEducationSystem::StudyDay() {
	if (EnrolledCourse.IsEmpty()) return;
	DaysStudied++;
	for (auto& C : Courses) { if (C.Name == EnrolledCourse) {
		auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
		if (Sess) Sess->AddSkillXP(FName(*C.Skill), C.XPGain / C.Days);
		if (DaysStudied >= C.Days) { Certifications++; Sess->ShowMessage(FString::Printf(TEXT("COURSE COMPLETE: %s! Certification earned. %d certifications."), *C.Name, Certifications)); EnrolledCourse.Empty(); }
		return;
	}}
}
FString UEducationSystem::GetProgress() const {
	if (EnrolledCourse.IsEmpty()) return TEXT("Not enrolled in any course.");
	for (auto& C : Courses) if (C.Name == EnrolledCourse) return FString::Printf(TEXT("Studying: %s — Day %d/%d"), *C.Name, DaysStudied, C.Days);
	return TEXT("");
}

// ====================================================================== 5. GYM ======================================================================
FString UGymSystem::Workout(const FString& Exercise) {
	WorkoutsDone++; Fitness = FMath::Min(100.f, Fitness + FMath::RandRange(1.f, 5.f));
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Exercise == TEXT("Weights") && Sess) { Sess->AddAttribute(EAttribute::Body, 1); return TEXT("Weights: +1 Body. Your arms feel like they could lift a car. Don't try that."); }
	if (Exercise == TEXT("Cardio") && Sess) { Sess->AddAttribute(EAttribute::Reflexes, 1); return TEXT("Cardio: +1 Reflexes. The treadmill is your nemesis. You defeated it today."); }
	if (Exercise == TEXT("Sparring")) { if (Sess) Sess->AddSkillXP(FName("Combat_Brawling"), 15); return TEXT("Sparring: +15 Brawling XP. You lasted 3 rounds. The bag didn't hit back. Much."); }
	if (Exercise == TEXT("Yoga")) { Fitness += 3.f; return TEXT("Yoga: inner peace + physical flexibility. The Convergence feels slightly more manageable."); }
	return TEXT("Workout complete. Your body thanks you. Tomorrow it will curse you.");
}
FString UGymSystem::GetPhysique() const {
	if (Fitness < 20) return TEXT("Just starting. Everyone does. Keep going.");
	if (Fitness < 50) return TEXT("Noticeable improvement. People are starting to see it.");
	if (Fitness < 80) return TEXT("Athletic. You turn heads. Your resting heart rate is a flex.");
	return TEXT("Peak physical condition. You could outrun a dragon. Don't try that.");
}

// ====================================================================== 6. CRIMINAL EMPIRE ======================================================================
void UCriminalEmpire::Initialize(FSubsystemCollectionBase& C) { Super::Initialize(C);
	auto O = [&](const TCHAR* Nm, const TCHAR* R, int32 Cost, int32 Inc, int32 Risk, const TCHAR* Cover, const TCHAR* Prod) {
		FCriminalOp Op; Op.Name = Nm; Op.Ring = R; Op.StartupCost = Cost; Op.IncomePerDay = Inc; Op.Risk = Risk; Op.CoverBusiness = Cover; Op.Product = Prod; Ops.Add(Op);
	};
	O(TEXT("Night City Smuggling"), TEXT("Night City"), 50000, 8000, 60, TEXT("Import/export front"), TEXT("Cyberware, black market tech"));
	O(TEXT("Panem Moonshine Ring"), TEXT("Panem"), 15000, 2500, 25, TEXT("The Hob tavern"), TEXT("Illegal grain alcohol, District 12 special"));
	O(TEXT("Mid-Wilshire Chop Shop"), TEXT("Mid-Wilshire"), 30000, 5000, 45, TEXT("Auto repair shop"), TEXT("Stolen vehicles, parts, VIN swaps"));
	O(TEXT("Alagaesia Herb Trade"), TEXT("Alagaesia"), 20000, 4000, 30, TEXT("Herbalist stall"), TEXT("Rare magical herbs, illegal potion ingredients"));
	O(TEXT("Aether City Power Theft"), TEXT("Aether City"), 100000, 15000, 75, TEXT("Energy consultancy"), TEXT("Redirected grid power, kaiju-event energy spikes"));
}
bool UCriminalEmpire::StartOperation(const FString& Name) {
	for (auto& Op : Ops) { if (Op.Name == Name && !Op.bActive) {
		auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>(); if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Op.StartupCost) return false;
		Sess->AddCurrency(ECurrency::Credits, -Op.StartupCost); Op.bActive = true; Heat += Op.Risk / 4;
		Sess->WantedLevel = FMath::Min(5, Sess->WantedLevel + 1); Sess->AddInfamy(10);
		Sess->ShowMessage(FString::Printf(TEXT("CRIMINAL OP: %s — %s. Cover: %s. Heat: %d"), *Op.Name, *Op.Product, *Op.CoverBusiness, Heat)); return true; } }
	return false;
}
bool UCriminalEmpire::CollectIncome() {
	int32 Total = 0; Heat = FMath::Max(0, Heat - 5);
	for (auto& Op : Ops) {
		if (!Op.bActive) continue;
		if (FMath::RandRange(0, 100) < Op.Risk) { Heat += 20; continue; } // busted this round
		Total += Op.IncomePerDay;
	}
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (Sess && Total > 0) { Sess->AddCurrency(ECurrency::Credits, Total); Sess->ShowMessage(FString::Printf(TEXT("Criminal income: +%d credits. Heat: %d."), Total, Heat)); }
	if (Heat > 80 && Sess) { Sess->WantedLevel = FMath::Min(5, Sess->WantedLevel + 1); Sess->ShowMessage(TEXT("WARNING: Heat critical. NPD investigation imminent.")); }
	return Total > 0;
}
void UCriminalEmpire::BribeCops(int32 Amount) {
	auto* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Amount) return;
	Sess->AddCurrency(ECurrency::Credits, -Amount); Heat = FMath::Max(0, Heat - Amount/1000);
	if (Sess->WantedLevel > 0) Sess->WantedLevel = FMath::Max(0, Sess->WantedLevel - 1);
	Sess->ShowMessage(FString::Printf(TEXT("Bribe paid: %d credits. Heat reduced to %d. Wanted level: %d."), Amount, Heat, Sess->WantedLevel));
}
FString UCriminalEmpire::GetEmpireReport() const {
	FString R; int32 Active = 0, Income = 0;
	for (auto& Op : Ops) { if (Op.bActive) { Active++; Income += Op.IncomePerDay; R += FString::Printf(TEXT("%s (%s): %d/day, Risk %d%%\n"), *Op.Name, *Op.Product, Op.IncomePerDay, Op.Risk); } }
	return FString::Printf(TEXT("Criminal Empire: %d ops, %d/day, Heat %d/100\n%s"), Active, Income, Heat, *R);
}
int32 UCriminalEmpire::GetDailyIncome() const { int32 T = 0; for (auto& Op : Ops) if (Op.bActive) T += Op.IncomePerDay; return T; }
