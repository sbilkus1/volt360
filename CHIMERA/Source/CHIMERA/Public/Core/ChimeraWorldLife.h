// GDD 9 / 14 — Banking, Aircraft, TV News, Education, Gym, Criminal Empire.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraWorldLife.generated.h"

// ======================================================================
// 1. BANKING — Deposit, withdraw, interest, loans, mortgages
// ======================================================================
UENUM() enum class ELoanType : uint8 { Small, Medium, Large, Mortgage, Business, COUNT };

USTRUCT() struct FBankAccount { int32 Balance = 0; float InterestRate = 0.02f; int32 LoanAmount = 0; int32 LoanRepaid = 0; ELoanType LoanType = ELoanType::Small; int32 DaysUntilPayment = 30; };

UCLASS()
class CHIMERA_API UBankingSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void Deposit(int32 Amt);
	bool Withdraw(int32 Amt);
	int32 GetBalance() const { return Account.Balance; }
	float GetInterestRate() const { return Account.InterestRate; }
	void ApplyDailyInterest();
	// Loans
	bool TakeLoan(ELoanType Type);
	bool MakeLoanPayment();
	int32 GetLoanRemaining() const { return Account.LoanAmount - Account.LoanRepaid; }
	FString GetLoanStatus() const;
private:
	FBankAccount Account;
};

// ======================================================================
// 2. AIRCRAFT — Helicopters and planes
// ======================================================================
UENUM() enum class EAircraft : uint8 { None, Helicopter, Seaplane, PrivateJet, FighterJet, Drone, COUNT };
USTRUCT() struct FAircraftDef { FString Name; EAircraft Type; int32 Cost; int32 Speed; int32 Capacity; FString Ring; FString Special; };

UCLASS()
class CHIMERA_API UAircraftSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FAircraftDef>& GetCatalog() const { return Aircraft; }
	bool BuyAircraft(const FString& Name);
	EAircraft GetActiveAircraft() const { return Active; }
	FString FlyTo(const FString& Destination);
private:
	TArray<FAircraftDef> Aircraft;
	EAircraft Active = EAircraft::None;
	FString CurrentAircraft;
};

// ======================================================================
// 3. TV NEWS — Watch reports about your actions
// ======================================================================
USTRUCT() struct FTVNewsSegment { FString Headline; FString Reporter; FString Location; FString Footage; };

UCLASS()
class CHIMERA_API UTVNewsSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void ReportPlayerAction(const FString& Action, const FString& Location);
	const TArray<FTVNewsSegment>& GetBroadcast() const { return Broadcast; }
	FTVNewsSegment GetCurrentSegment() const;
	FString WatchNews(); // returns a formatted news report
private:
	TArray<FTVNewsSegment> Broadcast;
	int32 CurrentSegment = 0;
};

// ======================================================================
// 4. EDUCATION — Skill schools, certifications
// ======================================================================
USTRUCT() struct FCourse { FString Name; FString Ring; FString Skill; int32 Cost; int32 Days; int32 XPGain; FString Prerequisite; };

UCLASS()
class CHIMERA_API UEducationSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FCourse>& GetCourses() const { return Courses; }
	bool Enroll(const FString& CourseName);
	void StudyDay(); // called daily, advances enrolled course
	FString GetProgress() const;
	int32 GetCertifications() const { return Certifications; }
private:
	TArray<FCourse> Courses;
	FString EnrolledCourse;
	int32 DaysStudied = 0;
	int32 Certifications = 0;
};

// ======================================================================
// 5. GYM — Work out to boost Body and Reflexes
// ======================================================================
UCLASS()
class CHIMERA_API UGymSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FString Workout(const FString& Exercise); // returns result + stat gain
	int32 GetWorkoutsDone() const { return WorkoutsDone; }
	float GetFitness() const { return Fitness; }
	FString GetPhysique() const;
private:
	int32 WorkoutsDone = 0;
	float Fitness = 10.f; // 0-100
};

// ======================================================================
// 6. CRIMINAL EMPIRE — Smuggling, drug ops, money laundering
// ======================================================================
USTRUCT() struct FCriminalOp { FString Name; FString Ring; int32 StartupCost; int32 IncomePerDay; int32 Risk; // 0-100
	FString CoverBusiness; FString Product; bool bActive; };

UCLASS()
class CHIMERA_API UCriminalEmpire : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FCriminalOp>& GetOperations() const { return Ops; }
	bool StartOperation(const FString& Name);
	bool CollectIncome(); // collect from all active ops
	FString GetEmpireReport() const;
	void BribeCops(int32 Amount);
	int32 GetHeat() const { return Heat; }
	int32 GetDailyIncome() const;

private:
	TArray<FCriminalOp> Ops;
	int32 Heat = 0; // police attention, 0-100
};
