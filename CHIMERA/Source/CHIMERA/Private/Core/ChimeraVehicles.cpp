#include "Core/ChimeraVehicles.h"
#include "Core/ChimeraSessionSubsystem.h"
#include "Engine/GameInstance.h"
#include "CHIMERA.h"

#define CAR(id,make,model,yr,cls,val,spd,acc,han,cntry) \
	V.Id=id; V.Make=make; V.Model=model; V.Year=yr; V.VClass=cls; V.BaseValue=val; V.TopSpeed=spd; V.Acceleration=acc; V.Handling=han; V.Country=cntry; Catalog.Add(V);

// Forward: complete catalogs in ChimeraVehiclesComplete.cpp
void AddMercedesComplete(TArray<FVehicleDef>&);
void AddPorscheComplete(TArray<FVehicleDef>&);
void AddBMWComplete(TArray<FVehicleDef>&);
void AddFerrariComplete(TArray<FVehicleDef>&);
void AddLamborghiniComplete(TArray<FVehicleDef>&);
void AddAstonMartinComplete(TArray<FVehicleDef>&);
void AddAudiComplete(TArray<FVehicleDef>&);

void UVehicleSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SeedCatalog();
	SeedUpgrades();
}

void UVehicleSystem::SeedCatalog()
{
	FVehicleDef V;
	// Load complete manufacturer catalogs
	AddMercedesComplete(Catalog);
	AddPorscheComplete(Catalog);
	AddBMWComplete(Catalog);
	AddFerrariComplete(Catalog);
	AddLamborghiniComplete(Catalog);
	AddAstonMartinComplete(Catalog);
	AddAudiComplete(Catalog);
	// === TOYOTA ===
	CAR("supra_mk4", "Toyota", "Supra MK4", 1993, EVehicleClass::Sports, 65000, 250, 65, 75, "Japan");
	CAR("supra_mk5", "Toyota", "GR Supra", 2020, EVehicleClass::Sports, 58000, 250, 68, 80, "Japan");
	CAR("ae86", "Toyota", "AE86 Sprinter Trueno", 1986, EVehicleClass::Classic, 18000, 195, 40, 85, "Japan");
	CAR("gt86", "Toyota", "GT86", 2015, EVehicleClass::Sports, 32000, 230, 55, 88, "Japan");
	CAR("celica_gt4", "Toyota", "Celica GT-Four", 1994, EVehicleClass::Rally, 25000, 240, 52, 80, "Japan");
	CAR("mr2", "Toyota", "MR2", 1991, EVehicleClass::Sports, 15000, 220, 50, 82, "Japan");
	CAR("land_cruiser", "Toyota", "Land Cruiser", 2020, EVehicleClass::SUV, 85000, 180, 35, 50, "Japan");
	CAR("hilux", "Toyota", "Hilux", 1995, EVehicleClass::Truck, 12000, 160, 30, 45, "Japan");
	CAR("camry", "Toyota", "Camry", 2020, EVehicleClass::Sedan, 28000, 210, 45, 65, "Japan");
	CAR("corolla", "Toyota", "Corolla", 1985, EVehicleClass::Compact, 5000, 170, 30, 60, "Japan");

	// === HONDA ===
	CAR("nsx", "Honda", "NSX", 1991, EVehicleClass::Super, 75000, 280, 72, 85, "Japan");
	CAR("nsx_new", "Honda", "NSX Type S", 2022, EVehicleClass::Super, 170000, 308, 80, 88, "Japan");
	CAR("civic_type_r", "Honda", "Civic Type R", 2020, EVehicleClass::Sports, 38000, 272, 62, 86, "Japan");
	CAR("s2000", "Honda", "S2000", 2000, EVehicleClass::Sports, 28000, 240, 58, 88, "Japan");
	CAR("integra_type_r", "Honda", "Integra Type R", 1998, EVehicleClass::Sports, 22000, 233, 55, 85, "Japan");
	CAR("crx", "Honda", "CRX SiR", 1990, EVehicleClass::Compact, 8000, 200, 42, 82, "Japan");
	CAR("cbr1000", "Honda", "CBR1000RR Fireblade", 2020, EVehicleClass::Motorcycle, 18000, 299, 90, 95, "Japan");

	// === NISSAN ===
	CAR("gtr_r34", "Nissan", "Skyline GT-R R34", 1999, EVehicleClass::Sports, 70000, 250, 70, 82, "Japan");
	CAR("gtr_r35", "Nissan", "GT-R R35", 2020, EVehicleClass::Super, 110000, 315, 82, 85, "Japan");
	CAR("350z", "Nissan", "350Z", 2005, EVehicleClass::Sports, 18000, 250, 55, 78, "Japan");
	CAR("370z", "Nissan", "370Z Nismo", 2020, EVehicleClass::Sports, 45000, 270, 62, 82, "Japan");
	CAR("silvia_s15", "Nissan", "Silvia S15 Spec-R", 1999, EVehicleClass::Sports, 22000, 240, 52, 84, "Japan");
	CAR("180sx", "Nissan", "180SX Type X", 1996, EVehicleClass::Sports, 15000, 225, 48, 80, "Japan");
	CAR("240z", "Nissan", "Fairlady 240Z", 1972, EVehicleClass::Classic, 30000, 210, 40, 75, "Japan");

	// === MITSUBISHI ===
	CAR("lancer_evo_vi", "Mitsubishi", "Lancer Evolution VI", 1999, EVehicleClass::Rally, 28000, 250, 60, 84, "Japan");
	CAR("lancer_evo_x", "Mitsubishi", "Lancer Evolution X", 2015, EVehicleClass::Sports, 32000, 255, 58, 86, "Japan");
	CAR("gto", "Mitsubishi", "3000GT VR-4", 1995, EVehicleClass::Sports, 18000, 250, 55, 76, "Japan");
	CAR("eclipse", "Mitsubishi", "Eclipse GSX", 1997, EVehicleClass::Sports, 10000, 220, 48, 75, "Japan");
	CAR("pajero", "Mitsubishi", "Pajero Evolution", 2002, EVehicleClass::SUV, 20000, 190, 35, 52, "Japan");

	// === SUBARU ===
	CAR("impreza_22b", "Subaru", "Impreza 22B STi", 1998, EVehicleClass::Rally, 90000, 240, 58, 86, "Japan");
	CAR("wrx_sti", "Subaru", "WRX STi", 2020, EVehicleClass::Sports, 38000, 260, 55, 82, "Japan");
	CAR("brz", "Subaru", "BRZ", 2020, EVehicleClass::Sports, 28000, 230, 55, 90, "Japan");

	// === MAZDA ===
	CAR("rx7_fd", "Mazda", "RX-7 FD3S", 1995, EVehicleClass::Sports, 35000, 255, 60, 86, "Japan");
	CAR("rx8", "Mazda", "RX-8 Spirit R", 2008, EVehicleClass::Sports, 12000, 235, 50, 82, "Japan");
	CAR("miata", "Mazda", "MX-5 Miata", 1990, EVehicleClass::Compact, 8000, 195, 40, 92, "Japan");
	CAR("miata_nd", "Mazda", "MX-5 RF", 2020, EVehicleClass::Sports, 32000, 220, 52, 94, "Japan");

	// === LEXUS ===
	CAR("lfa", "Lexus", "LFA", 2012, EVehicleClass::Super, 500000, 325, 82, 88, "Japan");
	CAR("lc500", "Lexus", "LC 500", 2020, EVehicleClass::Sports, 95000, 270, 65, 80, "Japan");

	// === FORD ===
	CAR("mustang_gt", "Ford", "Mustang GT", 2020, EVehicleClass::Muscle, 38000, 250, 65, 72, "USA");
	CAR("mustang_eleanor", "Ford", "Mustang GT500 'Eleanor'", 1967, EVehicleClass::Classic, 120000, 230, 55, 62, "USA");
	CAR("focus_rs", "Ford", "Focus RS", 2018, EVehicleClass::Sports, 35000, 266, 58, 82, "USA");
	CAR("f150_raptor", "Ford", "F-150 Raptor", 2020, EVehicleClass::Truck, 60000, 180, 40, 50, "USA");
	CAR("gt", "Ford", "GT", 2020, EVehicleClass::Super, 500000, 347, 85, 82, "USA");
	CAR("escort_cosworth", "Ford", "Escort RS Cosworth", 1994, EVehicleClass::Rally, 40000, 225, 48, 78, "UK");

	// === CHEVROLET ===
	CAR("camaro_zl1", "Chevrolet", "Camaro ZL1", 2020, EVehicleClass::Muscle, 65000, 298, 72, 70, "USA");
	CAR("corvette_c8", "Chevrolet", "Corvette C8 Stingray", 2020, EVehicleClass::Super, 68000, 312, 78, 84, "USA");
	CAR("impala_67", "Chevrolet", "Impala SS", 1967, EVehicleClass::Classic, 25000, 200, 45, 55, "USA");
	CAR("silverado", "Chevrolet", "Silverado 1500", 2020, EVehicleClass::Truck, 35000, 175, 38, 48, "USA");
	CAR("chevelle_ss", "Chevrolet", "Chevelle SS 454", 1970, EVehicleClass::Muscle, 45000, 220, 60, 55, "USA");

	// === DODGE ===
	CAR("charger_hellcat", "Dodge", "Charger SRT Hellcat", 2020, EVehicleClass::Muscle, 70000, 328, 75, 65, "USA");
	CAR("challenger_demon", "Dodge", "Challenger SRT Demon", 2018, EVehicleClass::Muscle, 85000, 325, 80, 60, "USA");
	CAR("viper_acr", "Dodge", "Viper ACR", 2017, EVehicleClass::Super, 120000, 331, 82, 80, "USA");

	// === BMW ===
	CAR("m3_e30", "BMW", "M3 E30", 1988, EVehicleClass::Classic, 50000, 230, 48, 86, "Germany");
	CAR("m3_e46", "BMW", "M3 E46 CSL", 2004, EVehicleClass::Sports, 35000, 250, 58, 85, "Germany");
	CAR("m3_g80", "BMW", "M3 Competition", 2022, EVehicleClass::Sports, 75000, 290, 68, 84, "Germany");
	CAR("m5_f90", "BMW", "M5 CS", 2022, EVehicleClass::Sports, 140000, 305, 72, 78, "Germany");
	CAR("m8", "BMW", "M8 Competition", 2022, EVehicleClass::Sports, 140000, 305, 70, 76, "Germany");
	CAR("z4", "BMW", "Z4 M40i", 2020, EVehicleClass::Sports, 50000, 250, 58, 82, "Germany");
	CAR("i8", "BMW", "i8", 2019, EVehicleClass::Sports, 140000, 250, 55, 80, "Germany");
	CAR("s1000rr", "BMW", "S1000RR", 2022, EVehicleClass::Motorcycle, 20000, 305, 95, 92, "Germany");

	// === MERCEDES ===
	CAR("amg_gt", "Mercedes-AMG", "GT R", 2020, EVehicleClass::Super, 162000, 318, 78, 82, "Germany");
	CAR("amg_one", "Mercedes-AMG", "ONE", 2023, EVehicleClass::Hypercar, 2750000, 352, 92, 88, "Germany");
	CAR("c63", "Mercedes", "C63 AMG", 2020, EVehicleClass::Sports, 70000, 290, 65, 78, "Germany");
	CAR("s_class", "Mercedes", "S-Class Maybach", 2023, EVehicleClass::Sedan, 185000, 250, 52, 72, "Germany");
	CAR("sls_amg", "Mercedes", "SLS AMG Black Series", 2013, EVehicleClass::Super, 250000, 315, 75, 78, "Germany");
	CAR("g_wagon", "Mercedes", "G63 AMG", 2022, EVehicleClass::SUV, 160000, 240, 48, 52, "Germany");
	CAR("unimog", "Mercedes", "Unimog", 1985, EVehicleClass::Truck, 25000, 90, 15, 20, "Germany");

	// === AUDI ===
	CAR("r8", "Audi", "R8 V10 Performance", 2022, EVehicleClass::Super, 195000, 330, 82, 84, "Germany");
	CAR("rs6", "Audi", "RS6 Avant", 2022, EVehicleClass::Sports, 115000, 305, 68, 74, "Germany");
	CAR("rs3", "Audi", "RS3", 2022, EVehicleClass::Sports, 65000, 280, 62, 82, "Germany");
	CAR("quattro_s1", "Audi", "Quattro S1", 1985, EVehicleClass::Rally, 80000, 220, 38, 78, "Germany");
	CAR("etron_gt", "Audi", "e-tron GT RS", 2022, EVehicleClass::Sports, 145000, 250, 72, 80, "Germany");

	// === PORSCHE ===
	CAR("911_turbo", "Porsche", "911 Turbo S", 2022, EVehicleClass::Super, 205000, 330, 84, 90, "Germany");
	CAR("911_gt3", "Porsche", "911 GT3 RS", 2022, EVehicleClass::Super, 190000, 318, 82, 94, "Germany");
	CAR("carrera_gt", "Porsche", "Carrera GT", 2005, EVehicleClass::Super, 950000, 334, 78, 82, "Germany");
	CAR("cayman", "Porsche", "718 Cayman GT4", 2022, EVehicleClass::Sports, 105000, 302, 72, 90, "Germany");
	CAR("959", "Porsche", "959", 1987, EVehicleClass::Classic, 1500000, 315, 65, 78, "Germany");

	// === FERRARI ===
	CAR("f40", "Ferrari", "F40", 1987, EVehicleClass::Super, 1800000, 324, 78, 75, "Italy");
	CAR("f50", "Ferrari", "F50", 1995, EVehicleClass::Super, 2200000, 325, 76, 78, "Italy");
	CAR("enzo", "Ferrari", "Enzo", 2003, EVehicleClass::Super, 2800000, 350, 82, 80, "Italy");
	CAR("laferrari", "Ferrari", "LaFerrari", 2015, EVehicleClass::Hypercar, 3500000, 352, 88, 84, "Italy");
	CAR("488_pista", "Ferrari", "488 Pista", 2019, EVehicleClass::Super, 330000, 340, 84, 86, "Italy");
	CAR("sf90", "Ferrari", "SF90 Stradale", 2022, EVehicleClass::Hypercar, 500000, 340, 90, 86, "Italy");
	CAR("testarossa", "Ferrari", "Testarossa", 1984, EVehicleClass::Classic, 150000, 290, 58, 68, "Italy");

	// === LAMBORGHINI ===
	CAR("countach", "Lamborghini", "Countach LP5000", 1985, EVehicleClass::Super, 250000, 295, 62, 65, "Italy");
	CAR("diablo", "Lamborghini", "Diablo SV", 1998, EVehicleClass::Super, 200000, 325, 68, 68, "Italy");
	CAR("murcielago", "Lamborghini", "Murciélago LP640", 2007, EVehicleClass::Super, 280000, 340, 72, 70, "Italy");
	CAR("aventador", "Lamborghini", "Aventador SVJ", 2020, EVehicleClass::Super, 500000, 350, 84, 76, "Italy");
	CAR("huracan", "Lamborghini", "Huracán STO", 2022, EVehicleClass::Super, 330000, 325, 82, 84, "Italy");
	CAR("urus", "Lamborghini", "Urus", 2022, EVehicleClass::SUV, 230000, 305, 72, 70, "Italy");

	// === ASTON MARTIN ===
	CAR("db5", "Aston Martin", "DB5", 1964, EVehicleClass::Classic, 650000, 230, 40, 62, "UK");
	CAR("db11", "Aston Martin", "DB11", 2022, EVehicleClass::Sports, 220000, 322, 72, 78, "UK");
	CAR("valkyrie", "Aston Martin", "Valkyrie", 2023, EVehicleClass::Hypercar, 3200000, 362, 94, 88, "UK");

	// === McLAREN ===
	CAR("f1", "McLaren", "F1", 1994, EVehicleClass::Super, 18000000, 386, 88, 78, "UK");
	CAR("p1", "McLaren", "P1", 2015, EVehicleClass::Hypercar, 1500000, 350, 86, 84, "UK");
	CAR("senna", "McLaren", "Senna", 2019, EVehicleClass::Hypercar, 1000000, 340, 88, 92, "UK");
	CAR("720s", "McLaren", "720S", 2022, EVehicleClass::Super, 300000, 341, 82, 88, "UK");
	CAR("artura", "McLaren", "Artura", 2023, EVehicleClass::Super, 230000, 330, 78, 86, "UK");

	// === BUGATTI ===
	CAR("veyron", "Bugatti", "Veyron Super Sport", 2010, EVehicleClass::Hypercar, 2000000, 431, 88, 78, "France");
	CAR("chiron", "Bugatti", "Chiron Super Sport", 2022, EVehicleClass::Hypercar, 3900000, 490, 92, 80, "France");
	CAR("divo", "Bugatti", "Divo", 2020, EVehicleClass::Hypercar, 5800000, 380, 86, 92, "France");

	// === KOENIGSEGG ===
	CAR("agera", "Koenigsegg", "Agera RS", 2017, EVehicleClass::Hypercar, 2500000, 457, 92, 82, "Sweden");
	CAR("jesko", "Koenigsegg", "Jesko Absolut", 2023, EVehicleClass::Hypercar, 3000000, 532, 94, 84, "Sweden");
	CAR("gemera", "Koenigsegg", "Gemera", 2023, EVehicleClass::Hypercar, 1700000, 400, 88, 80, "Sweden");

	// === PAGANI ===
	CAR("zonda", "Pagani", "Zonda Cinque", 2009, EVehicleClass::Hypercar, 1500000, 350, 80, 82, "Italy");
	CAR("huayra", "Pagani", "Huayra BC", 2020, EVehicleClass::Hypercar, 2800000, 370, 84, 84, "Italy");

	// === LOTUS ===
	CAR("esprit", "Lotus", "Esprit V8", 1996, EVehicleClass::Sports, 35000, 275, 55, 80, "UK");
	CAR("elise", "Lotus", "Elise", 2004, EVehicleClass::Sports, 25000, 240, 52, 92, "UK");
	CAR("evora", "Lotus", "Evora GT", 2020, EVehicleClass::Sports, 95000, 303, 65, 86, "UK");
	CAR("emira", "Lotus", "Emira", 2023, EVehicleClass::Sports, 82000, 290, 62, 88, "UK");

	// === JAGUAR ===
	CAR("e_type", "Jaguar", "E-Type Series 3", 1974, EVehicleClass::Classic, 70000, 240, 38, 62, "UK");
	CAR("f_type", "Jaguar", "F-Type R", 2022, EVehicleClass::Sports, 105000, 300, 72, 78, "UK");
	CAR("xj220", "Jaguar", "XJ220", 1992, EVehicleClass::Super, 250000, 341, 72, 70, "UK");

	// === TESLA ===
	CAR("model_s_plaid", "Tesla", "Model S Plaid", 2023, EVehicleClass::Sedan, 110000, 322, 90, 72, "USA");
	CAR("roadster", "Tesla", "Roadster", 2023, EVehicleClass::Super, 200000, 402, 88, 80, "USA");
	CAR("cybertruck", "Tesla", "Cybertruck", 2024, EVehicleClass::Truck, 70000, 210, 48, 50, "USA");
	CAR("model_3_perf", "Tesla", "Model 3 Performance", 2023, EVehicleClass::Sedan, 55000, 261, 72, 78, "USA");
	CAR("model_x_plaid", "Tesla", "Model X Plaid", 2023, EVehicleClass::SUV, 120000, 262, 68, 60, "USA");
	CAR("model_y", "Tesla", "Model Y Performance", 2023, EVehicleClass::SUV, 58000, 250, 65, 65, "USA");
	CAR("tesla_semi", "Tesla", "Semi", 2023, EVehicleClass::Truck, 180000, 105, 25, 30, "USA");

	// === ROLLS-ROYCE ===
	CAR("phantom", "Rolls-Royce", "Phantom VIII", 2023, EVehicleClass::Sedan, 460000, 250, 42, 60, "UK");
	CAR("ghost", "Rolls-Royce", "Ghost", 2023, EVehicleClass::Sedan, 340000, 250, 48, 65, "UK");
	CAR("cullinan", "Rolls-Royce", "Cullinan", 2023, EVehicleClass::SUV, 350000, 250, 45, 55, "UK");
	CAR("wraith", "Rolls-Royce", "Wraith Black Badge", 2022, EVehicleClass::Sports, 380000, 250, 50, 62, "UK");
	CAR("dawn", "Rolls-Royce", "Dawn", 2022, EVehicleClass::Sports, 370000, 250, 48, 64, "UK");
	CAR("silver_ghost", "Rolls-Royce", "Silver Ghost", 1914, EVehicleClass::Classic, 2500000, 85, 10, 30, "UK");
	CAR("phantom_ii", "Rolls-Royce", "Phantom II Continental", 1931, EVehicleClass::Classic, 800000, 120, 12, 35, "UK");

	// === BENTLEY ===
	CAR("continental_gt", "Bentley", "Continental GT Speed", 2023, EVehicleClass::Sports, 280000, 335, 72, 72, "UK");
	CAR("flying_spur", "Bentley", "Flying Spur", 2023, EVehicleClass::Sedan, 215000, 333, 68, 68, "UK");
	CAR("bentayga", "Bentley", "Bentayga Speed", 2023, EVehicleClass::SUV, 260000, 306, 62, 58, "UK");
	CAR("blower", "Bentley", "4.5L Blower", 1929, EVehicleClass::Classic, 2200000, 140, 15, 30, "UK");
	CAR("speed_8", "Bentley", "Speed 8 Le Mans", 2003, EVehicleClass::Super, 3000000, 330, 80, 75, "UK");
	CAR("mulsanne", "Bentley", "Mulsanne", 2020, EVehicleClass::Sedan, 310000, 296, 55, 62, "UK");

	// === ALFA ROMEO ===
	CAR("giulia_qv", "Alfa Romeo", "Giulia Quadrifoglio", 2023, EVehicleClass::Sports, 80000, 307, 72, 86, "Italy");
	CAR("stelvio_qv", "Alfa Romeo", "Stelvio Quadrifoglio", 2023, EVehicleClass::SUV, 88000, 283, 62, 72, "Italy");
	CAR("4c_spider", "Alfa Romeo", "4C Spider", 2020, EVehicleClass::Sports, 68000, 257, 58, 92, "Italy");
	CAR("8c_competizione", "Alfa Romeo", "8C Competizione", 2008, EVehicleClass::Super, 250000, 292, 68, 78, "Italy");
	CAR("tipo_33", "Alfa Romeo", "Tipo 33 Stradale", 1967, EVehicleClass::Classic, 8000000, 260, 52, 75, "Italy");
	CAR("disco_volante", "Alfa Romeo", "Disco Volante Touring", 2013, EVehicleClass::Sports, 500000, 290, 62, 82, "Italy");
	CAR("gz_tz2", "Alfa Romeo", "Giulia TZ2", 1965, EVehicleClass::Classic, 1800000, 245, 50, 80, "Italy");

	// === MASERATI ===
	CAR("mc20", "Maserati", "MC20", 2023, EVehicleClass::Super, 215000, 325, 76, 82, "Italy");
	CAR("granturismo", "Maserati", "GranTurismo Trofeo", 2023, EVehicleClass::Sports, 180000, 320, 68, 76, "Italy");
	CAR("ghibli_trofeo", "Maserati", "Ghibli Trofeo", 2022, EVehicleClass::Sedan, 115000, 326, 70, 70, "Italy");
	CAR("mc12", "Maserati", "MC12", 2005, EVehicleClass::Hypercar, 2500000, 330, 74, 78, "Italy");
	CAR("birdcage", "Maserati", "Tipo 61 Birdcage", 1960, EVehicleClass::Classic, 3500000, 250, 45, 72, "Italy");
	CAR("bora", "Maserati", "Bora", 1973, EVehicleClass::Classic, 120000, 270, 50, 68, "Italy");

	// === CADILLAC ===
	CAR("ct5_v_blackwing", "Cadillac", "CT5-V Blackwing", 2023, EVehicleClass::Sports, 92000, 322, 72, 76, "USA");
	CAR("escalade_v", "Cadillac", "Escalade-V", 2023, EVehicleClass::SUV, 150000, 200, 55, 48, "USA");
	CAR("eldorado_59", "Cadillac", "Eldorado Biarritz", 1959, EVehicleClass::Classic, 80000, 170, 28, 40, "USA");
	CAR("ct6_v", "Cadillac", "CT6-V", 2020, EVehicleClass::Sedan, 95000, 322, 68, 68, "USA");
	CAR("celestiq", "Cadillac", "Celestiq", 2024, EVehicleClass::Sedan, 340000, 250, 55, 65, "USA");

	// === LEXUS (add more) ===
	CAR("lc500", "Lexus", "LC 500", 2023, EVehicleClass::Sports, 95000, 270, 58, 78, "Japan");
	CAR("rc_f", "Lexus", "RC F Track Edition", 2023, EVehicleClass::Sports, 98000, 270, 62, 76, "Japan");
	CAR("is500", "Lexus", "IS 500 F Sport", 2023, EVehicleClass::Sports, 62000, 270, 60, 74, "Japan");

	// === ACURA ===
	CAR("nsx_type_s", "Acura", "NSX Type S", 2022, EVehicleClass::Super, 170000, 308, 80, 86, "Japan");
	CAR("integra_s", "Acura", "Integra Type S", 2024, EVehicleClass::Sports, 52000, 267, 58, 84, "Japan");
	CAR("tlx_s", "Acura", "TLX Type S", 2023, EVehicleClass::Sports, 55000, 270, 55, 76, "Japan");

	// === INFINITI ===
	CAR("q60_red_sport", "Infiniti", "Q60 Red Sport 400", 2022, EVehicleClass::Sports, 58000, 250, 55, 74, "Japan");
	CAR("g35", "Infiniti", "G35 Coupe", 2005, EVehicleClass::Sports, 8000, 250, 48, 72, "Japan");

	// === GENESIS ===
	CAR("g70_shooting", "Genesis", "G70 Shooting Brake", 2023, EVehicleClass::Sports, 45000, 270, 55, 76, "Korea");
	CAR("g90", "Genesis", "G90", 2023, EVehicleClass::Sedan, 88000, 250, 48, 68, "Korea");

	// === HYUNDAI N ===
	CAR("i30n", "Hyundai", "i30 N", 2023, EVehicleClass::Sports, 45000, 250, 52, 78, "Korea");
	CAR("elantra_n", "Hyundai", "Elantra N", 2023, EVehicleClass::Sports, 34000, 260, 50, 76, "Korea");
	CAR("ioniq5_n", "Hyundai", "Ioniq 5 N", 2024, EVehicleClass::Sports, 66000, 260, 65, 74, "Korea");
	CAR("veloster_n", "Hyundai", "Veloster N", 2022, EVehicleClass::Sports, 33000, 250, 50, 78, "Korea");

	// === KIA ===
	CAR("stinger_gt", "Kia", "Stinger GT", 2023, EVehicleClass::Sports, 53000, 270, 55, 74, "Korea");
	CAR("ev6_gt", "Kia", "EV6 GT", 2023, EVehicleClass::Sports, 62000, 260, 68, 72, "Korea");

	// === VOLKSWAGEN ===
	CAR("golf_r", "Volkswagen", "Golf R", 2023, EVehicleClass::Sports, 45000, 270, 55, 82, "Germany");
	CAR("golf_gti", "Volkswagen", "Golf GTI Clubsport", 2023, EVehicleClass::Sports, 38000, 250, 50, 84, "Germany");
	CAR("scirocco_r", "Volkswagen", "Scirocco R", 2015, EVehicleClass::Sports, 20000, 250, 48, 80, "Germany");
	CAR("corrado_vr6", "Volkswagen", "Corrado VR6", 1995, EVehicleClass::Sports, 12000, 225, 42, 78, "Germany");
	CAR("beetle_turbo", "Volkswagen", "Beetle Turbo", 2019, EVehicleClass::Compact, 22000, 210, 42, 72, "Germany");
	CAR("passat_r36", "Volkswagen", "Passat R36", 2008, EVehicleClass::Sedan, 18000, 250, 48, 68, "Germany");
	CAR("touareg_v10", "Volkswagen", "Touareg V10 TDI", 2008, EVehicleClass::SUV, 15000, 230, 38, 50, "Germany");
	CAR("id_buzz", "Volkswagen", "ID. Buzz", 2024, EVehicleClass::SUV, 60000, 160, 42, 55, "Germany");

	// === OPEL / VAUXHALL ===
	CAR("astra_opc", "Opel", "Astra OPC", 2018, EVehicleClass::Sports, 28000, 270, 48, 78, "Germany");
	CAR("corsa_opc", "Opel", "Corsa OPC", 2018, EVehicleClass::Compact, 22000, 240, 45, 80, "Germany");
	CAR("insignia_opc", "Opel", "Insignia OPC", 2017, EVehicleClass::Sedan, 25000, 270, 50, 70, "Germany");
	CAR("manta_400", "Opel", "Manta 400", 1982, EVehicleClass::Rally, 50000, 210, 38, 72, "Germany");
	CAR("calibra_turbo", "Opel", "Calibra Turbo 4x4", 1992, EVehicleClass::Sports, 10000, 245, 45, 76, "Germany");

	// === RENAULT ===
	CAR("megane_rs", "Renault", "Mégane RS Trophy-R", 2020, EVehicleClass::Sports, 45000, 262, 50, 88, "France");
	CAR("clio_v6", "Renault", "Clio V6", 2003, EVehicleClass::Sports, 35000, 235, 48, 82, "France");
	CAR("r5_turbo", "Renault", "5 Turbo", 1982, EVehicleClass::Rally, 60000, 200, 42, 76, "France");
	CAR("alpine_a110", "Alpine", "A110", 2023, EVehicleClass::Sports, 65000, 275, 58, 90, "France");
	CAR("alpine_a310", "Alpine", "A310", 1980, EVehicleClass::Classic, 25000, 215, 38, 74, "France");
	CAR("twingo", "Renault", "Twingo RS", 2015, EVehicleClass::Compact, 12000, 195, 38, 80, "France");

	// === PEUGEOT ===
	CAR("205_t16", "Peugeot", "205 T16", 1985, EVehicleClass::Rally, 280000, 220, 55, 78, "France");
	CAR("208_gti", "Peugeot", "208 GTi", 2018, EVehicleClass::Compact, 18000, 230, 44, 80, "France");
	CAR("508_pse", "Peugeot", "508 PSE", 2023, EVehicleClass::Sedan, 55000, 250, 52, 72, "France");
	CAR("rcz_r", "Peugeot", "RCZ R", 2015, EVehicleClass::Sports, 25000, 250, 48, 76, "France");
	CAR("106_rallye", "Peugeot", "106 Rallye", 1998, EVehicleClass::Compact, 10000, 195, 38, 82, "France");

	// === CITROEN ===
	CAR("ds_23", "Citroën", "DS 23 Pallas", 1973, EVehicleClass::Classic, 22000, 175, 22, 58, "France");
	CAR("saxo_vts", "Citroën", "Saxo VTS", 2002, EVehicleClass::Compact, 5000, 195, 38, 80, "France");

	// === VOLVO ===
	CAR("v60_polestar", "Volvo", "V60 Polestar Engineered", 2023, EVehicleClass::Sports, 68000, 250, 48, 72, "Sweden");
	CAR("850_t5r", "Volvo", "850 T-5R", 1995, EVehicleClass::Sports, 12000, 240, 42, 68, "Sweden");
	CAR("xc90", "Volvo", "XC90 T8", 2023, EVehicleClass::SUV, 75000, 230, 42, 58, "Sweden");
	CAR("polestar_1", "Polestar", "1", 2021, EVehicleClass::Sports, 155000, 250, 55, 78, "Sweden");
	CAR("polestar_2", "Polestar", "2 BST", 2023, EVehicleClass::Sports, 75000, 205, 58, 74, "Sweden");

	// === SAAB ===
	CAR("saab_900_turbo", "Saab", "900 Turbo", 1985, EVehicleClass::Classic, 15000, 200, 35, 74, "Sweden");
	CAR("93_viggen", "Saab", "9-3 Viggen", 2002, EVehicleClass::Sports, 10000, 260, 48, 72, "Sweden");
	CAR("saab_95_aero", "Saab", "9-5 Aero", 2008, EVehicleClass::Sedan, 8000, 260, 50, 68, "Sweden");

	// === JEEP ===
	CAR("wrangler_392", "Jeep", "Wrangler Rubicon 392", 2023, EVehicleClass::SUV, 85000, 180, 48, 45, "USA");
	CAR("grand_cherokee_trackhawk", "Jeep", "Grand Cherokee Trackhawk", 2022, EVehicleClass::SUV, 88000, 290, 62, 50, "USA");
	CAR("gladiator", "Jeep", "Gladiator Mojave", 2023, EVehicleClass::Truck, 55000, 175, 35, 42, "USA");

	// === LAND ROVER ===
	CAR("defender_v8", "Land Rover", "Defender V8", 2023, EVehicleClass::SUV, 110000, 240, 48, 52, "UK");
	CAR("range_rover_svr", "Land Rover", "Range Rover Sport SVR", 2023, EVehicleClass::SUV, 130000, 283, 55, 50, "UK");
	CAR("evoque", "Land Rover", "Range Rover Evoque", 2023, EVehicleClass::SUV, 48000, 225, 42, 60, "UK");
	CAR("series_3", "Land Rover", "Series III", 1975, EVehicleClass::Classic, 15000, 95, 12, 30, "UK");

	// === MINI ===
	CAR("mini_cooper_s", "Mini", "Cooper S JCW", 2023, EVehicleClass::Compact, 38000, 240, 50, 84, "UK");
	CAR("mini_gp3", "Mini", "JCW GP", 2021, EVehicleClass::Sports, 45000, 265, 52, 82, "UK");
	CAR("classic_mini", "Mini", "Cooper 1275S", 1965, EVehicleClass::Classic, 25000, 145, 28, 78, "UK");

	// === FIAT ===
	CAR("fiat_500_abarth", "Fiat", "500 Abarth 695", 2023, EVehicleClass::Compact, 38000, 225, 48, 82, "Italy");
	CAR("124_spider_abarth", "Fiat", "124 Spider Abarth", 2020, EVehicleClass::Sports, 32000, 230, 48, 84, "Italy");
	CAR("x19", "Fiat", "X1/9", 1982, EVehicleClass::Classic, 8000, 170, 32, 82, "Italy");

	// === LANCIA ===
	CAR("delta_integrale", "Lancia", "Delta HF Integrale Evo II", 1993, EVehicleClass::Rally, 60000, 230, 50, 82, "Italy");
	CAR("stratops_hf", "Lancia", "Stratos HF", 1975, EVehicleClass::Rally, 400000, 230, 55, 80, "Italy");
	CAR("037_rally", "Lancia", "037 Rally", 1983, EVehicleClass::Rally, 350000, 220, 48, 78, "Italy");
	CAR("fulvia_hf", "Lancia", "Fulvia HF", 1970, EVehicleClass::Rally, 25000, 180, 32, 76, "Italy");

	// === DE TOMASO ===
	CAR("pantera_gts", "De Tomaso", "Pantera GTS", 1974, EVehicleClass::Classic, 80000, 275, 55, 65, "Italy");
	CAR("mangusta", "De Tomaso", "Mangusta", 1970, EVehicleClass::Classic, 65000, 250, 48, 62, "Italy");

	// === DELOREAN ===
	CAR("dmc12", "DeLorean", "DMC-12", 1981, EVehicleClass::Classic, 55000, 195, 35, 62, "USA");

	// === SHELBY ===
	CAR("cobra_427", "Shelby", "Cobra 427", 1965, EVehicleClass::Classic, 1400000, 265, 65, 55, "USA");
	CAR("gt350r", "Shelby", "GT350R", 1965, EVehicleClass::Muscle, 350000, 240, 48, 65, "USA");
	CAR("daytona_coupe", "Shelby", "Cobra Daytona Coupe", 1964, EVehicleClass::Classic, 8000000, 310, 60, 68, "USA");
	CAR("gt500_super_snake", "Shelby", "GT500 Super Snake", 2022, EVehicleClass::Muscle, 130000, 300, 72, 62, "USA");

	// === HOLDEN ===
	CAR("commodore_vf", "Holden", "Commodore VF SS-V Redline", 2017, EVehicleClass::Muscle, 35000, 270, 52, 65, "Australia");
	CAR("maloo_r8", "Holden", "Maloo R8 LSA", 2017, EVehicleClass::Truck, 45000, 280, 55, 55, "Australia");
	CAR("monaro_cv8", "Holden", "Monaro CV8", 2005, EVehicleClass::Muscle, 25000, 260, 48, 62, "Australia");

	// === TVR ===
	CAR("tvr_sagaris", "TVR", "Sagaris", 2006, EVehicleClass::Sports, 50000, 297, 58, 78, "UK");
	CAR("tvr_tuscan", "TVR", "Tuscan Speed Six", 2005, EVehicleClass::Sports, 35000, 290, 55, 76, "UK");
	CAR("tvr_cerbera", "TVR", "Cerbera 4.5", 2003, EVehicleClass::Sports, 28000, 290, 52, 74, "UK");
	CAR("tvr_griffith", "TVR", "Griffith 500", 2000, EVehicleClass::Sports, 22000, 270, 50, 72, "UK");

	// === MORGAN ===
	CAR("morgan_plus_8", "Morgan", "Plus 8", 2018, EVehicleClass::Sports, 80000, 250, 45, 72, "UK");
	CAR("morgan_3_wheeler", "Morgan", "3 Wheeler", 2020, EVehicleClass::Classic, 45000, 185, 40, 68, "UK");
	CAR("aero_8", "Morgan", "Aero 8", 2018, EVehicleClass::Sports, 90000, 275, 50, 70, "UK");

	// === NOBLE ===
	CAR("noble_m600", "Noble", "M600", 2020, EVehicleClass::Super, 250000, 362, 78, 76, "UK");
	CAR("m12_gto", "Noble", "M12 GTO-3R", 2005, EVehicleClass::Sports, 45000, 298, 62, 80, "UK");

	// === RIMAC ===
	CAR("rimac_nevera", "Rimac", "Nevera", 2023, EVehicleClass::Hypercar, 2400000, 412, 96, 82, "Croatia");
	CAR("rimac_concept_one", "Rimac", "Concept_One", 2015, EVehicleClass::Hypercar, 1000000, 355, 88, 78, "Croatia");
	CAR("rimac_nevera_r", "Rimac", "Nevera R", 2025, EVehicleClass::Hypercar, 2500000, 412, 96, 84, "Croatia");

	// === HENNESSEY ===
	CAR("venom_gt", "Hennessey", "Venom GT", 2015, EVehicleClass::Hypercar, 1200000, 435, 90, 78, "USA");
	CAR("venom_f5", "Hennessey", "Venom F5", 2024, EVehicleClass::Hypercar, 2100000, 500, 94, 76, "USA");

	// === SSC ===
	CAR("ssc_tuatara", "SSC", "Tuatara", 2023, EVehicleClass::Hypercar, 1900000, 508, 92, 74, "USA");
	CAR("ultimate_aero", "SSC", "Ultimate Aero TT", 2010, EVehicleClass::Hypercar, 650000, 412, 85, 70, "USA");

	// === SPYKER ===
	CAR("c8_spyder", "Spyker", "C8 Spyder", 2009, EVehicleClass::Sports, 250000, 300, 55, 72, "Netherlands");
	CAR("c8_laviolette", "Spyker", "C8 Laviolette", 2007, EVehicleClass::Sports, 230000, 300, 52, 70, "Netherlands");

	// === GINETTA ===
	CAR("ginetta_g60", "Ginetta", "G60-LT-P1", 2020, EVehicleClass::Super, 300000, 330, 75, 80, "UK");
	CAR("ginetta_g40", "Ginetta", "G40R", 2020, EVehicleClass::Sports, 45000, 240, 52, 84, "UK");

	// === W MOTORS ===
	CAR("lykan_hypersport", "W Motors", "Lykan HyperSport", 2016, EVehicleClass::Hypercar, 3400000, 395, 82, 72, "UAE");
	CAR("fenyr_supersport", "W Motors", "Fenyr SuperSport", 2019, EVehicleClass::Hypercar, 1800000, 400, 84, 74, "UAE");

	// === MAZDA (add more) ===
	CAR("rx7_spirit_r", "Mazda", "RX-7 Spirit R Type A", 2002, EVehicleClass::Sports, 45000, 260, 58, 84, "Japan");
	CAR("rx3", "Mazda", "RX-3", 1974, EVehicleClass::Classic, 18000, 195, 32, 62, "Japan");
	CAR("cosmo_sport", "Mazda", "Cosmo Sport 110S", 1970, EVehicleClass::Classic, 35000, 185, 28, 60, "Japan");
	CAR("mazda3_turbo", "Mazda", "Mazda3 Turbo", 2023, EVehicleClass::Compact, 33000, 240, 48, 78, "Japan");

	// === MITSUBISHI (add more) ===
	CAR("evo_ix_wagon", "Mitsubishi", "Lancer Evolution IX Wagon", 2006, EVehicleClass::Sports, 18000, 250, 48, 72, "Japan");
	CAR("starion", "Mitsubishi", "Starion Turbo", 1987, EVehicleClass::Sports, 10000, 220, 38, 68, "Japan");
	CAR("fq400", "Mitsubishi", "Lancer Evolution VIII FQ-400", 2005, EVehicleClass::Sports, 35000, 280, 62, 78, "Japan");

	// === SUBARU (add more) ===
	CAR("22b_sti", "Subaru", "Impreza 22B STI", 1998, EVehicleClass::Rally, 90000, 240, 58, 86, "Japan");
	CAR("bugeye_sti", "Subaru", "Impreza WRX STI (Bugeye)", 2002, EVehicleClass::Sports, 15000, 240, 52, 80, "Japan");
	CAR("forester_sti", "Subaru", "Forester STI", 2005, EVehicleClass::SUV, 18000, 230, 48, 72, "Japan");
	CAR("legacy_gtb", "Subaru", "Legacy GT-B", 2003, EVehicleClass::Sedan, 8000, 250, 45, 70, "Japan");
	CAR("svx", "Subaru", "SVX", 1996, EVehicleClass::Sports, 8000, 230, 40, 68, "Japan");

	// === NISSAN (add more) ===
	CAR("r32_gtr", "Nissan", "Skyline GT-R R32", 1990, EVehicleClass::Sports, 45000, 250, 62, 78, "Japan");
	CAR("r33_gtr", "Nissan", "Skyline GT-R R33", 1996, EVehicleClass::Sports, 35000, 250, 60, 76, "Japan");
	CAR("pulsar_gti_r", "Nissan", "Pulsar GTI-R", 1992, EVehicleClass::Rally, 15000, 230, 45, 74, "Japan");
	CAR("300zx_z32", "Nissan", "300ZX Twin Turbo", 1994, EVehicleClass::Sports, 15000, 250, 50, 72, "Japan");
	CAR("stagea_260rs", "Nissan", "Stagea 260RS Autech", 1998, EVehicleClass::Sports, 18000, 250, 48, 64, "Japan");

	// === TOYOTA (add more) ===
	CAR("soarer_25gt", "Toyota", "Soarer 2.5GT-T", 1995, EVehicleClass::Sports, 10000, 240, 42, 68, "Japan");
	CAR("chaser_jzx100", "Toyota", "Chaser Tourer V", 1998, EVehicleClass::Sports, 20000, 260, 50, 70, "Japan");
	CAR("aristo_v300", "Toyota", "Aristo V300", 2000, EVehicleClass::Sedan, 12000, 250, 48, 65, "Japan");
	CAR("century", "Toyota", "Century", 2023, EVehicleClass::Sedan, 180000, 200, 38, 58, "Japan");
	CAR("tundra_trd", "Toyota", "Tundra TRD Pro", 2023, EVehicleClass::Truck, 62000, 180, 38, 48, "USA");
	CAR("tacoma_trd", "Toyota", "Tacoma TRD Pro", 2023, EVehicleClass::Truck, 48000, 175, 35, 45, "USA");
	CAR("prius_prime", "Toyota", "Prius Prime", 2023, EVehicleClass::Compact, 33000, 180, 35, 65, "Japan");
	CAR("yaris_gr", "Toyota", "GR Yaris", 2023, EVehicleClass::Rally, 45000, 230, 52, 84, "Japan");
	CAR("corolla_gr", "Toyota", "GR Corolla", 2023, EVehicleClass::Sports, 38000, 230, 50, 80, "Japan");
	CAR("2000gt", "Toyota", "2000GT", 1968, EVehicleClass::Classic, 950000, 220, 40, 72, "Japan");

	// === HONDA (add more) ===
	CAR("nsx_na1", "Honda", "NSX (NA1)", 1992, EVehicleClass::Sports, 50000, 272, 58, 82, "Japan");
	CAR("civic_ek9", "Honda", "Civic Type R (EK9)", 1999, EVehicleClass::Sports, 22000, 230, 48, 84, "Japan");
	CAR("civic_fd2", "Honda", "Civic Type R (FD2)", 2008, EVehicleClass::Sports, 20000, 240, 50, 82, "Japan");
	CAR("accord_euro_r", "Honda", "Accord Euro R", 2005, EVehicleClass::Sports, 12000, 235, 45, 74, "Japan");
	CAR("prelude_vtec", "Honda", "Prelude VTEC", 1999, EVehicleClass::Sports, 8000, 230, 42, 76, "Japan");
	CAR("beat", "Honda", "Beat", 1992, EVehicleClass::Compact, 8000, 140, 30, 78, "Japan");
	CAR("cbr600", "Honda", "CBR600RR", 2020, EVehicleClass::Motorcycle, 12000, 275, 88, 92, "Japan");
	CAR("goldwing", "Honda", "Gold Wing", 2023, EVehicleClass::Motorcycle, 28000, 180, 40, 55, "Japan");

	// === SUZUKI ===
	CAR("swift_sport", "Suzuki", "Swift Sport", 2023, EVehicleClass::Compact, 22000, 210, 42, 82, "Japan");
	CAR("jimny", "Suzuki", "Jimny", 2023, EVehicleClass::SUV, 25000, 145, 28, 55, "Japan");
	CAR("gsxr1000", "Suzuki", "GSX-R1000R", 2023, EVehicleClass::Motorcycle, 18000, 299, 92, 90, "Japan");
	CAR("hayabusa", "Suzuki", "Hayabusa", 2023, EVehicleClass::Motorcycle, 19000, 312, 88, 78, "Japan");
	CAR("cappuccino", "Suzuki", "Cappuccino", 1995, EVehicleClass::Compact, 6000, 160, 30, 82, "Japan");

	// === DAIHATSU ===
	CAR("copen", "Daihatsu", "Copen", 2023, EVehicleClass::Compact, 20000, 180, 32, 78, "Japan");
	CAR("charade_gtti", "Daihatsu", "Charade GTti", 1988, EVehicleClass::Compact, 5000, 185, 35, 76, "Japan");

	// === ISUZU ===
	CAR("dmax_at35", "Isuzu", "D-Max Arctic Trucks AT35", 2023, EVehicleClass::Truck, 48000, 175, 32, 42, "Japan");
	CAR("vehicross", "Isuzu", "VehiCROSS", 2001, EVehicleClass::SUV, 10000, 175, 32, 52, "Japan");
	CAR("piazza_turbo", "Isuzu", "Piazza Turbo", 1988, EVehicleClass::Sports, 8000, 210, 38, 68, "Japan");

	// === DODGE (add more) ===
	CAR("ram_trx", "Dodge", "Ram 1500 TRX", 2023, EVehicleClass::Truck, 85000, 190, 55, 42, "USA");
	CAR("durango_hellcat", "Dodge", "Durango SRT Hellcat", 2023, EVehicleClass::SUV, 90000, 290, 58, 48, "USA");
	CAR("ram_srt10", "Dodge", "Ram SRT-10", 2005, EVehicleClass::Truck, 25000, 245, 52, 40, "USA");
	CAR("neon_srt4", "Dodge", "Neon SRT-4", 2005, EVehicleClass::Compact, 10000, 245, 52, 72, "USA");

	// === FORD (add more) ===
	CAR("mustang_dark_horse", "Ford", "Mustang Dark Horse", 2024, EVehicleClass::Muscle, 60000, 280, 68, 72, "USA");
	CAR("bronco_raptor", "Ford", "Bronco Raptor", 2023, EVehicleClass::SUV, 88000, 190, 48, 50, "USA");
	CAR("mustang_boss_429", "Ford", "Mustang Boss 429", 1969, EVehicleClass::Muscle, 250000, 220, 48, 55, "USA");
	CAR("sierra_rs500", "Ford", "Sierra RS500 Cosworth", 1987, EVehicleClass::Rally, 70000, 248, 45, 76, "UK");
	CAR("escort_mk1", "Ford", "Escort RS1600", 1972, EVehicleClass::Rally, 35000, 190, 38, 75, "UK");

	// === CHEVROLET (add more) ===
	CAR("corvette_z06", "Chevrolet", "Corvette C8 Z06", 2023, EVehicleClass::Super, 110000, 312, 82, 86, "USA");
	CAR("camaro_z28", "Chevrolet", "Camaro Z/28", 2015, EVehicleClass::Muscle, 50000, 280, 65, 80, "USA");
	CAR("chevelle_ss_70", "Chevrolet", "Chevelle SS 454 LS6", 1970, EVehicleClass::Muscle, 80000, 220, 58, 52, "USA");
	CAR("bel_air_57", "Chevrolet", "Bel Air", 1957, EVehicleClass::Classic, 40000, 160, 25, 40, "USA");
	CAR("nova_ss", "Chevrolet", "Nova SS 396", 1970, EVehicleClass::Muscle, 35000, 210, 52, 50, "USA");
	CAR("corvette_c2", "Chevrolet", "Corvette Sting Ray Split Window", 1963, EVehicleClass::Classic, 150000, 210, 40, 62, "USA");

	// === PONTIAC ===
	CAR("gto_judge", "Pontiac", "GTO Judge", 1969, EVehicleClass::Muscle, 50000, 220, 52, 50, "USA");
	CAR("firebird_trans_am", "Pontiac", "Firebird Trans Am", 1979, EVehicleClass::Muscle, 30000, 210, 42, 48, "USA");
	CAR("aztek", "Pontiac", "Aztek", 2005, EVehicleClass::SUV, 3000, 175, 28, 45, "USA");
	CAR("solstice_gxp", "Pontiac", "Solstice GXP", 2008, EVehicleClass::Sports, 12000, 230, 48, 76, "USA");

	// === PLYMOUTH ===
	CAR("superbird", "Plymouth", "Superbird", 1970, EVehicleClass::Muscle, 180000, 235, 55, 50, "USA");
	CAR("hemi_cuda", "Plymouth", "Hemi 'Cuda", 1970, EVehicleClass::Muscle, 120000, 220, 58, 48, "USA");
	CAR("roadrunner", "Plymouth", "Road Runner", 1970, EVehicleClass::Muscle, 40000, 210, 52, 48, "USA");

	// === OLDSMOBILE ===
	CAR("442_w30", "Oldsmobile", "442 W-30", 1970, EVehicleClass::Muscle, 40000, 210, 48, 48, "USA");
	CAR("cutlass_supreme", "Oldsmobile", "Cutlass Supreme", 1985, EVehicleClass::Sedan, 5000, 170, 28, 45, "USA");

	// === BUICK ===
	CAR("grand_national", "Buick", "Grand National GNX", 1987, EVehicleClass::Muscle, 65000, 200, 52, 55, "USA");
	CAR("riviera_65", "Buick", "Riviera", 1965, EVehicleClass::Classic, 25000, 185, 30, 50, "USA");

	// === LINCOLN ===
	CAR("continental_64", "Lincoln", "Continental", 1964, EVehicleClass::Classic, 35000, 175, 22, 45, "USA");
	CAR("aviator", "Lincoln", "Aviator Grand Touring", 2023, EVehicleClass::SUV, 75000, 230, 48, 55, "USA");

	// === CHRYSLER ===
	CAR("300c_srt8", "Chrysler", "300C SRT8", 2023, EVehicleClass::Muscle, 55000, 260, 55, 62, "USA");
	CAR("pt_cruiser_gt", "Chrysler", "PT Cruiser GT Turbo", 2005, EVehicleClass::Compact, 4000, 195, 35, 58, "USA");
	CAR("crossfire_srt6", "Chrysler", "Crossfire SRT-6", 2005, EVehicleClass::Sports, 10000, 255, 48, 72, "USA");

	// === GMC ===
	CAR("syclone", "GMC", "Syclone", 1991, EVehicleClass::Truck, 25000, 200, 48, 50, "USA");
	CAR("typhoon", "GMC", "Typhoon", 1993, EVehicleClass::SUV, 20000, 210, 48, 52, "USA");
	CAR("hummer_ev", "GMC", "Hummer EV", 2023, EVehicleClass::Truck, 110000, 170, 55, 45, "USA");

	// === SRT ===
	CAR("viper_gts", "SRT", "Viper GTS", 2017, EVehicleClass::Super, 95000, 331, 78, 72, "USA");

	// ==============================================
	// GENERATIONAL VARIANTS & SPECIAL EDITIONS
	// ==============================================

	// --- MERCEDES SLK / SLC (all generations) ---
	CAR("slk230_r170", "Mercedes", "SLK 230 Kompressor (R170)", 1998, EVehicleClass::Sports, 8000, 231, 42, 74, "Germany");
	CAR("slk320_r170", "Mercedes", "SLK 320 (R170)", 2000, EVehicleClass::Sports, 7000, 245, 45, 72, "Germany");
	CAR("slk32_amg", "Mercedes", "SLK 32 AMG", 2002, EVehicleClass::Sports, 12000, 275, 52, 74, "Germany");
	CAR("slk200_r171", "Mercedes", "SLK 200 Kompressor (R171)", 2005, EVehicleClass::Sports, 8000, 230, 44, 76, "Germany");
	CAR("slk350_r171", "Mercedes", "SLK 350 (R171)", 2005, EVehicleClass::Sports, 10000, 275, 52, 74, "Germany");
	CAR("slk55_amg_r171", "Mercedes", "SLK 55 AMG", 2006, EVehicleClass::Sports, 25000, 290, 58, 72, "Germany");
	CAR("slk200_r172", "Mercedes", "SLK 200 (R172)", 2012, EVehicleClass::Sports, 18000, 240, 48, 78, "Germany");
	CAR("slk350_r172", "Mercedes", "SLK 350 (R172)", 2012, EVehicleClass::Sports, 22000, 305, 55, 76, "Germany");
	CAR("slk55_amg_r172", "Mercedes", "SLK 55 AMG", 2013, EVehicleClass::Sports, 45000, 315, 62, 74, "Germany");
	CAR("slc43_amg", "Mercedes", "SLC 43 AMG", 2017, EVehicleClass::Sports, 45000, 305, 58, 76, "Germany");

	// --- MERCEDES SL (all generations) ---
	CAR("sl300_gullwing", "Mercedes", "300SL Gullwing", 1954, EVehicleClass::Classic, 1400000, 260, 38, 58, "Germany");
	CAR("sl190", "Mercedes", "190SL", 1958, EVehicleClass::Classic, 110000, 175, 25, 55, "Germany");
	CAR("sl280_pagoda", "Mercedes", "280SL Pagoda", 1968, EVehicleClass::Classic, 120000, 195, 30, 60, "Germany");
	CAR("sl450_r107", "Mercedes", "450SL (R107)", 1978, EVehicleClass::Classic, 20000, 205, 32, 58, "Germany");
	CAR("sl560_r107", "Mercedes", "560SL (R107)", 1988, EVehicleClass::Classic, 30000, 225, 38, 56, "Germany");
	CAR("sl500_r129", "Mercedes", "SL 500 (R129)", 1995, EVehicleClass::Sports, 12000, 250, 48, 65, "Germany");
	CAR("sl600_r129", "Mercedes", "SL 600 (R129)", 1998, EVehicleClass::Sports, 18000, 275, 52, 62, "Germany");
	CAR("sl55_amg_r230", "Mercedes", "SL 55 AMG", 2004, EVehicleClass::Sports, 25000, 310, 60, 68, "Germany");
	CAR("sl65_amg_black", "Mercedes", "SL 65 AMG Black Series", 2009, EVehicleClass::Super, 250000, 335, 72, 70, "Germany");
	CAR("sl63_amg_r231", "Mercedes", "SL 63 AMG", 2017, EVehicleClass::Sports, 90000, 310, 68, 72, "Germany");

	// --- BMW 3 SERIES & M3 (all generations) ---
	CAR("m3_e30_sport_evo", "BMW", "M3 E30 Sport Evolution", 1990, EVehicleClass::Classic, 120000, 248, 52, 84, "Germany");
	CAR("m3_e36", "BMW", "M3 E36", 1995, EVehicleClass::Sports, 25000, 255, 52, 78, "Germany");
	CAR("m3_e36_gt", "BMW", "M3 E36 GT", 1995, EVehicleClass::Sports, 60000, 265, 54, 80, "Germany");
	CAR("m3_e46_gtr", "BMW", "M3 E46 GTR", 2001, EVehicleClass::Sports, 200000, 295, 62, 82, "Germany");
	CAR("m3_e92", "BMW", "M3 E92 Coupé", 2008, EVehicleClass::Sports, 25000, 310, 62, 80, "Germany");
	CAR("m3_e92_gts", "BMW", "M3 E92 GTS", 2011, EVehicleClass::Sports, 120000, 320, 65, 82, "Germany");
	CAR("m3_f80", "BMW", "M3 F80", 2015, EVehicleClass::Sports, 40000, 305, 65, 82, "Germany");
	CAR("m3_csl", "BMW", "M3 E46 CSL", 2004, EVehicleClass::Sports, 70000, 282, 60, 86, "Germany");
	CAR("m4_g82_csl", "BMW", "M4 G82 CSL", 2023, EVehicleClass::Sports, 140000, 312, 68, 84, "Germany");
	CAR("330i_e46", "BMW", "330i ZHP (E46)", 2005, EVehicleClass::Sports, 12000, 250, 42, 78, "Germany");
	CAR("335i_e92", "BMW", "335i Coupé (E92)", 2009, EVehicleClass::Sports, 10000, 275, 48, 76, "Germany");

	// --- BMW M5 (all generations) ---
	CAR("m5_e28", "BMW", "M5 E28", 1986, EVehicleClass::Classic, 35000, 245, 42, 68, "Germany");
	CAR("m5_e34", "BMW", "M5 E34", 1992, EVehicleClass::Classic, 25000, 275, 45, 66, "Germany");
	CAR("m5_e39", "BMW", "M5 E39", 2000, EVehicleClass::Sports, 25000, 300, 52, 70, "Germany");
	CAR("m5_e60", "BMW", "M5 E60", 2007, EVehicleClass::Sports, 20000, 330, 62, 68, "Germany");
	CAR("m5_f10", "BMW", "M5 F10", 2015, EVehicleClass::Sports, 40000, 320, 65, 72, "Germany");
	CAR("m5_cs", "BMW", "M5 CS", 2022, EVehicleClass::Sports, 140000, 305, 72, 76, "Germany");
	CAR("m5_touring_e61", "BMW", "M5 Touring E61", 2008, EVehicleClass::Sports, 25000, 320, 60, 65, "Germany");

	// --- PORSCHE 911 (all generations) ---
	CAR("911_carrera_rs_73", "Porsche", "911 Carrera RS 2.7", 1973, EVehicleClass::Classic, 600000, 240, 45, 72, "Germany");
	CAR("911_turbo_930", "Porsche", "911 Turbo (930)", 1985, EVehicleClass::Classic, 150000, 260, 50, 62, "Germany");
	CAR("911_carrera_964_rs", "Porsche", "911 Carrera RS (964)", 1992, EVehicleClass::Classic, 200000, 260, 52, 78, "Germany");
	CAR("911_turbo_993", "Porsche", "911 Turbo (993)", 1996, EVehicleClass::Sports, 180000, 290, 58, 68, "Germany");
	CAR("911_gt2_993", "Porsche", "911 GT2 (993)", 1995, EVehicleClass::Super, 500000, 301, 62, 72, "Germany");
	CAR("911_gt3_996", "Porsche", "911 GT3 (996)", 2001, EVehicleClass::Sports, 50000, 306, 60, 82, "Germany");
	CAR("911_turbo_996", "Porsche", "911 Turbo (996)", 2003, EVehicleClass::Sports, 35000, 306, 58, 72, "Germany");
	CAR("911_gt3_997", "Porsche", "911 GT3 (997)", 2008, EVehicleClass::Sports, 70000, 312, 62, 84, "Germany");
	CAR("911_gt2rs_997", "Porsche", "911 GT2 RS (997)", 2011, EVehicleClass::Super, 300000, 330, 72, 76, "Germany");
	CAR("911_gt3_991", "Porsche", "911 GT3 (991)", 2015, EVehicleClass::Super, 140000, 315, 68, 88, "Germany");
	CAR("911_gt2rs_991", "Porsche", "911 GT2 RS (991)", 2018, EVehicleClass::Super, 300000, 340, 76, 80, "Germany");
	CAR("911_turbo_s_992", "Porsche", "911 Turbo S (992)", 2022, EVehicleClass::Super, 205000, 330, 78, 84, "Germany");
	CAR("911_sport_classic", "Porsche", "911 Sport Classic", 2023, EVehicleClass::Sports, 270000, 315, 65, 76, "Germany");
	CAR("911_dakar", "Porsche", "911 Dakar", 2023, EVehicleClass::Rally, 240000, 240, 58, 72, "Germany");
	CAR("911_st", "Porsche", "911 S/T", 2024, EVehicleClass::Sports, 300000, 300, 58, 86, "Germany");

	// --- PORSCHE OTHER MODELS ---
	CAR("boxster_s_986", "Porsche", "Boxster S (986)", 2003, EVehicleClass::Sports, 12000, 260, 48, 80, "Germany");
	CAR("boxster_spyder", "Porsche", "Boxster Spyder", 2016, EVehicleClass::Sports, 60000, 290, 55, 86, "Germany");
	CAR("cayman_s_987", "Porsche", "Cayman S (987)", 2008, EVehicleClass::Sports, 18000, 275, 52, 84, "Germany");
	CAR("cayman_gt4_981", "Porsche", "Cayman GT4", 2016, EVehicleClass::Sports, 100000, 295, 62, 90, "Germany");
	CAR("cayman_gt4_rs", "Porsche", "718 Cayman GT4 RS", 2023, EVehicleClass::Super, 150000, 315, 68, 92, "Germany");
	CAR("cayenne_turbo_gt", "Porsche", "Cayenne Turbo GT", 2023, EVehicleClass::SUV, 180000, 300, 62, 58, "Germany");
	CAR("macan_gts", "Porsche", "Macan GTS", 2023, EVehicleClass::SUV, 85000, 272, 58, 68, "Germany");
	CAR("taycan_turbo_s", "Porsche", "Taycan Turbo S", 2023, EVehicleClass::Sports, 195000, 260, 78, 78, "Germany");

	// --- FERRARI (all eras) ---
	CAR("250_gto", "Ferrari", "250 GTO", 1962, EVehicleClass::Classic, 48000000, 280, 48, 68, "Italy");
	CAR("250_lm", "Ferrari", "250 LM", 1964, EVehicleClass::Classic, 15000000, 290, 50, 65, "Italy");
	CAR("275_gtb4", "Ferrari", "275 GTB/4", 1967, EVehicleClass::Classic, 3000000, 270, 48, 65, "Italy");
	CAR("dino_246_gt", "Ferrari", "Dino 246 GT", 1972, EVehicleClass::Classic, 300000, 230, 45, 78, "Italy");
	CAR("365_gtb4", "Ferrari", "365 GTB/4 Daytona", 1971, EVehicleClass::Classic, 800000, 280, 50, 62, "Italy");
	CAR("308_gts", "Ferrari", "308 GTS", 1982, EVehicleClass::Classic, 60000, 255, 42, 72, "Italy");
	CAR("288_gto", "Ferrari", "288 GTO", 1985, EVehicleClass::Super, 3000000, 304, 58, 68, "Italy");
	CAR("testarossa", "Ferrari", "Testarossa", 1986, EVehicleClass::Classic, 120000, 290, 55, 62, "Italy");
	CAR("512_tr", "Ferrari", "512 TR", 1992, EVehicleClass::Classic, 100000, 314, 55, 64, "Italy");
	CAR("f355", "Ferrari", "F355 Berlinetta", 1997, EVehicleClass::Sports, 60000, 295, 54, 75, "Italy");
	CAR("550_maranello", "Ferrari", "550 Maranello", 2000, EVehicleClass::Sports, 90000, 320, 55, 70, "Italy");
	CAR("360_modena", "Ferrari", "360 Modena", 2003, EVehicleClass::Sports, 55000, 295, 54, 76, "Italy");
	CAR("360_cs", "Ferrari", "360 Challenge Stradale", 2005, EVehicleClass::Super, 150000, 300, 58, 82, "Italy");
	CAR("430_scuderia", "Ferrari", "430 Scuderia", 2008, EVehicleClass::Super, 180000, 320, 68, 84, "Italy");
	CAR("f430", "Ferrari", "F430", 2007, EVehicleClass::Super, 80000, 315, 62, 78, "Italy");
	CAR("599_gto", "Ferrari", "599 GTO", 2011, EVehicleClass::Super, 350000, 335, 72, 74, "Italy");
	CAR("458_speciale", "Ferrari", "458 Speciale", 2015, EVehicleClass::Super, 280000, 325, 74, 86, "Italy");
	CAR("458", "Ferrari", "458 Italia", 2012, EVehicleClass::Super, 150000, 325, 68, 82, "Italy");
	CAR("812_gts", "Ferrari", "812 GTS", 2021, EVehicleClass::Super, 400000, 340, 72, 78, "Italy");
	CAR("sp3_daytona", "Ferrari", "SP3 Daytona", 2023, EVehicleClass::Super, 2200000, 340, 78, 80, "Italy");
	CAR("296_gtb", "Ferrari", "296 GTB", 2023, EVehicleClass::Super, 320000, 330, 74, 84, "Italy");

	// --- LAMBORGHINI (all eras) ---
	CAR("miura_sv", "Lamborghini", "Miura SV", 1971, EVehicleClass::Classic, 2000000, 290, 52, 60, "Italy");
	CAR("jalpa", "Lamborghini", "Jalpa", 1985, EVehicleClass::Classic, 60000, 240, 42, 62, "Italy");
	CAR("diablo_sv", "Lamborghini", "Diablo SV", 1997, EVehicleClass::Super, 180000, 328, 62, 64, "Italy");
	CAR("diablo_gt", "Lamborghini", "Diablo GT", 2000, EVehicleClass::Super, 500000, 338, 65, 66, "Italy");
	CAR("gallardo_560", "Lamborghini", "Gallardo LP560-4", 2009, EVehicleClass::Super, 100000, 325, 68, 78, "Italy");
	CAR("gallardo_superleggera", "Lamborghini", "Gallardo Superleggera", 2008, EVehicleClass::Super, 120000, 315, 64, 82, "Italy");
	CAR("gallardo_squadra", "Lamborghini", "Gallardo Squadra Corse", 2014, EVehicleClass::Super, 200000, 320, 66, 84, "Italy");
	CAR("murcielago_sv", "Lamborghini", "Murciélago SV", 2010, EVehicleClass::Super, 350000, 342, 72, 70, "Italy");
	CAR("reventon", "Lamborghini", "Reventón", 2008, EVehicleClass::Super, 1500000, 340, 68, 72, "Italy");
	CAR("veneno", "Lamborghini", "Veneno", 2014, EVehicleClass::Hypercar, 4500000, 355, 78, 76, "Italy");
	CAR("centenario", "Lamborghini", "Centenario", 2017, EVehicleClass::Hypercar, 2000000, 350, 76, 78, "Italy");
	CAR("sian", "Lamborghini", "Sián FKP 37", 2020, EVehicleClass::Hypercar, 3600000, 350, 76, 78, "Italy");
	CAR("countach_lpi", "Lamborghini", "Countach LPI 800-4", 2022, EVehicleClass::Hypercar, 2640000, 355, 76, 76, "Italy");
	CAR("revuelto", "Lamborghini", "Revuelto", 2024, EVehicleClass::Hypercar, 600000, 350, 78, 80, "Italy");
	CAR("huracan_sterrato", "Lamborghini", "Huracán Sterrato", 2023, EVehicleClass::Rally, 280000, 260, 62, 76, "Italy");
	CAR("huracan_tecnica", "Lamborghini", "Huracán Tecnica", 2023, EVehicleClass::Super, 250000, 325, 72, 82, "Italy");
	CAR("lm002", "Lamborghini", "LM002", 1988, EVehicleClass::SUV, 80000, 210, 32, 40, "Italy");

	// --- BUGATTI (all eras) ===
	CAR("type_35b", "Bugatti", "Type 35B", 1928, EVehicleClass::Classic, 2000000, 190, 30, 52, "France");
	CAR("type_41_royale", "Bugatti", "Type 41 Royale", 1930, EVehicleClass::Classic, 15000000, 160, 18, 38, "France");
	CAR("type_57sc_atlantic", "Bugatti", "Type 57SC Atlantic", 1938, EVehicleClass::Classic, 35000000, 200, 35, 50, "France");
	CAR("eb110_ss", "Bugatti", "EB110 Super Sport", 1994, EVehicleClass::Super, 800000, 355, 72, 72, "France");
	CAR("veyron_hermes", "Bugatti", "Veyron Hermès", 2008, EVehicleClass::Hypercar, 4500000, 407, 82, 72, "France");
	CAR("veyron_gs_vitesse", "Bugatti", "Veyron Grand Sport Vitesse", 2013, EVehicleClass::Hypercar, 2500000, 431, 84, 70, "France");
	CAR("chiron_sport_300", "Bugatti", "Chiron Super Sport 300+", 2021, EVehicleClass::Hypercar, 4500000, 490, 92, 76, "France");
	CAR("centodieci", "Bugatti", "Centodieci", 2022, EVehicleClass::Hypercar, 9000000, 380, 82, 78, "France");
	CAR("bolide", "Bugatti", "Bolide", 2024, EVehicleClass::Hypercar, 4700000, 500, 94, 74, "France");

	// --- McLAREN (all eras) ===
	CAR("mp4_12c", "McLaren", "MP4-12C", 2013, EVehicleClass::Super, 120000, 333, 72, 82, "UK");
	CAR("650s", "McLaren", "650S", 2017, EVehicleClass::Super, 160000, 333, 74, 84, "UK");
	CAR("675lt", "McLaren", "675LT", 2017, EVehicleClass::Super, 280000, 330, 76, 86, "UK");
	CAR("570s", "McLaren", "570S", 2018, EVehicleClass::Super, 150000, 328, 72, 84, "UK");
	CAR("600lt", "McLaren", "600LT", 2020, EVehicleClass::Super, 200000, 328, 74, 86, "UK");
	CAR("765lt", "McLaren", "765LT", 2021, EVehicleClass::Super, 380000, 330, 78, 88, "UK");
	CAR("speedtail", "McLaren", "Speedtail", 2020, EVehicleClass::Hypercar, 2300000, 403, 88, 80, "UK");
	CAR("elva", "McLaren", "Elva", 2021, EVehicleClass::Super, 1700000, 326, 72, 86, "UK");
	CAR("gt_mclaren", "McLaren", "GT", 2023, EVehicleClass::Sports, 210000, 326, 68, 82, "UK");

	// --- ASTON MARTIN (all eras) ===
	CAR("db4_gt_zagato", "Aston Martin", "DB4 GT Zagato", 1961, EVehicleClass::Classic, 12000000, 246, 42, 60, "UK");
	CAR("db6", "Aston Martin", "DB6", 1967, EVehicleClass::Classic, 250000, 240, 40, 58, "UK");
	CAR("v8_vantage_77", "Aston Martin", "V8 Vantage", 1977, EVehicleClass::Classic, 80000, 265, 42, 55, "UK");
	CAR("vanquish_s", "Aston Martin", "Vanquish S", 2005, EVehicleClass::Sports, 55000, 320, 58, 68, "UK");
	CAR("dbs_superleggera", "Aston Martin", "DBS Superleggera", 2020, EVehicleClass::Super, 300000, 340, 72, 76, "UK");
	CAR("vantage_f1", "Aston Martin", "Vantage F1 Edition", 2023, EVehicleClass::Sports, 160000, 314, 64, 82, "UK");
	CAR("db12", "Aston Martin", "DB12", 2024, EVehicleClass::Sports, 250000, 325, 68, 78, "UK");
	CAR("one_77", "Aston Martin", "One-77", 2011, EVehicleClass::Hypercar, 1500000, 354, 72, 74, "UK");
	CAR("victor", "Aston Martin", "Victor", 2021, EVehicleClass::Super, 3000000, 320, 68, 72, "UK");
	CAR("vulcan", "Aston Martin", "Vulcan", 2016, EVehicleClass::Hypercar, 2300000, 330, 80, 82, "UK");

	// === LINCOLN / CADILLAC / BUICK CLASSICS ===
	CAR("mark_viii", "Lincoln", "Mark VIII", 1998, EVehicleClass::Sports, 8000, 240, 42, 62, "USA");
	CAR("coupe_deville_59", "Cadillac", "Coupe DeVille", 1959, EVehicleClass::Classic, 50000, 175, 22, 40, "USA");
	CAR("el_dorado_76", "Cadillac", "Eldorado Convertible", 1976, EVehicleClass::Classic, 15000, 185, 25, 38, "USA");
	CAR("fleetwood_brougham", "Cadillac", "Fleetwood Brougham", 1996, EVehicleClass::Sedan, 5000, 175, 22, 42, "USA");
	CAR("roadmaster", "Buick", "Roadmaster Estate", 1996, EVehicleClass::Sedan, 8000, 190, 30, 45, "USA");

	// === JAPANESE ICONS ADDITIONS ===
	CAR("civic_si_em1", "Honda", "Civic Si Coupé (EM1)", 2000, EVehicleClass::Sports, 12000, 210, 40, 84, "Japan");
	CAR("del_sol_vtec", "Honda", "CR-X del Sol VTEC", 1997, EVehicleClass::Compact, 8000, 195, 38, 80, "Japan");
	CAR("legend_coupe", "Honda", "Legend Coupé", 1991, EVehicleClass::Sports, 8000, 220, 38, 68, "Japan");
	CAR("mr2_turbo", "Toyota", "MR2 Turbo", 1993, EVehicleClass::Sports, 15000, 240, 48, 78, "Japan");
	CAR("celica_gt4_st205", "Toyota", "Celica GT-Four ST205", 1995, EVehicleClass::Rally, 25000, 250, 48, 80, "Japan");
	CAR("celica_supra_mk2", "Toyota", "Celica Supra", 1984, EVehicleClass::Classic, 15000, 210, 35, 62, "Japan");
	CAR("galant_vr4", "Mitsubishi", "Galant VR-4", 1992, EVehicleClass::Sports, 10000, 220, 40, 72, "Japan");
	CAR("skyline_r31_gts", "Nissan", "Skyline R31 GTS-R", 1988, EVehicleClass::Sports, 15000, 220, 38, 68, "Japan");
	CAR("fairlady_z32", "Nissan", "Fairlady Z Twin Turbo", 1993, EVehicleClass::Sports, 12000, 260, 50, 70, "Japan");
	CAR("datsun_510", "Datsun", "510 SSS", 1972, EVehicleClass::Classic, 25000, 175, 30, 72, "Japan");
	CAR("datsun_280z", "Datsun", "280Z", 1977, EVehicleClass::Classic, 22000, 210, 35, 65, "Japan");
	CAR("levin_ae111", "Toyota", "Levin BZ-R", 1998, EVehicleClass::Sports, 12000, 210, 42, 82, "Japan");

	// === EUORPEAN HOT HATCH ICONS ===
	CAR("escort_rs_turbo", "Ford", "Escort RS Turbo", 1990, EVehicleClass::Sports, 12000, 205, 38, 74, "UK");
	CAR("fiesta_st_mk7", "Ford", "Fiesta ST", 2018, EVehicleClass::Compact, 18000, 230, 48, 82, "UK");
	CAR("puma_racing", "Ford", "Puma Racing", 2001, EVehicleClass::Compact, 6000, 205, 38, 80, "UK");
	CAR("fiat_coupe_20v", "Fiat", "Coupé 20V Turbo", 2000, EVehicleClass::Sports, 10000, 250, 48, 72, "Italy");
	CAR("seat_leon_cupra", "Seat", "León Cupra R", 2023, EVehicleClass::Sports, 42000, 265, 52, 76, "Spain");
	CAR("megane_r26r", "Renault", "Mégane R26.R", 2009, EVehicleClass::Sports, 25000, 237, 45, 86, "France");
	CAR("a45_amg", "Mercedes", "A45 AMG", 2023, EVehicleClass::Sports, 62000, 310, 62, 78, "Germany");
	CAR("m135i", "BMW", "M135i xDrive", 2023, EVehicleClass::Sports, 52000, 280, 52, 76, "Germany");
	CAR("rs3_sportback", "Audi", "RS3 Sportback", 2023, EVehicleClass::Sports, 65000, 280, 62, 80, "Germany");
	CAR("s3", "Audi", "S3", 2023, EVehicleClass::Sports, 48000, 280, 52, 78, "Germany");
	CAR("cupra_formentor", "Cupra", "Formentor VZ5", 2023, EVehicleClass::Sports, 58000, 270, 52, 74, "Spain");

	// === AMERICAN MUSCLE ADDITIONS ===
	CAR("chevelle_ss_454", "Chevrolet", "Chevelle SS 454", 1970, EVehicleClass::Muscle, 60000, 215, 55, 48, "USA");
	CAR("mustang_mach1", "Ford", "Mustang Mach 1", 1971, EVehicleClass::Muscle, 45000, 215, 48, 48, "USA");
	CAR("camaro_iroc_z", "Chevrolet", "Camaro IROC-Z", 1988, EVehicleClass::Muscle, 12000, 230, 42, 55, "USA");
	CAR("trans_am_ws6", "Pontiac", "Firebird Trans Am WS6", 2002, EVehicleClass::Muscle, 18000, 260, 50, 58, "USA");
	CAR("gtx_440", "Plymouth", "GTX 440 Six-Pack", 1970, EVehicleClass::Muscle, 50000, 220, 52, 48, "USA");
	CAR("dart_demon", "Dodge", "Dart Demon 340", 1971, EVehicleClass::Muscle, 25000, 205, 42, 48, "USA");
	CAR("duster_340", "Plymouth", "Duster 340", 1971, EVehicleClass::Muscle, 20000, 205, 42, 48, "USA");

	// === MOTORCYCLES ===
	CAR("ducati_panigale_v4", "Ducati", "Panigale V4 SP2", 2023, EVehicleClass::Motorcycle, 45000, 312, 98, 95, "Italy");
	CAR("ducati_monster", "Ducati", "Monster 1200S", 2020, EVehicleClass::Motorcycle, 18000, 250, 85, 88, "Italy");
	CAR("kawasaki_h2r", "Kawasaki", "Ninja H2R", 2023, EVehicleClass::Motorcycle, 58000, 400, 96, 80, "Japan");
	CAR("kawasaki_zx10r", "Kawasaki", "Ninja ZX-10R", 2023, EVehicleClass::Motorcycle, 18000, 312, 92, 90, "Japan");
	CAR("yamaha_r1", "Yamaha", "YZF-R1M", 2023, EVehicleClass::Motorcycle, 27000, 305, 92, 92, "Japan");
	CAR("yamaha_mt09", "Yamaha", "MT-09 SP", 2023, EVehicleClass::Motorcycle, 12000, 245, 85, 86, "Japan");
	CAR("mv_agusta_f4", "MV Agusta", "F4 RC", 2023, EVehicleClass::Motorcycle, 55000, 305, 92, 88, "Italy");
	CAR("aprilia_rsv4", "Aprilia", "RSV4 Factory", 2023, EVehicleClass::Motorcycle, 27000, 310, 94, 92, "Italy");
	CAR("triumph_rocket3", "Triumph", "Rocket 3 R", 2023, EVehicleClass::Motorcycle, 24000, 240, 80, 65, "UK");
	CAR("indian_scout", "Indian", "Scout Bobber", 2023, EVehicleClass::Motorcycle, 14000, 195, 55, 68, "USA");
	CAR("harley_fatboy", "Harley-Davidson", "Fat Boy 114", 2023, EVehicleClass::Motorcycle, 22000, 185, 52, 58, "USA");
	CAR("ktm_superduke", "KTM", "1290 Super Duke R", 2023, EVehicleClass::Motorcycle, 20000, 280, 90, 86, "Austria");
	CAR("triumph_street_triple", "Triumph", "Street Triple RS", 2023, EVehicleClass::Motorcycle, 13000, 245, 82, 90, "UK");
	// Accessibility vehicles
	CAR("wheelchair_manual", "Quickie", "Manual Wheelchair", 2024, EVehicleClass::Compact, 500, 15, 15, 85, "UK");
	CAR("wheelchair_power", "Invacare", "Power Wheelchair", 2024, EVehicleClass::Compact, 3000, 20, 25, 75, "UK");
	CAR("wheelchair_sport", "RGK", "Basketball Chair", 2024, EVehicleClass::Compact, 5000, 30, 40, 90, "UK");
	CAR("wheelchair_adapt", "BraunAbility", "Adaptive Drive Vehicle", 2024, EVehicleClass::Sedan, 45000, 180, 35, 65, "UK");
}

