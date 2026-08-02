// GDD 12 / 14 — Doctor Who: the TARDIS, the Doctor, sonic screwdrivers, companions.
// A vehicle you can explore, customise, and use as a police box during investigations.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraTARDIS.generated.h"

USTRUCT(BlueprintType)
struct CHIMERA_API FDoctorDef
{
	GENERATED_BODY()
	UPROPERTY() int32 Number = 1;          // 1..15 (+War)
	UPROPERTY() FString Actor;
	UPROPERTY() FString Era;               // "1963", "2005", etc
	UPROPERTY() FString Catchphrase;
	UPROPERTY() FString ScrewdriverStyle;  // e.g. "Classic silver", "Green Mark III", "Sonic sunglasses"
	UPROPERTY() FString CompanionExample;
};

USTRUCT(BlueprintType)
struct CHIMERA_API FSonicDef
{
	GENERATED_BODY()
	UPROPERTY() FString Name;
	UPROPERTY() FString Description;
	UPROPERTY() int32 PowerLevel = 5; // 1..10, affects hacking
};

UCLASS()
class CHIMERA_API UTARDISSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Doctor selection.
	const TArray<FDoctorDef>& GetDoctors() const { return Doctors; }
	void SetFavouriteDoctor(int32 Num);
	int32 GetFavouriteDoctor() const { return FavouriteDoctor; }

	// TARDIS state.
	bool bTARDISPresent = false;
	FString TARDISExterior = TEXT("Classic 1963 Police Box");
	FString TARDISInterior = TEXT("Default coral control room");
	bool bPoliceMode = false; // TARDIS becomes official police box during investigations

	// Sonic screwdriver.
	const TArray<FSonicDef>& GetSonics() const { return Sonics; }
	void SetSonic(int32 Index);
	int32 GetSonicIndex() const { return SonicIndex; }
	const FSonicDef* GetActiveSonic() const { return SonicIndex >= 0 && SonicIndex < Sonics.Num() ? &Sonics[SonicIndex] : nullptr; }

	// TARDIS interior exploration.
	FString ExploreRoom(); // random narrative room description
	FString ScanWithSonic(); // uses sonic screwdriver to scan environment

	// Police TARDIS mode.
	void ActivatePoliceMode();
	void DeactivatePoliceMode();

	// Companions.
	const TArray<FString>& GetCompanions() const { return Companions; }
	void AddCompanion(const FString& Name);

private:
	TArray<FDoctorDef> Doctors;
	TArray<FSonicDef> Sonics;
	TArray<FString> Companions;
	int32 FavouriteDoctor = 10; // default: 10th Doctor (David Tennant)
	int32 SonicIndex = 0;
};
