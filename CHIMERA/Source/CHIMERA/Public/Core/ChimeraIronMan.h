// GDD 14 — Iron Man Armor Catalog. Every suit from the comics (1963-2024).
// Each armor requires previous suits + trading pieces. Progression tree system.
#pragma once

#include "CoreMinimal.h"
#include "Core/ChimeraTrading.h"
#include "ChimeraIronMan.generated.h"

USTRUCT() struct FIronManArmor
{
	GENERATED_BODY()
	UPROPERTY() FString Id;
	UPROPERTY() FString Name;
	UPROPERTY() FString Mark;      // "Model 1", "Mark III", "Mark LXXXV"
	UPROPERTY() int32 Year = 2008; // comic debut or MCU year
	UPROPERTY() FString Era;       // "Classic", "Modular", "Extremis", "Bleeding Edge", "MCU"
	UPROPERTY() int32 ArmorStat = 1;    // 1-10
	UPROPERTY() int32 MobilityStat = 5;
	UPROPERTY() int32 PowerStat = 3;
	UPROPERTY() int32 StealthStat = 1;
	UPROPERTY() TArray<FString> RequiredPieces;  // trading pieces needed
	UPROPERTY() TArray<FString> Prerequisites;   // previous armors required
	UPROPERTY() int32 CreditCost = 5000;
	UPROPERTY() FString SpecialAbility;
	UPROPERTY() bool bCrafted = false;
};

// ======================================================================
// IRON MAN ARMOR DATABASE — EVERY single armor ever.
// ARMOR TREE: earlier suits are prerequisites for later ones.
// ======================================================================
namespace ChimeraIronMan
{
	// Build the complete catalog (called once, stored statically in interactable)
	void BuildCompleteCatalog(TArray<FIronManArmor>& A);

	inline TArray<FIronManArmor> GetAllArmors()
	{
		TArray<FIronManArmor> A;
		BuildCompleteCatalog(A);
		return A;
	}


	inline const FIronManArmor* FindArmor(const FString& Id, const TArray<FIronManArmor>& Armors)
	{
		for (auto& A : Armors) if (A.Id == Id) return &A;
		return nullptr;
	}

	inline bool CanCraftArmor(const FString& Id, const TArray<FIronManArmor>& Armors, UTradeSystem* Trade)
	{
		auto* A = FindArmor(Id, Armors);
		if (!A || A->bCrafted) return false;
		// Check prerequisites: all prerequisite armors must be crafted
		for (auto& Pre : A->Prerequisites)
		{
			auto* PA = FindArmor(Pre, Armors);
			if (!PA || !PA->bCrafted) return false;
		}
		// Check pieces
		for (auto& P : A->RequiredPieces)
			if (!Trade || !Trade->HasPiece(P)) return false;
		return true;
	}

	inline FString GetMissingForArmor(const FString& Id, const TArray<FIronManArmor>& Armors, UTradeSystem* Trade)
	{
		auto* A = FindArmor(Id, Armors);
		if (!A) return TEXT("Armor not found.");
		FString M;
		for (auto& Pre : A->Prerequisites)
		{
			auto* PA = FindArmor(Pre, Armors);
			if (!PA || !PA->bCrafted) M += FString::Printf(TEXT("  Prerequisite: %s (must craft first)\n"), PA ? *PA->Name : *Pre);
		}
		for (auto& P : A->RequiredPieces)
		{
			const FTradeablePiece* TP = Trade ? Trade->FindPiece(P) : nullptr;
			if (!Trade || !Trade->HasPiece(P)) M += FString::Printf(TEXT("  Missing: %s\n"), TP ? *TP->Name : *P);
		}
		return M.IsEmpty() ? TEXT("Ready to craft!") : M;
	}
}