void UVehicleSystem::SeedUpgrades()
{
	for (int32 cat = 0; cat < 8; ++cat)
	{
		const TCHAR* CatName = cat == 0 ? TEXT("Engine") : cat == 1 ? TEXT("Brakes") : cat == 2 ? TEXT("Transmission") :
			cat == 3 ? TEXT("Turbo") : cat == 4 ? TEXT("Suspension") : cat == 5 ? TEXT("Armor") : cat == 6 ? TEXT("Tires") : TEXT("Nitrous");
		FVehicleUpgrade U;
		U.Level = 0; U.Cost = 0; U.Name = FString::Printf(TEXT("Stock %s"), CatName); UpgradeCatalog[cat].Add(U);
		U.Level = 1; U.Cost = 5000; U.Name = FString::Printf(TEXT("Street %s"), CatName); UpgradeCatalog[cat].Add(U);
		U.Level = 2; U.Cost = 12000; U.Name = FString::Printf(TEXT("Sport %s"), CatName); UpgradeCatalog[cat].Add(U);
		U.Level = 3; U.Cost = 25000; U.Name = FString::Printf(TEXT("Race %s"), CatName); UpgradeCatalog[cat].Add(U);
		U.Level = 4; U.Cost = 50000; U.Name = FString::Printf(TEXT("Elite %s"), CatName); UpgradeCatalog[cat].Add(U);
	}
}

