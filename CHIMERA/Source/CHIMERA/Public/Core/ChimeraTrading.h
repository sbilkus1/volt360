// GDD 9 / 14 — Trading Marketplace + Superhero Blueprint Pieces.
// Buy, sell, and trade parts. Each hero blueprint requires specific pieces.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ChimeraTrading.generated.h"

// ======================================================================
// Tradeable Superhero Pieces (required to craft blueprints)
// ======================================================================
USTRUCT() struct FTradeablePiece
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString Category;    // "Tech", "Magic", "Alien", "Cosmic", "Mutant", "Meta", "Mystic"
	UPROPERTY() int32 BuyPrice = 5000;
	UPROPERTY() int32 SellPrice = 2500;
	UPROPERTY() FString Description;
	UPROPERTY() TArray<FString> UsedIn; // which blueprint IDs need this piece
};

// ======================================================================
// Complete Blueprint Parts Catalog — every hero/villain needs specific pieces
// ======================================================================
// Each of the 160+ Marvel/DC blueprints from ChimeraSuperhero requires 3-4 pieces.
// When all pieces are owned, the blueprint can be crafted.

USTRUCT() struct FTradeOffer
{
	GENERATED_BODY()
	UPROPERTY() FString ItemId;      // what's being sold
	UPROPERTY() int32 AskPrice;      // what the seller wants
	UPROPERTY() int32 Quantity = 1;
	UPROPERTY() FString Seller;      // "Market", "BlackMarket", "Trader"
};

UCLASS()
class CHIMERA_API UTradeSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& C) override;

	// === Piece Catalog ===
	const TArray<FTradeablePiece>& GetPieces() const { return Pieces; }
	const FTradeablePiece* FindPiece(const FString& Id) const;

	// === Player Inventory of Pieces ===
	bool HasPiece(const FString& Id) const;
	int32 GetPieceCount(const FString& Id) const;
	bool BuyPiece(const FString& Id);     // buy from marketplace
	bool SellPiece(const FString& Id);    // sell to marketplace
	void AddPiece(const FString& Id, int32 Count = 1);

	// === Trading Post Offers (random daily deals) ===
	const TArray<FTradeOffer>& GetOffers() const { return Offers; }
	void RefreshOffers();
	bool BuyOffer(int32 OfferIndex);

	// === Blueprint Crafting Check ===
	bool CanCraftBlueprint(const FString& BlueprintId) const; // from ChimeraSuperhero catalog
	FString GetMissingPieces(const FString& BlueprintId) const;

private:
	void SeedPieces();
	TArray<FTradeablePiece> Pieces;
	TArray<FTradeOffer> Offers;
	TMap<FString, int32> OwnedPieces; // piece ID -> quantity
};
