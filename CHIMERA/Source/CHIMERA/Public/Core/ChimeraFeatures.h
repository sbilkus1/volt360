// GDD 7 / 14 — Phone, Radio, Clothing, Gangs, Collectibles, Property Customization.
// 6 missing AAA features that make the world feel alive.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraFeatures.generated.h"

// ======================================================================
// 1. PHONE / MESSAGING SYSTEM
// ======================================================================
USTRUCT() struct FMPhoneMessage
{
	GENERATED_BODY()
	FString Sender; FString Body; int32 Hour; bool bRead;
};

UCLASS()
class CHIMERA_API UPhoneSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void SendMessage(const FString& From, const FString& Text);
	void SendMissionCall(const FString& From, const FString& Mission);
	const TArray<FMPhoneMessage>& GetInbox() const { return Inbox; }
	int32 UnreadCount() const;
	void MarkAllRead();
	FString GetSocialFeed() const;

private:
	TArray<FMPhoneMessage> Inbox;
	TArray<FString> SocialFeed;
	void GenerateSocialPost();
};

// ======================================================================
// 2. RADIO / AUDIO SYSTEM
// ======================================================================
USTRUCT() struct FRadioStation
{
	GENERATED_BODY()
	FString Name; FString Genre; FString Ring; FString Frequency;
	TArray<FString> Tracks; TArray<FString> DJLines; bool bHasNews;
};

UCLASS()
class CHIMERA_API URadioSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FRadioStation>& GetStations() const { return Stations; }
	void SetStation(int32 Idx);
	int32 GetStation() const { return CurrentStation; }
	FString GetCurrentTrack() const;
	FString GetDJLine() const;

private:
	TArray<FRadioStation> Stations;
	int32 CurrentStation = 0;
};

// ======================================================================
// 3. CLOTHING / APPEARANCE
// ======================================================================
USTRUCT() struct FClothingItem
{
	GENERATED_BODY()
	FString Name; FString Slot; FString Ring; int32 Cost;
	FString Style; FString Effect; // social effect when worn
};

UCLASS()
class CHIMERA_API UClothingSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FClothingItem>& GetCatalog() const { return Catalog; }
	bool BuyOutfit(const FString& Name);
	const FString& GetCurrentOutfit() const { return CurrentOutfit; }
	FString GetOutfitEffect() const;

private:
	TArray<FClothingItem> Catalog;
	FString CurrentOutfit = TEXT("Crash Survivor Rags");
};

// ======================================================================
// 4. GANG / TURF WARFARE
// ======================================================================
USTRUCT() struct FGangTurf
{
	GENERATED_BODY()
	FString GangName; FString Ring; FString Colour;
	int32 TurfPercent; int32 Hostility; TArray<FString> Rivals;
};

UCLASS()
class CHIMERA_API UGangWarfare : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FGangTurf>& GetGangs() const { return Gangs; }
	void AttackTurf(const FString& GangName);
	FString GetTurfReport() const;
	bool IsDrivebyActive() const { return bDrivebyActive; }

private:
	TArray<FGangTurf> Gangs;
	bool bDrivebyActive = false;
};

// ======================================================================
// 5. COLLECTIBLES (100 hidden items)
// ======================================================================
USTRUCT() struct FCollectible
{
	GENERATED_BODY()
	int32 Id; FString Name; FString Ring; FString Clue;
	bool bFound; FString Lore;
};

UCLASS()
class CHIMERA_API UCollectibleSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FCollectible>& GetAll() const { return Items; }
	int32 FoundCount() const;
	bool FindCollectible(int32 Id);
	FString GetProgressText() const;
	FString GetRewardText() const;

private:
	TArray<FCollectible> Items;
};

// ======================================================================
// 6. PROPERTY CUSTOMIZATION
// ======================================================================
USTRUCT() struct FFurnitureItem
{
	GENERATED_BODY()
	FString Name; FString Category; int32 Cost;
	FString Description;
};

UCLASS()
class CHIMERA_API UPropertyCustomizer : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;
	const TArray<FFurnitureItem>& GetFurniture() const { return Furniture; }
	bool BuyFurniture(const FString& BaseId, const FString& FurnitureName);
	const TArray<FString>& GetFurnitureInBase(const FString& BaseId) const;
	FString DescribeBase(const FString& BaseId) const;

private:
	TArray<FFurnitureItem> Furniture;
	TMap<FString, TArray<FString>> BaseFurniture;
};