const FVehicleDef* UVehicleSystem::FindVehicle(const FString& Id) const
{
	for (const FVehicleDef& V : Catalog) if (V.Id == Id) return &V;
	return nullptr;
}

bool UVehicleSystem::BuyVehicle(const FString& Id)
{
	const FVehicleDef* Def = FindVehicle(Id);
	if (!Def || Garage.Num() >= GarageSlots) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Def->BaseValue) return false;
	Sess->AddCurrency(ECurrency::Credits, -Def->BaseValue);
	FPlayerVehicle PV; PV.Id = Id; PV.bOwned = true; PV.bInsured = false; PV.Condition = 100; PV.Mileage = 0;
	PV.Plate = FString::Printf(TEXT("%s-%04d"), *Def->Make.Left(3).ToUpper(), FMath::RandRange(100, 9999));
	for (int32 i = 0; i < (int32)EUpgradeCategory::COUNT; ++i) PV.Upgrades[i] = 0;
	Garage.Add(PV);
	Sess->ShowMessage(FString::Printf(TEXT("Purchased: %s %s (%d) for %d credits. Plate: %s"), *Def->Make, *Def->Model, Def->Year, Def->BaseValue, *PV.Plate));
	Sess->RecordEvent(FString::Printf(TEXT("Vehicle purchased: %s %s"), *Def->Make, *Def->Model));
	return true;
}

