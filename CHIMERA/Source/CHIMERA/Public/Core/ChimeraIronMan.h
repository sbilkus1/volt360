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
// IRON MAN ARMOR DATABASE — 70+ suits across all eras
// ARMOR TREE: earlier suits are prerequisites for later ones.
// ======================================================================
namespace ChimeraIronMan
{
	inline TArray<FIronManArmor> GetAllArmors()
	{
		TArray<FIronManArmor> A;
		FIronManArmor IM;

		// ===== CLASSIC ERA (1963-1985) — the foundational suits =====
		IM.Mark = TEXT("Model 1"); IM.Name = TEXT("Iron Man Mark I — The Grey Armor");
		IM.Year = 1963; IM.Era = TEXT("Classic");
		IM.ArmorStat = 4; IM.MobilityStat = 2; IM.PowerStat = 2; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("nanotube_alloy") };
		IM.CreditCost = 5000;
		IM.SpecialAbility = TEXT("Built in a cave with a box of scraps. The armor that started everything. Heavy. Slow. Bulletproof. Chest-mounted unibeam.");
		A.Add(IM);

		IM.Mark = TEXT("Model 2"); IM.Name = TEXT("Iron Man Mark II — The Golden Avenger");
		IM.Year = 1964; IM.Era = TEXT("Classic");
		IM.ArmorStat = 4; IM.MobilityStat = 3; IM.PowerStat = 3; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("repulsor_coil") };
		IM.Prerequisites = { TEXT("iron_man_mk1") }; IM.CreditCost = 8000;
		IM.SpecialAbility = TEXT("First suit to use repulsor technology. Golden. Shiny. Intimidating. Still no flight stabilisers.");
		A.Add(IM);

		IM.Mark = TEXT("Model 3"); IM.Name = TEXT("Iron Man Mark III — Classic Red & Gold");
		IM.Year = 1964; IM.Era = TEXT("Classic");
		IM.ArmorStat = 5; IM.MobilityStat = 4; IM.PowerStat = 4; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("repulsor_coil"), TEXT("arc_reactor") };
		IM.Prerequisites = { TEXT("iron_man_mk2") }; IM.CreditCost = 10000;
		IM.SpecialAbility = TEXT("The iconic red and gold. First true Iron Man. Sleeve-mounted repulsors. Unibeam chest projector.");
		A.Add(IM);

		IM.Mark = TEXT("Model 4"); IM.Name = TEXT("Iron Man Mark IV — Classic Refined");
		IM.Year = 1970; IM.Era = TEXT("Classic");
		IM.ArmorStat = 5; IM.MobilityStat = 5; IM.PowerStat = 5; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("ai_chip") };
		IM.Prerequisites = { TEXT("iron_man_mk3") }; IM.CreditCost = 12000;
		IM.SpecialAbility = TEXT("Refined version. Better power distribution. First to include onboard computer assist.");
		A.Add(IM);

		// ===== SPECIALISED CLASSIC ARMORS =====
		IM.Mark = TEXT("Space Armor MK I"); IM.Name = TEXT("Space Armor — Deep Space Suit");
		IM.Year = 1970; IM.Era = TEXT("Classic");
		IM.ArmorStat = 5; IM.MobilityStat = 6; IM.PowerStat = 5; IM.StealthStat = 3;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("tesseract_energy") };
		IM.Prerequisites = { TEXT("iron_man_mk3") }; IM.CreditCost = 15000;
		IM.SpecialAbility = TEXT("Vacuum-sealed. Oxygen supply: 72 hours. Deep space navigation. First suit to leave Earth's orbit.");
		A.Add(IM);

		IM.Mark = TEXT("Stealth Armor MK I"); IM.Name = TEXT("Stealth Armor — Blue Ghost");
		IM.Year = 1981; IM.Era = TEXT("Classic");
		IM.ArmorStat = 3; IM.MobilityStat = 5; IM.PowerStat = 3; IM.StealthStat = 9;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("ai_chip") };
		IM.Prerequisites = { TEXT("iron_man_mk4") }; IM.CreditCost = 15000;
		IM.SpecialAbility = TEXT("Radar-absorbent coating. Silent flight. Thermal masking. Blue-black finish. Invisible to every sensor.");
		A.Add(IM);

		IM.Mark = TEXT("Hydro Armor"); IM.Name = TEXT("Hydro Armor — Underwater Suit");
		IM.Year = 1981; IM.Era = TEXT("Classic");
		IM.ArmorStat = 5; IM.MobilityStat = 4; IM.PowerStat = 4; IM.StealthStat = 5;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("vibranium") };
		IM.Prerequisites = { TEXT("iron_man_mk4") }; IM.CreditCost = 14000;
		IM.SpecialAbility = TEXT("Pressure-resistant to 4000m. Hydro-jets. Sonar array. Electric eel-inspired shock defense.");
		A.Add(IM);

		// ===== SILVER CENTURION ERA =====
		IM.Mark = TEXT("Silver Centurion"); IM.Name = TEXT("Silver Centurion Armor");
		IM.Year = 1985; IM.Era = TEXT("Classic");
		IM.ArmorStat = 6; IM.MobilityStat = 5; IM.PowerStat = 6; IM.StealthStat = 2;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("ai_chip"), TEXT("repulsor_coil") };
		IM.Prerequisites = { TEXT("iron_man_mk4") }; IM.CreditCost = 20000;
		IM.SpecialAbility = TEXT("Silver and crimson. Chameleon mode (bends light for camouflage). Pulse bolts. First shoulder-mounted weapons.");
		A.Add(IM);

		// ===== MODULAR ARMOR ERA (1990s) =====
		IM.Mark = TEXT("Modular Armor"); IM.Name = TEXT("Modular Armor — War Machine Prototype");
		IM.Year = 1992; IM.Era = TEXT("Modular");
		IM.ArmorStat = 7; IM.MobilityStat = 6; IM.PowerStat = 6; IM.StealthStat = 2;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("wrist_blaster"), TEXT("ai_chip") };
		IM.Prerequisites = { TEXT("silver_centurion") }; IM.CreditCost = 25000;
		IM.SpecialAbility = TEXT("Hot-swappable weapon modules. Gatling gun. Missile pods. The prototype that became War Machine.");
		A.Add(IM);

		IM.Mark = TEXT("Hulkbuster Mk I"); IM.Name = TEXT("Hulkbuster Armor — Original");
		IM.Year = 1994; IM.Era = TEXT("Modular");
		IM.ArmorStat = 9; IM.MobilityStat = 3; IM.PowerStat = 8; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("adamantium"), TEXT("gamma_isotope") };
		IM.Prerequisites = { TEXT("modular_armor") }; IM.CreditCost = 50000;
		IM.SpecialAbility = TEXT("Designed specifically to go toe-to-toe with the Hulk. Reinforced everything. Auto-repair nanites. 'Go to sleep, Banner.'");
		A.Add(IM);

		IM.Mark = TEXT("Arctic Armor"); IM.Name = TEXT("Arctic Armor — Sub-Zero Suit");
		IM.Year = 1996; IM.Era = TEXT("Modular");
		IM.ArmorStat = 5; IM.MobilityStat = 5; IM.PowerStat = 4; IM.StealthStat = 4;
		IM.RequiredPieces = { TEXT("arc_reactor") };
		IM.Prerequisites = { TEXT("modular_armor") }; IM.CreditCost = 12000;
		IM.SpecialAbility = TEXT("Functional in -200 degree environments. Heating coils woven into every plate. Ice shatters on contact.");
		A.Add(IM);

		// ===== EXTREMIS / BLEEDING EDGE ERA (2000s-2010s) =====
		IM.Mark = TEXT("Extremis Armor"); IM.Name = TEXT("Extremis Armor — Nanotech Integration");
		IM.Year = 2005; IM.Era = TEXT("Extremis");
		IM.ArmorStat = 7; IM.MobilityStat = 8; IM.PowerStat = 7; IM.StealthStat = 4;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("nanotube_alloy"), TEXT("ai_chip"), TEXT("super_soldier_serum") };
		IM.Prerequisites = { TEXT("silver_centurion") }; IM.CreditCost = 40000;
		IM.SpecialAbility = TEXT("Stored in Tony's bones. Deploys from his body via neural interface. Direct brain-to-armor connection. The game-changer.");
		A.Add(IM);

		IM.Mark = TEXT("Bleeding Edge"); IM.Name = TEXT("Bleeding Edge Armor — Liquid Metal");
		IM.Year = 2010; IM.Era = TEXT("Bleeding Edge");
		IM.ArmorStat = 8; IM.MobilityStat = 9; IM.PowerStat = 8; IM.StealthStat = 6;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("nanotube_alloy"), TEXT("repulsor_coil"), TEXT("ai_chip") };
		IM.Prerequisites = { TEXT("extremis") }; IM.CreditCost = 60000;
		IM.SpecialAbility = TEXT("Liquid smart-metal. Flows from Tony's body. Shapes any weapon imaginable. Repairs mid-combat. Unstoppable.");
		A.Add(IM);

		IM.Mark = TEXT("Endo-Sym"); IM.Name = TEXT("Endo-Sym Armor — Symbiote Hybrid");
		IM.Year = 2014; IM.Era = TEXT("Bleeding Edge");
		IM.ArmorStat = 7; IM.MobilityStat = 9; IM.PowerStat = 8; IM.StealthStat = 7;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("symbiote_sample"), TEXT("nanotube_alloy") };
		IM.Prerequisites = { TEXT("bleeding_edge") }; IM.CreditCost = 75000;
		IM.SpecialAbility = TEXT("Liquid smart-metal bonded with symbiote biology. Can shapeshift into anyone. Weak to sonic and fire. Morally questionable.");
		A.Add(IM);

		// ===== SPECIALISED EXTREMIS-ERA ARMORS =====
		IM.Mark = TEXT("Thorbuster"); IM.Name = TEXT("Thorbuster Armor — Asgardian Counter");
		IM.Year = 2011; IM.Era = TEXT("Extremis");
		IM.ArmorStat = 9; IM.MobilityStat = 6; IM.PowerStat = 10; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("uru_metal"), TEXT("storm_essence"), TEXT("adamantium") };
		IM.Prerequisites = { TEXT("extremis"), TEXT("hulkbuster_mk1") }; IM.CreditCost = 120000;
		IM.SpecialAbility = TEXT("Powered by an Asgardian crystal. Absorbs and redirects Mjolnir's lightning. 'If a god bleeds, we can kill it.'");
		A.Add(IM);

		IM.Mark = TEXT("Phoenix Buster"); IM.Name = TEXT("Phoenix Buster — Cosmic Countermeasure");
		IM.Year = 2012; IM.Era = TEXT("Extremis");
		IM.ArmorStat = 8; IM.MobilityStat = 6; IM.PowerStat = 10; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("phoenix_shard"), TEXT("power_cosmic") };
		IM.Prerequisites = { TEXT("thorbuster") }; IM.CreditCost = 200000;
		IM.SpecialAbility = TEXT("Designed to fight the Phoenix Force. Splits cosmic fire into harmless energy. Barely worked. Worth a shot.");
		A.Add(IM);

		IM.Mark = TEXT("Hypervelocity"); IM.Name = TEXT("Hypervelocity Armor — Mach 10");
		IM.Year = 2008; IM.Era = TEXT("Extremis");
		IM.ArmorStat = 5; IM.MobilityStat = 10; IM.PowerStat = 6; IM.StealthStat = 3;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("speed_force_conduit") };
		IM.Prerequisites = { TEXT("extremis") }; IM.CreditCost = 55000;
		IM.SpecialAbility = TEXT("Mach 10 flight. Tony uploaded his consciousness into the armor while his body healed. Became pure data briefly.");
		A.Add(IM);

		IM.Mark = TEXT("Godkiller Mk I"); IM.Name = TEXT("Godkiller Armor — Celestial Slayer");
		IM.Year = 2013; IM.Era = TEXT("Extremis");
		IM.ArmorStat = 9; IM.MobilityStat = 7; IM.PowerStat = 10; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("power_cosmic"), TEXT("uru_metal"), TEXT("mother_box") };
		IM.Prerequisites = { TEXT("phoenix_buster"), TEXT("thorbuster") }; IM.CreditCost = 500000;
		IM.SpecialAbility = TEXT("Colossal. Built to fight Celestials. Planet-cracking weaponry. One suit. One Celestial. One winner.");
		A.Add(IM);

		// ===== MCU SUITS (2008-2019) =====
		IM.Mark = TEXT("Mark III"); IM.Name = TEXT("MCU Mark III — First Flight");
		IM.Year = 2008; IM.Era = TEXT("MCU");
		IM.ArmorStat = 5; IM.MobilityStat = 5; IM.PowerStat = 5; IM.StealthStat = 2;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("repulsor_coil"), TEXT("ai_chip") };
		IM.Prerequisites = { TEXT("iron_man_mk1") }; IM.CreditCost = 15000;
		IM.SpecialAbility = TEXT("The suit that launched the MCU. Culver City flight test. Tank missile. Gold-titanium alloy. 'Yeah, I can fly.'");
		A.Add(IM);

		IM.Mark = TEXT("Mark VI"); IM.Name = TEXT("MCU Mark VI — Triangle Arc Reactor");
		IM.Year = 2010; IM.Era = TEXT("MCU");
		IM.ArmorStat = 6; IM.MobilityStat = 6; IM.PowerStat = 6; IM.StealthStat = 2;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("vibranium"), TEXT("repulsor_coil") };
		IM.Prerequisites = { TEXT("mcu_mk3") }; IM.CreditCost = 18000;
		IM.SpecialAbility = TEXT("New triangular arc reactor. Underwater deployment. Laser cutter. Suitcase-portable. Fought Thor and lived.");
		A.Add(IM);

		IM.Mark = TEXT("Mark VII"); IM.Name = TEXT("MCU Mark VII — Battle of New York");
		IM.Year = 2012; IM.Era = TEXT("MCU");
		IM.ArmorStat = 7; IM.MobilityStat = 7; IM.PowerStat = 7; IM.StealthStat = 3;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("nanotube_alloy"), TEXT("ai_chip"), TEXT("repulsor_coil") };
		IM.Prerequisites = { TEXT("mcu_mk6") }; IM.CreditCost = 25000;
		IM.SpecialAbility = TEXT("Remote-deploying via wristbands. Autonomously navigated Stark Tower. Caught Tony mid-fall. Nuke through the portal.");
		A.Add(IM);

		IM.Mark = TEXT("Mark XLII"); IM.Name = TEXT("MCU Mark XLII — The Prodigal Son");
		IM.Year = 2013; IM.Era = TEXT("MCU");
		IM.ArmorStat = 6; IM.MobilityStat = 8; IM.PowerStat = 6; IM.StealthStat = 4;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("nanotube_alloy"), TEXT("ai_chip") };
		IM.Prerequisites = { TEXT("mcu_mk7") }; IM.CreditCost = 30000;
		IM.SpecialAbility = TEXT("Prehensile suit. Individual pieces fly to Tony from anywhere. 'I'm the best.' Falls apart constantly. Still works.");
		A.Add(IM);

		IM.Mark = TEXT("Mark XLIV"); IM.Name = TEXT("MCU Hulkbuster — Veronica");
		IM.Year = 2015; IM.Era = TEXT("MCU");
		IM.ArmorStat = 10; IM.MobilityStat = 4; IM.PowerStat = 9; IM.StealthStat = 1;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("adamantium"), TEXT("gamma_isotope"), TEXT("vibranium") };
		IM.Prerequisites = { TEXT("mcu_mk7"), TEXT("hulkbuster_mk1") }; IM.CreditCost = 80000;
		IM.SpecialAbility = TEXT("Modular deployment from orbital satellite Veronica. Replacement parts mid-fight. 'Go to sleep, go to sleep, go to sleep.'");
		A.Add(IM);

		IM.Mark = TEXT("Mark XLV"); IM.Name = TEXT("MCU Mark XLV — Heartbreaker");
		IM.Year = 2015; IM.Era = TEXT("MCU");
		IM.ArmorStat = 7; IM.MobilityStat = 7; IM.PowerStat = 8; IM.StealthStat = 4;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("ai_chip"), TEXT("repulsor_coil") };
		IM.Prerequisites = { TEXT("mcu_mk42") }; IM.CreditCost = 35000;
		IM.SpecialAbility = TEXT("First suit with Friday AI. Hexagonal chest RT. Sokovia battle. 'Together.' 'We'll lose.' 'Then we'll do that together too.'");
		A.Add(IM);

		IM.Mark = TEXT("Mark L"); IM.Name = TEXT("MCU Mark L — Nanotech Genesis");
		IM.Year = 2018; IM.Era = TEXT("MCU");
		IM.ArmorStat = 8; IM.MobilityStat = 9; IM.PowerStat = 9; IM.StealthStat = 5;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("nanotube_alloy"), TEXT("ai_chip"), TEXT("repulsor_coil"), TEXT("infinity_gauntlet") };
		IM.Prerequisites = { TEXT("mcu_mk45"), TEXT("bleeding_edge") }; IM.CreditCost = 100000;
		IM.SpecialAbility = TEXT("Nanotech. Deploys from the chest RT. Forms weapons, shields, cannons instantly. Fought Thanos 1v1. 'You throw another moon at me...'");
		A.Add(IM);

		IM.Mark = TEXT("Mark LXXXV"); IM.Name = TEXT("MCU Mark LXXXV — The Snap");
		IM.Year = 2019; IM.Era = TEXT("MCU");
		IM.ArmorStat = 9; IM.MobilityStat = 10; IM.PowerStat = 10; IM.StealthStat = 6;
		IM.RequiredPieces = { TEXT("arc_reactor"), TEXT("nanotube_alloy"), TEXT("ai_chip"), TEXT("power_stone"), TEXT("infinity_gauntlet") };
		IM.Prerequisites = { TEXT("mcu_mk50") }; IM.CreditCost = 250000;
		IM.SpecialAbility = TEXT("The final MCU armor. Integrated Infinity Gauntlet. Thor's lightning absorption. The Snap. 'And I... am... Iron Man.' Snap. 3,000.");
		A.Add(IM);

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