bool UVehicleSystem::StealVehicle(const FString& Id)
{
	const FVehicleDef* Def = FindVehicle(Id);
	if (!Def || Garage.Num() >= GarageSlots) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	FPlayerVehicle PV; PV.Id = Id; PV.bOwned = false; PV.Condition = FMath::RandRange(60, 95);
	PV.Plate = FString::Printf(TEXT("HOT-%04d"), FMath::RandRange(1, 9999));
	for (int32 i = 0; i < (int32)EUpgradeCategory::COUNT; ++i) PV.Upgrades[i] = 0;
	Garage.Add(PV);
	int32 Stars = (Def->BaseValue > 100000) ? 3 : (Def->BaseValue > 50000) ? 2 : 1;
	Sess->WantedLevel = FMath::Min(5, Sess->WantedLevel + Stars);
	Sess->ShowMessage(FString::Printf(TEXT("GRAND THEFT AUTO: %s %s stolen! Wanted level +%d. Hot plate: %s"), *Def->Make, *Def->Model, Stars, *PV.Plate));
	Sess->AddInfamy(5);
	return true;
}

bool UVehicleSystem::SellVehicle(int32 GarageIdx)
{
	if (GarageIdx < 0 || GarageIdx >= Garage.Num()) return false;
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess) return false;
	int32 Value = GetVehicleValue(Garage[GarageIdx]);
	Sess->AddCurrency(ECurrency::Credits, Value);
	const FVehicleDef* Def = FindVehicle(Garage[GarageIdx].Id);
	Sess->ShowMessage(FString::Printf(TEXT("Sold: %s %s for %d credits."), Def ? *Def->Make : TEXT("?"), Def ? *Def->Model : TEXT("?"), Value));
	Garage.RemoveAt(GarageIdx);
	return true;
}

int32 UVehicleSystem::GetVehicleValue(const FPlayerVehicle& V) const
{
	const FVehicleDef* Def = FindVehicle(V.Id);
	if (!Def) return 0;
	float Value = Def->BaseValue * (V.bOwned ? 0.6f : 0.25f); // stolen = 25%, owned = 60%
	Value *= V.Condition / 100.f;
	for (int32 i = 0; i < 8; ++i) Value += V.Upgrades[i] * 3000;
	return FMath::Max(500, FMath::RoundToInt(Value));
}

bool UVehicleSystem::ApplyUpgrade(int32 GarageIdx, EUpgradeCategory Cat)
{
	if (GarageIdx < 0 || GarageIdx >= Garage.Num()) return false;
	int32 Cur = Garage[GarageIdx].Upgrades[(int32)Cat];
	if (Cur >= 4) return false;
	int32 Cost = GetUpgradeCost(Cur + 1);
	UChimeraSessionSubsystem* Sess = GetGameInstance()->GetSubsystem<UChimeraSessionSubsystem>();
	if (!Sess || Sess->GetCurrency(ECurrency::Credits) < Cost) return false;
	Sess->AddCurrency(ECurrency::Credits, -Cost);
	Garage[GarageIdx].Upgrades[(int32)Cat]++;
	const FVehicleDef* Def = FindVehicle(Garage[GarageIdx].Id);
	const FVehicleUpgrade& U = GetUpgradeInfo(Cat, Garage[GarageIdx].Upgrades[(int32)Cat]);
	Sess->ShowMessage(FString::Printf(TEXT("Upgraded %s: %s installed (%d credits)."), Def ? *Def->Model : TEXT("?"), *U.Name, Cost));
	return true;
}

int32 UVehicleSystem::GetUpgradeCost(int32 Level) const { return Level == 1 ? 5000 : Level == 2 ? 12000 : Level == 3 ? 25000 : 50000; }
const FVehicleUpgrade& UVehicleSystem::GetUpgradeInfo(EUpgradeCategory Cat, int32 Level) const { return UpgradeCatalog[(int32)Cat][Level]; }
