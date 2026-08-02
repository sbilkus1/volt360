// Auto-generated complete vehicle catalog: Porsche, Mercedes, BMW, Ferrari, Lamborghini, Aston Martin.
// Every model, every generation, every variant.
#include "Core/ChimeraVehicles.h"

#define CAR(id,make,model,yr,cls,val,spd,acc,han,cntry) \
	V.Id=id; V.Make=make; V.Model=model; V.Year=yr; V.VClass=cls; V.BaseValue=val; V.TopSpeed=spd; V.Acceleration=acc; V.Handling=han; V.Country=cntry; Catalog.Add(V);

void AddMercedesComplete(TArray<FVehicleDef>& Catalog)
{
	FVehicleDef V;
	// === 1920s-1940s ===
	CAR("w02_200", "Mercedes-Benz", "W02 8/38 PS Stuttgart 200", 1928, EVehicleClass::Classic, 35000, 85, 10, 40, "Germany");
	CAR("w06_ssk", "Mercedes-Benz", "SSK", 1928, EVehicleClass::Classic, 5000000, 192, 30, 42, "Germany");
	CAR("w07_770", "Mercedes-Benz", "770 Großer (W07)", 1931, EVehicleClass::Classic, 800000, 160, 18, 35, "Germany");
	CAR("w15_170", "Mercedes-Benz", "170 (W15)", 1934, EVehicleClass::Classic, 18000, 108, 12, 42, "Germany");
	CAR("w29_500k", "Mercedes-Benz", "500K Roadster (W29)", 1935, EVehicleClass::Classic, 4000000, 160, 25, 40, "Germany");
	CAR("w29_540k", "Mercedes-Benz", "540K Roadster (W29)", 1937, EVehicleClass::Classic, 6000000, 170, 28, 38, "Germany");
	CAR("w136_170v", "Mercedes-Benz", "170V (W136)", 1938, EVehicleClass::Classic, 15000, 108, 12, 42, "Germany");
	CAR("w138_260d", "Mercedes-Benz", "260D (W138)", 1937, EVehicleClass::Classic, 22000, 95, 10, 38, "Germany");
	CAR("w150_770", "Mercedes-Benz", "770 Großer (W150)", 1940, EVehicleClass::Classic, 1200000, 170, 18, 32, "Germany");
	// === 1950s ===
	CAR("w187_220", "Mercedes-Benz", "220 (W187)", 1953, EVehicleClass::Classic, 30000, 145, 18, 48, "Germany");
	CAR("w186_300", "Mercedes-Benz", "300 Adenauer (W186)", 1955, EVehicleClass::Classic, 50000, 160, 20, 45, "Germany");
	CAR("w188_300s", "Mercedes-Benz", "300S (W188)", 1954, EVehicleClass::Classic, 150000, 175, 22, 45, "Germany");
	CAR("w120_180", "Mercedes-Benz", "180 Ponton (W120)", 1956, EVehicleClass::Classic, 12000, 130, 15, 50, "Germany");
	CAR("w180_220s", "Mercedes-Benz", "220S (W180)", 1957, EVehicleClass::Classic, 18000, 150, 18, 48, "Germany");
	CAR("w198_300sl_gullwing", "Mercedes-Benz", "300SL Gullwing (W198)", 1955, EVehicleClass::Classic, 1400000, 260, 38, 58, "Germany");
	CAR("w198_300sl_roadster", "Mercedes-Benz", "300SL Roadster (W198)", 1958, EVehicleClass::Classic, 1200000, 250, 38, 58, "Germany");
	CAR("r121_190sl", "Mercedes-Benz", "190SL (R121)", 1958, EVehicleClass::Classic, 110000, 175, 25, 55, "Germany");
	CAR("w128_220se", "Mercedes-Benz", "220SE (W128)", 1959, EVehicleClass::Classic, 22000, 155, 20, 48, "Germany");
	// === 1960s ===
	CAR("w111_280se", "Mercedes-Benz", "280SE (W111)", 1968, EVehicleClass::Classic, 25000, 190, 28, 50, "Germany");
	CAR("w110_190c", "Mercedes-Benz", "190c (W110)", 1964, EVehicleClass::Classic, 12000, 140, 20, 50, "Germany");
	CAR("w112_300se", "Mercedes-Benz", "300SE (W112)", 1964, EVehicleClass::Classic, 30000, 195, 30, 48, "Germany");
	CAR("w113_230sl", "Mercedes-Benz", "230SL Pagoda (W113)", 1965, EVehicleClass::Classic, 80000, 195, 28, 58, "Germany");
	CAR("w113_280sl", "Mercedes-Benz", "280SL Pagoda (W113)", 1968, EVehicleClass::Classic, 120000, 195, 30, 58, "Germany");
	CAR("w100_600", "Mercedes-Benz", "600 (W100)", 1965, EVehicleClass::Classic, 180000, 205, 30, 42, "Germany");
	CAR("w108_280se", "Mercedes-Benz", "280SE (W108)", 1969, EVehicleClass::Classic, 18000, 190, 28, 50, "Germany");
	CAR("w109_300sel_63", "Mercedes-Benz", "300SEL 6.3 (W109)", 1969, EVehicleClass::Classic, 40000, 220, 35, 52, "Germany");
	CAR("w114_250", "Mercedes-Benz", "250 (W114)", 1971, EVehicleClass::Classic, 10000, 180, 25, 52, "Germany");
	CAR("w115_240d", "Mercedes-Benz", "240D (W115)", 1975, EVehicleClass::Classic, 8000, 148, 18, 50, "Germany");
	// === 1970s ===
	CAR("r107_350sl", "Mercedes-Benz", "350SL (R107)", 1974, EVehicleClass::Classic, 18000, 195, 28, 55, "Germany");
	CAR("r107_450sl", "Mercedes-Benz", "450SL (R107)", 1977, EVehicleClass::Classic, 20000, 205, 30, 55, "Germany");
	CAR("r107_560sl", "Mercedes-Benz", "560SL (R107)", 1987, EVehicleClass::Classic, 30000, 225, 35, 55, "Germany");
	CAR("c107_450slc", "Mercedes-Benz", "450SLC (C107)", 1977, EVehicleClass::Classic, 18000, 210, 30, 55, "Germany");
	CAR("w116_450sel", "Mercedes-Benz", "450SEL 6.9 (W116)", 1976, EVehicleClass::Classic, 25000, 225, 35, 50, "Germany");
	CAR("w123_280e", "Mercedes-Benz", "280E (W123)", 1979, EVehicleClass::Classic, 12000, 190, 28, 52, "Germany");
	CAR("w123_300d", "Mercedes-Benz", "300D (W123)", 1981, EVehicleClass::Classic, 10000, 165, 22, 50, "Germany");
	// === 1980s ===
	CAR("w126_560sel", "Mercedes-Benz", "560SEL (W126)", 1987, EVehicleClass::Classic, 15000, 240, 32, 50, "Germany");
	CAR("w126_500sec", "Mercedes-Benz", "500SEC (C126)", 1987, EVehicleClass::Classic, 18000, 240, 35, 50, "Germany");
	CAR("w201_190e_26", "Mercedes-Benz", "190E 2.6 (W201)", 1988, EVehicleClass::Classic, 8000, 210, 32, 58, "Germany");
	CAR("w201_190e_23_16v", "Mercedes-Benz", "190E 2.3-16 (W201)", 1986, EVehicleClass::Classic, 25000, 230, 40, 62, "Germany");
	CAR("w201_190e_evo_ii", "Mercedes-Benz", "190E 2.5-16 Evo II", 1991, EVehicleClass::Classic, 150000, 250, 48, 68, "Germany");
	CAR("w124_500e", "Mercedes-Benz", "500E (W124)", 1992, EVehicleClass::Classic, 30000, 285, 42, 58, "Germany");
	CAR("w124_e320", "Mercedes-Benz", "E320 (W124)", 1994, EVehicleClass::Classic, 8000, 220, 32, 55, "Germany");
	CAR("r129_500sl", "Mercedes-Benz", "500SL (R129)", 1992, EVehicleClass::Sports, 12000, 250, 42, 58, "Germany");
	CAR("r129_sl600", "Mercedes-Benz", "SL600 (R129)", 1995, EVehicleClass::Sports, 18000, 280, 48, 56, "Germany");
	CAR("w460_g300d", "Mercedes-Benz", "G 300D (W460)", 1984, EVehicleClass::SUV, 15000, 135, 15, 35, "Germany");
	// === 1990s ===
	CAR("w140_s600", "Mercedes-Benz", "S600 (W140)", 1995, EVehicleClass::Sedan, 15000, 280, 42, 50, "Germany");
	CAR("w202_c220", "Mercedes-Benz", "C220 (W202)", 1996, EVehicleClass::Sedan, 5000, 210, 28, 58, "Germany");
	CAR("w202_c43_amg", "Mercedes-Benz", "C43 AMG (W202)", 1998, EVehicleClass::Sports, 10000, 300, 48, 68, "Germany");
	CAR("w210_e55_amg", "Mercedes-Benz", "E55 AMG (W210)", 1999, EVehicleClass::Sports, 12000, 320, 50, 65, "Germany");
	CAR("w163_ml55_amg", "Mercedes-Benz", "ML55 AMG (W163)", 2000, EVehicleClass::SUV, 10000, 240, 48, 48, "Germany");
	// === 2000s ===
	CAR("w220_s65_amg", "Mercedes-Benz", "S65 AMG (W220)", 2005, EVehicleClass::Sedan, 25000, 320, 52, 52, "Germany");
	CAR("w203_c32_amg", "Mercedes-Benz", "C32 AMG (W203)", 2003, EVehicleClass::Sports, 12000, 320, 52, 65, "Germany");
	CAR("c209_clk55_amg", "Mercedes-Benz", "CLK55 AMG (C209)", 2004, EVehicleClass::Sports, 15000, 320, 52, 68, "Germany");
	CAR("w219_cls55_amg", "Mercedes-Benz", "CLS55 AMG (W219)", 2005, EVehicleClass::Sports, 15000, 320, 55, 68, "Germany");
	CAR("w211_e63_amg", "Mercedes-Benz", "E63 AMG (W211)", 2007, EVehicleClass::Sports, 18000, 325, 55, 68, "Germany");
	CAR("r230_sl55_amg", "Mercedes-Benz", "SL55 AMG (R230)", 2003, EVehicleClass::Sports, 25000, 310, 60, 68, "Germany");
	CAR("r230_sl65_amg", "Mercedes-Benz", "SL65 AMG (R230)", 2005, EVehicleClass::Super, 35000, 330, 62, 66, "Germany");
	CAR("r170_slk230", "Mercedes-Benz", "SLK230 Kompressor (R170)", 1999, EVehicleClass::Sports, 8000, 231, 42, 74, "Germany");
	CAR("r170_slk32_amg", "Mercedes-Benz", "SLK32 AMG (R170)", 2002, EVehicleClass::Sports, 12000, 275, 52, 74, "Germany");
	CAR("w164_ml63_amg", "Mercedes-Benz", "ML63 AMG (W164)", 2007, EVehicleClass::SUV, 15000, 275, 55, 50, "Germany");
	CAR("x164_gl550", "Mercedes-Benz", "GL550 (X164)", 2008, EVehicleClass::SUV, 12000, 240, 42, 48, "Germany");
	CAR("w251_r63_amg", "Mercedes-Benz", "R63 AMG (W251)", 2007, EVehicleClass::Sedan, 15000, 275, 55, 55, "Germany");
	CAR("w221_s65_amg", "Mercedes-Benz", "S65 AMG (W221)", 2012, EVehicleClass::Sedan, 80000, 310, 55, 52, "Germany");
	CAR("w204_c63_amg", "Mercedes-Benz", "C63 AMG (W204)", 2011, EVehicleClass::Sports, 25000, 320, 55, 70, "Germany");
	// === 2010s current ===
	CAR("c197_sls_amg", "Mercedes-Benz", "SLS AMG (C197)", 2011, EVehicleClass::Super, 150000, 317, 68, 72, "Germany");
	CAR("c190_amg_gt", "Mercedes-AMG", "AMG GT (C190)", 2016, EVehicleClass::Super, 100000, 310, 68, 78, "Germany");
	CAR("r172_slk55_amg", "Mercedes-Benz", "SLK55 AMG (R172)", 2013, EVehicleClass::Sports, 45000, 315, 62, 74, "Germany");
	CAR("r231_sl63_amg", "Mercedes-Benz", "SL63 AMG (R231)", 2015, EVehicleClass::Sports, 80000, 310, 68, 72, "Germany");
	CAR("w205_c63s", "Mercedes-Benz", "C63 S AMG (W205)", 2018, EVehicleClass::Sports, 55000, 310, 62, 74, "Germany");
	CAR("w213_e63s", "Mercedes-Benz", "E63 S AMG (W213)", 2020, EVehicleClass::Sports, 80000, 310, 62, 70, "Germany");
	CAR("w222_s63_amg", "Mercedes-Benz", "S63 AMG (W222)", 2018, EVehicleClass::Sedan, 120000, 310, 58, 58, "Germany");
	CAR("c257_cls53", "Mercedes-Benz", "CLS53 AMG (C257)", 2019, EVehicleClass::Sports, 70000, 280, 55, 68, "Germany");
	CAR("x290_gt63s", "Mercedes-AMG", "GT63 S 4-Door", 2022, EVehicleClass::Sports, 170000, 315, 68, 72, "Germany");
	CAR("x167_gls63", "Mercedes-Benz", "GLS63 AMG (X167)", 2022, EVehicleClass::SUV, 140000, 280, 58, 50, "Germany");
	CAR("w167_gle63s", "Mercedes-Benz", "GLE63 S AMG (W167)", 2022, EVehicleClass::SUV, 120000, 280, 58, 52, "Germany");
	CAR("v297_eqs580", "Mercedes-Benz", "EQS580 (V297)", 2022, EVehicleClass::Sedan, 125000, 210, 55, 65, "Germany");
	CAR("v295_eqe350", "Mercedes-Benz", "EQE350", 2023, EVehicleClass::Sedan, 75000, 210, 50, 65, "Germany");
	CAR("r232_sl63", "Mercedes-Benz", "SL63 AMG (R232)", 2023, EVehicleClass::Sports, 180000, 315, 68, 72, "Germany");
	CAR("c236_cle53", "Mercedes-Benz", "CLE53 AMG", 2024, EVehicleClass::Sports, 85000, 280, 58, 74, "Germany");
	CAR("w206_c63s_e", "Mercedes-Benz", "C63 S E-Performance", 2024, EVehicleClass::Sports, 95000, 290, 68, 72, "Germany");
	CAR("w465_g63", "Mercedes-Benz", "G63 AMG (W465)", 2025, EVehicleClass::SUV, 180000, 240, 55, 48, "Germany");
	// Tuned
	CAR("brabus_rocket900", "Mercedes-Benz", "Brabus Rocket 900 (GT63 based)", 2022, EVehicleClass::Super, 500000, 330, 78, 74, "Germany");
	CAR("brabus_g800", "Mercedes-Benz", "Brabus G800 Widestar", 2021, EVehicleClass::SUV, 350000, 250, 52, 42, "Germany");
	CAR("mansory_gronos", "Mercedes-Benz", "Mansory Gronos G63", 2022, EVehicleClass::SUV, 500000, 250, 52, 42, "Germany");
	// Special race/homologation
	CAR("clk_gtr_strasse", "Mercedes-Benz", "CLK GTR Straßenversion", 1998, EVehicleClass::Hypercar, 2500000, 320, 72, 72, "Germany");
	CAR("clk_dtm_amg", "Mercedes-Benz", "CLK DTM AMG", 2005, EVehicleClass::Super, 350000, 320, 70, 72, "Germany");
	CAR("amg_one", "Mercedes-AMG", "ONE", 2023, EVehicleClass::Hypercar, 2750000, 352, 92, 88, "Germany");
}

void AddPorscheComplete(TArray<FVehicleDef>& Catalog)
{
	FVehicleDef V;
	// 356
	CAR("356_pre_a", "Porsche", "356 Pre-A", 1953, EVehicleClass::Classic, 120000, 145, 18, 62, "Germany");
	CAR("356a_speedster", "Porsche", "356A Speedster", 1957, EVehicleClass::Classic, 250000, 160, 22, 68, "Germany");
	CAR("356b_s90", "Porsche", "356B Super 90", 1961, EVehicleClass::Classic, 90000, 175, 25, 65, "Germany");
	CAR("356c_carrera2", "Porsche", "356C Carrera 2", 1964, EVehicleClass::Classic, 300000, 180, 28, 65, "Germany");
	// 914
	CAR("914_6", "Porsche", "914/6", 1970, EVehicleClass::Classic, 40000, 195, 32, 78, "Germany");
	// 924
	CAR("924_turbo_s1", "Porsche", "924 Turbo", 1980, EVehicleClass::Sports, 15000, 220, 38, 74, "Germany");
	CAR("924_carrera_gt", "Porsche", "924 Carrera GT", 1981, EVehicleClass::Sports, 35000, 245, 42, 72, "Germany");
	// 928
	CAR("928_s4", "Porsche", "928 S4", 1988, EVehicleClass::Sports, 25000, 270, 42, 62, "Germany");
	CAR("928_gt", "Porsche", "928 GT", 1990, EVehicleClass::Sports, 30000, 280, 45, 60, "Germany");
	CAR("928_gts", "Porsche", "928 GTS", 1994, EVehicleClass::Sports, 45000, 290, 48, 58, "Germany");
	// 944/968
	CAR("944_turbo_s", "Porsche", "944 Turbo S", 1989, EVehicleClass::Sports, 25000, 272, 50, 78, "Germany");
	CAR("968_cs", "Porsche", "968 Club Sport", 1993, EVehicleClass::Sports, 35000, 260, 48, 82, "Germany");
	// 911 Classic + G-Series
	CAR("911_27_rs", "Porsche", "911 Carrera RS 2.7", 1973, EVehicleClass::Classic, 600000, 240, 45, 72, "Germany");
	CAR("911_30_turbo", "Porsche", "911 Turbo 3.0", 1975, EVehicleClass::Classic, 200000, 250, 48, 58, "Germany");
	CAR("911_33_turbo", "Porsche", "911 Turbo 3.3", 1978, EVehicleClass::Classic, 180000, 260, 50, 56, "Germany");
	CAR("911_sc_targa", "Porsche", "911 SC Targa", 1982, EVehicleClass::Classic, 40000, 235, 38, 68, "Germany");
	CAR("911_32_carrera", "Porsche", "911 Carrera 3.2", 1987, EVehicleClass::Classic, 50000, 245, 40, 68, "Germany");
	CAR("911_speedster_89", "Porsche", "911 Speedster", 1989, EVehicleClass::Classic, 250000, 245, 42, 66, "Germany");
	// 964
	CAR("911_964_c2", "Porsche", "911 Carrera 2 (964)", 1991, EVehicleClass::Classic, 45000, 260, 48, 76, "Germany");
	CAR("911_964_turbo", "Porsche", "911 Turbo (964)", 1992, EVehicleClass::Classic, 120000, 290, 55, 68, "Germany");
	CAR("911_964_rs", "Porsche", "911 Carrera RS (964)", 1992, EVehicleClass::Classic, 200000, 260, 52, 78, "Germany");
	CAR("911_964_rsa", "Porsche", "911 RS America", 1993, EVehicleClass::Classic, 150000, 270, 52, 78, "Germany");
	// 993
	CAR("911_993_c2", "Porsche", "911 Carrera (993)", 1995, EVehicleClass::Sports, 60000, 270, 48, 78, "Germany");
	CAR("911_993_turbo", "Porsche", "911 Turbo (993)", 1996, EVehicleClass::Sports, 180000, 290, 58, 68, "Germany");
	CAR("911_993_turbo_s", "Porsche", "911 Turbo S (993)", 1997, EVehicleClass::Sports, 250000, 300, 60, 66, "Germany");
	CAR("911_993_gt2", "Porsche", "911 GT2 (993)", 1995, EVehicleClass::Super, 500000, 301, 62, 72, "Germany");
	// 996
	CAR("911_996_c2", "Porsche", "911 Carrera (996)", 2000, EVehicleClass::Sports, 20000, 280, 48, 78, "Germany");
	CAR("911_996_turbo", "Porsche", "911 Turbo (996)", 2003, EVehicleClass::Sports, 35000, 306, 58, 72, "Germany");
	CAR("911_996_gt2", "Porsche", "911 GT2 (996)", 2003, EVehicleClass::Super, 100000, 315, 65, 72, "Germany");
	CAR("911_996_gt3", "Porsche", "911 GT3 (996)", 2001, EVehicleClass::Sports, 50000, 306, 60, 82, "Germany");
	CAR("911_996_gt3_rs", "Porsche", "911 GT3 RS (996)", 2004, EVehicleClass::Sports, 90000, 308, 62, 84, "Germany");
	// 997
	CAR("911_997_c2s", "Porsche", "911 Carrera S (997)", 2007, EVehicleClass::Sports, 30000, 295, 52, 80, "Germany");
	CAR("911_997_turbo", "Porsche", "911 Turbo (997)", 2008, EVehicleClass::Sports, 45000, 312, 60, 74, "Germany");
	CAR("911_997_turbo_s", "Porsche", "911 Turbo S (997)", 2011, EVehicleClass::Super, 70000, 318, 62, 72, "Germany");
	CAR("911_997_gt2_rs", "Porsche", "911 GT2 RS (997)", 2011, EVehicleClass::Super, 300000, 330, 72, 76, "Germany");
	CAR("911_997_gt3", "Porsche", "911 GT3 (997)", 2008, EVehicleClass::Sports, 70000, 312, 62, 84, "Germany");
	CAR("911_997_gt3_rs_40", "Porsche", "911 GT3 RS 4.0", 2011, EVehicleClass::Super, 350000, 312, 66, 88, "Germany");
	CAR("911_997_sport_classic", "Porsche", "911 Sport Classic", 2010, EVehicleClass::Sports, 250000, 302, 55, 78, "Germany");
	// 991
	CAR("911_991_c2s", "Porsche", "911 Carrera S (991)", 2014, EVehicleClass::Sports, 55000, 302, 55, 82, "Germany");
	CAR("911_991_turbo_s", "Porsche", "911 Turbo S (991)", 2017, EVehicleClass::Super, 140000, 330, 72, 76, "Germany");
	CAR("911_991_gt2_rs", "Porsche", "911 GT2 RS (991)", 2018, EVehicleClass::Super, 300000, 340, 76, 80, "Germany");
	CAR("911_991_gt3", "Porsche", "911 GT3 (991)", 2015, EVehicleClass::Super, 140000, 315, 68, 88, "Germany");
	CAR("911_991_gt3_rs", "Porsche", "911 GT3 RS (991)", 2018, EVehicleClass::Super, 180000, 312, 66, 90, "Germany");
	CAR("911_991_r", "Porsche", "911 R", 2016, EVehicleClass::Sports, 350000, 323, 62, 84, "Germany");
	CAR("911_991_gts", "Porsche", "911 Carrera GTS (991)", 2016, EVehicleClass::Sports, 80000, 308, 58, 84, "Germany");
	// 992
	CAR("911_992_c2s", "Porsche", "911 Carrera S (992)", 2020, EVehicleClass::Sports, 120000, 308, 58, 84, "Germany");
	CAR("911_992_turbo_s", "Porsche", "911 Turbo S (992)", 2022, EVehicleClass::Super, 205000, 330, 78, 84, "Germany");
	CAR("911_992_gt3", "Porsche", "911 GT3 (992)", 2022, EVehicleClass::Super, 170000, 320, 68, 92, "Germany");
	CAR("911_992_gt3_rs", "Porsche", "911 GT3 RS (992)", 2023, EVehicleClass::Super, 250000, 318, 68, 94, "Germany");
	CAR("911_992_sport_classic", "Porsche", "911 Sport Classic", 2023, EVehicleClass::Sports, 270000, 315, 65, 76, "Germany");
	CAR("911_992_st", "Porsche", "911 S/T", 2024, EVehicleClass::Sports, 300000, 300, 58, 86, "Germany");
	// Boxster
	CAR("boxster_986_s", "Porsche", "Boxster S (986)", 2003, EVehicleClass::Sports, 12000, 260, 48, 80, "Germany");
	CAR("boxster_987_spyder", "Porsche", "Boxster Spyder", 2011, EVehicleClass::Sports, 45000, 290, 55, 86, "Germany");
	CAR("boxster_981_gts", "Porsche", "Boxster GTS (981)", 2015, EVehicleClass::Sports, 50000, 284, 55, 86, "Germany");
	CAR("718_boxster_s", "Porsche", "718 Boxster S", 2018, EVehicleClass::Sports, 65000, 285, 55, 86, "Germany");
	CAR("718_spyder_rs", "Porsche", "718 Spyder RS", 2024, EVehicleClass::Sports, 150000, 310, 65, 90, "Germany");
	// Cayman
	CAR("cayman_s_987", "Porsche", "Cayman S (987)", 2008, EVehicleClass::Sports, 18000, 275, 52, 84, "Germany");
	CAR("cayman_r_987", "Porsche", "Cayman R", 2011, EVehicleClass::Sports, 45000, 282, 55, 86, "Germany");
	CAR("cayman_gt4_981", "Porsche", "Cayman GT4", 2016, EVehicleClass::Sports, 100000, 295, 62, 90, "Germany");
	CAR("718_cayman_gt4", "Porsche", "718 Cayman GT4", 2021, EVehicleClass::Super, 110000, 308, 65, 92, "Germany");
	CAR("718_cayman_gt4_rs", "Porsche", "718 Cayman GT4 RS", 2023, EVehicleClass::Super, 150000, 315, 68, 92, "Germany");
	// Hypercars
	CAR("carrera_gt", "Porsche", "Carrera GT", 2005, EVehicleClass::Super, 950000, 334, 78, 82, "Germany");
	CAR("918_spyder", "Porsche", "918 Spyder", 2015, EVehicleClass::Hypercar, 950000, 345, 82, 84, "Germany");
	// Panamera
	CAR("panamera_turbo_s", "Porsche", "Panamera Turbo S", 2018, EVehicleClass::Sports, 90000, 306, 62, 72, "Germany");
	CAR("panamera_sport_turismo", "Porsche", "Panamera Sport Turismo GTS", 2022, EVehicleClass::Sports, 140000, 300, 58, 74, "Germany");
	// Cayenne
	CAR("cayenne_turbo_gt", "Porsche", "Cayenne Turbo GT", 2023, EVehicleClass::SUV, 180000, 300, 62, 58, "Germany");
	// Macan
	CAR("macan_gts", "Porsche", "Macan GTS", 2023, EVehicleClass::SUV, 85000, 272, 58, 68, "Germany");
	// Taycan
	CAR("taycan_turbo_s", "Porsche", "Taycan Turbo S", 2023, EVehicleClass::Sports, 195000, 260, 78, 78, "Germany");
	CAR("taycan_cross_turismo", "Porsche", "Taycan Cross Turismo Turbo", 2023, EVehicleClass::Sports, 200000, 250, 72, 72, "Germany");
	// === Discontinued classics ===
	CAR("550_spyder", "Porsche", "550 Spyder", 1955, EVehicleClass::Classic, 3500000, 220, 35, 65, "Germany");
	CAR("718_rs60", "Porsche", "718 RS 60 Spyder", 1960, EVehicleClass::Classic, 2500000, 240, 38, 68, "Germany");
	CAR("718_rs61", "Porsche", "718 RS 61", 1961, EVehicleClass::Classic, 2200000, 245, 38, 68, "Germany");
	CAR("904_gts", "Porsche", "904 Carrera GTS", 1964, EVehicleClass::Classic, 1800000, 260, 42, 72, "Germany");
	CAR("906_carrera6", "Porsche", "906 Carrera 6", 1966, EVehicleClass::Classic, 1500000, 280, 48, 74, "Germany");
	CAR("912", "Porsche", "912", 1967, EVehicleClass::Classic, 40000, 185, 28, 72, "Germany");
	CAR("914_6", "Porsche", "914/6", 1970, EVehicleClass::Classic, 40000, 195, 32, 78, "Germany");
	CAR("914_8", "Porsche", "914/8", 1971, EVehicleClass::Classic, 2000000, 245, 38, 72, "Germany");
	CAR("930_turbo", "Porsche", "930 Turbo", 1978, EVehicleClass::Classic, 120000, 260, 48, 55, "Germany");
	CAR("930_turbo_flachbau", "Porsche", "930 Turbo Flachbau", 1985, EVehicleClass::Classic, 180000, 270, 50, 55, "Germany");
	// 959
	CAR("959_komfort", "Porsche", "959 Komfort", 1987, EVehicleClass::Classic, 1500000, 315, 65, 78, "Germany");
	CAR("959_sport", "Porsche", "959 Sport", 1988, EVehicleClass::Classic, 1800000, 320, 65, 80, "Germany");
	CAR("959_dakar", "Porsche", "959 Paris-Dakar", 1985, EVehicleClass::Rally, 2500000, 240, 55, 62, "Germany");
	// 968
	CAR("968_cs", "Porsche", "968 Club Sport", 1993, EVehicleClass::Sports, 35000, 260, 48, 82, "Germany");
	CAR("968_turbo_s", "Porsche", "968 Turbo S", 1993, EVehicleClass::Sports, 80000, 280, 52, 80, "Germany");
	// 911 GT1
	CAR("911_gt1_strasse", "Porsche", "911 GT1 Straßenversion", 1997, EVehicleClass::Hypercar, 8000000, 308, 72, 76, "Germany");
	CAR("911_gt1_evo", "Porsche", "911 GT1 Evo", 1997, EVehicleClass::Hypercar, 10000000, 315, 78, 78, "Germany");
	// === Panamera Gen 1 ===
	CAR("panamera_turbo_970", "Porsche", "Panamera Turbo (970)", 2012, EVehicleClass::Sports, 40000, 306, 58, 70, "Germany");
	CAR("panamera_gts_970", "Porsche", "Panamera GTS (970)", 2014, EVehicleClass::Sports, 50000, 290, 55, 72, "Germany");
	// === Cayenne Gen 1/2 ===
	CAR("cayenne_turbo_955", "Porsche", "Cayenne Turbo (955)", 2006, EVehicleClass::SUV, 12000, 266, 52, 50, "Germany");
	CAR("cayenne_turbo_s_955", "Porsche", "Cayenne Turbo S (955)", 2008, EVehicleClass::SUV, 15000, 270, 55, 48, "Germany");
	CAR("cayenne_gts_958", "Porsche", "Cayenne GTS (958)", 2014, EVehicleClass::SUV, 28000, 272, 55, 52, "Germany");
	CAR("cayenne_turbo_s_958", "Porsche", "Cayenne Turbo S (958)", 2015, EVehicleClass::SUV, 35000, 283, 58, 50, "Germany");
	// === Macan Gen 1 ===
	CAR("macan_turbo_95b", "Porsche", "Macan Turbo (95B)", 2018, EVehicleClass::SUV, 45000, 270, 58, 68, "Germany");
	// === Racing Cars ===
	CAR("910_coupe", "Porsche", "910 Coupé", 1967, EVehicleClass::Classic, 1200000, 285, 52, 76, "Germany");
	CAR("907", "Porsche", "907", 1968, EVehicleClass::Classic, 1000000, 300, 55, 78, "Germany");
	CAR("908_3", "Porsche", "908/3 Spyder", 1971, EVehicleClass::Classic, 1500000, 310, 58, 80, "Germany");
	CAR("917k", "Porsche", "917K", 1970, EVehicleClass::Classic, 14000000, 340, 72, 75, "Germany");
	CAR("917_30", "Porsche", "917/30 Can-Am", 1973, EVehicleClass::Classic, 18000000, 385, 88, 65, "Germany");
	CAR("934", "Porsche", "934", 1976, EVehicleClass::Classic, 800000, 290, 58, 72, "Germany");
	CAR("935_77", "Porsche", "935/77", 1977, EVehicleClass::Classic, 1200000, 320, 62, 74, "Germany");
	CAR("935_moby_dick", "Porsche", "935/78 Moby Dick", 1978, EVehicleClass::Classic, 2000000, 366, 68, 72, "Germany");
	CAR("936", "Porsche", "936 Spyder", 1976, EVehicleClass::Classic, 2500000, 320, 62, 78, "Germany");
	CAR("956", "Porsche", "956", 1982, EVehicleClass::Classic, 3500000, 350, 72, 80, "Germany");
	CAR("962c", "Porsche", "962C", 1987, EVehicleClass::Classic, 2500000, 365, 75, 82, "Germany");
	CAR("961", "Porsche", "961", 1986, EVehicleClass::Classic, 1500000, 330, 62, 72, "Germany");
	CAR("919_hybrid", "Porsche", "919 Hybrid", 2015, EVehicleClass::Hypercar, 5000000, 340, 82, 88, "Germany");
	CAR("919_evo", "Porsche", "919 Hybrid Evo", 2018, EVehicleClass::Hypercar, 8000000, 369, 90, 92, "Germany");
	CAR("963", "Porsche", "963 (9R0)", 2023, EVehicleClass::Hypercar, 2500000, 340, 84, 86, "Germany");
	// GT Racing
	CAR("911_rsr_30", "Porsche", "911 Carrera RSR 3.0", 1974, EVehicleClass::Classic, 600000, 280, 52, 72, "Germany");
	CAR("911_rsr_38", "Porsche", "911 Carrera RSR 3.8 (964)", 1993, EVehicleClass::Classic, 400000, 295, 58, 74, "Germany");
	CAR("911_gt2_evo", "Porsche", "911 GT2 Evo (993)", 1996, EVehicleClass::Super, 500000, 305, 65, 76, "Germany");
	CAR("911_gt3r_996", "Porsche", "911 GT3 R (996)", 2001, EVehicleClass::Super, 200000, 310, 68, 82, "Germany");
	CAR("911_gt3_cup_997", "Porsche", "911 GT3 Cup (997)", 2008, EVehicleClass::Super, 120000, 305, 62, 84, "Germany");
	CAR("911_rsr_991", "Porsche", "911 RSR (991)", 2015, EVehicleClass::Super, 500000, 315, 72, 86, "Germany");
	CAR("911_rsr_19", "Porsche", "911 RSR-19", 2019, EVehicleClass::Super, 800000, 315, 75, 88, "Germany");
	CAR("935_991", "Porsche", "935 (991.2)", 2019, EVehicleClass::Super, 800000, 340, 72, 80, "Germany");
	CAR("911_gt2_rs_clubsport", "Porsche", "911 GT2 RS Clubsport", 2019, EVehicleClass::Super, 500000, 335, 74, 82, "Germany");
	CAR("911_gt3_cup_992", "Porsche", "911 GT3 Cup (992)", 2021, EVehicleClass::Super, 250000, 310, 68, 88, "Germany");
	CAR("718_gt4_clubsport", "Porsche", "718 Cayman GT4 Clubsport", 2019, EVehicleClass::Super, 180000, 295, 65, 90, "Germany");
	// Rally
	CAR("911_sc_rs", "Porsche", "911 SC RS", 1984, EVehicleClass::Rally, 300000, 240, 48, 72, "Germany");
	CAR("953_dakar", "Porsche", "911 Carrera 3.2 4x4 Dakar (953)", 1984, EVehicleClass::Rally, 500000, 220, 45, 68, "Germany");
	// Formula
	CAR("718_f2", "Porsche", "718 F2", 1960, EVehicleClass::Classic, 800000, 260, 55, 78, "Germany");
	CAR("804_f1", "Porsche", "804 F1", 1962, EVehicleClass::Classic, 2500000, 270, 60, 76, "Germany");
	CAR("2708_cart", "Porsche", "2708 CART", 1988, EVehicleClass::Classic, 500000, 340, 78, 72, "Germany");
	// === Concepts ===
	CAR("boxster_concept", "Porsche", "Boxster Concept", 1993, EVehicleClass::Classic, 400000, 230, 42, 78, "Germany");
	CAR("carrera_gt_concept", "Porsche", "Carrera GT Concept", 2000, EVehicleClass::Classic, 600000, 310, 65, 78, "Germany");
	CAR("918_rsr", "Porsche", "918 RSR Concept", 2011, EVehicleClass::Hypercar, 2000000, 340, 78, 80, "Germany");
	CAR("mission_e", "Porsche", "Mission E Concept", 2015, EVehicleClass::Sports, 400000, 250, 62, 80, "Germany");
	CAR("mission_e_cross", "Porsche", "Mission E Cross Turismo", 2018, EVehicleClass::Sports, 500000, 250, 60, 75, "Germany");
	CAR("vision_357", "Porsche", "Vision 357", 2023, EVehicleClass::Super, 1200000, 310, 68, 82, "Germany");
	// === Tractors (yes, Porsche made tractors) ===
	CAR("porsche_junior", "Porsche", "Junior 108 Tractor", 1958, EVehicleClass::Classic, 25000, 25, 2, 20, "Germany");
	CAR("porsche_super_308", "Porsche", "Super 308 Tractor", 1960, EVehicleClass::Classic, 30000, 30, 3, 18, "Germany");
	CAR("porsche_master_408", "Porsche", "Master 408 Tractor", 1962, EVehicleClass::Classic, 35000, 35, 4, 18, "Germany");
	// === Historical ===
	CAR("p1", "Porsche", "P1 Egger-Lohner C.2 Phaeton", 1898, EVehicleClass::Classic, 5000000, 25, 2, 15, "Austria");
	CAR("type_64", "Porsche", "Type 64 (60K10)", 1939, EVehicleClass::Classic, 8000000, 145, 18, 58, "Germany");
}

void AddBMWComplete(TArray<FVehicleDef>& Catalog)
{
	FVehicleDef V;
	// M2
	CAR("m2_comp", "BMW", "M2 Competition", 2020, EVehicleClass::Sports, 52000, 285, 58, 84, "Germany");
	CAR("m2_g87", "BMW", "M2 G87", 2023, EVehicleClass::Sports, 65000, 285, 60, 84, "Germany");
	// M3
	CAR("m3_e30_evo2", "BMW", "M3 E30 Evo II", 1989, EVehicleClass::Classic, 90000, 248, 50, 84, "Germany");
	CAR("m3_e36_gt", "BMW", "M3 E36 GT", 1995, EVehicleClass::Sports, 60000, 265, 54, 80, "Germany");
	CAR("m3_e46_csl", "BMW", "M3 E46 CSL", 2004, EVehicleClass::Sports, 70000, 282, 60, 86, "Germany");
	CAR("m3_e92_gts", "BMW", "M3 E92 GTS", 2011, EVehicleClass::Sports, 120000, 320, 65, 82, "Germany");
	CAR("m3_f80_cs", "BMW", "M3 F80 CS", 2018, EVehicleClass::Sports, 80000, 305, 68, 84, "Germany");
	CAR("m3_g80_comp", "BMW", "M3 Competition G80", 2021, EVehicleClass::Sports, 75000, 290, 68, 84, "Germany");
	CAR("m3_touring", "BMW", "M3 Touring", 2023, EVehicleClass::Sports, 80000, 280, 62, 78, "Germany");
	// M4
	CAR("m4_gts", "BMW", "M4 GTS", 2016, EVehicleClass::Super, 100000, 312, 65, 84, "Germany");
	CAR("m4_csl", "BMW", "M4 CSL", 2023, EVehicleClass::Sports, 140000, 312, 68, 84, "Germany");
	// M5
	CAR("m5_e39", "BMW", "M5 E39", 2000, EVehicleClass::Sports, 25000, 300, 52, 70, "Germany");
	CAR("m5_e60", "BMW", "M5 E60", 2007, EVehicleClass::Sports, 20000, 330, 62, 68, "Germany");
	CAR("m5_cs", "BMW", "M5 CS", 2022, EVehicleClass::Sports, 140000, 305, 72, 76, "Germany");
	// M8
	CAR("m8_comp", "BMW", "M8 Competition", 2022, EVehicleClass::Sports, 140000, 305, 70, 76, "Germany");
	// Special
	CAR("1m_coupe", "BMW", "1M Coupé", 2011, EVehicleClass::Sports, 50000, 280, 58, 84, "Germany");
	CAR("m1", "BMW", "M1", 1980, EVehicleClass::Classic, 350000, 260, 50, 68, "Germany");
	CAR("z8", "BMW", "Z8", 2001, EVehicleClass::Classic, 220000, 250, 50, 72, "Germany");
	CAR("i8", "BMW", "i8", 2019, EVehicleClass::Sports, 80000, 250, 55, 80, "Germany");
	// X M
	CAR("x5m_comp", "BMW", "X5 M Competition", 2023, EVehicleClass::SUV, 120000, 290, 60, 58, "Germany");
	CAR("x6m_comp", "BMW", "X6 M Competition", 2023, EVehicleClass::SUV, 125000, 290, 60, 56, "Germany");
	// Z
	CAR("z3m_coupe", "BMW", "Z3 M Coupé", 2001, EVehicleClass::Sports, 35000, 270, 50, 76, "Germany");
	CAR("z4m_coupe", "BMW", "Z4 M Coupé", 2007, EVehicleClass::Sports, 30000, 282, 52, 78, "Germany");
	// Classics (pre-war)
	CAR("328_roadster", "BMW", "328 Roadster", 1938, EVehicleClass::Classic, 550000, 155, 22, 58, "Germany");
	CAR("507_roadster", "BMW", "507 Roadster", 1958, EVehicleClass::Classic, 2200000, 200, 35, 62, "Germany");
	CAR("isetta_300", "BMW", "Isetta 300", 1957, EVehicleClass::Classic, 25000, 85, 8, 40, "Germany");
	// Electric / PHEV
	CAR("i3", "BMW", "i3", 2015, EVehicleClass::Compact, 25000, 150, 42, 72, "Germany");
	CAR("i4_m50", "BMW", "i4 M50", 2023, EVehicleClass::Sports, 70000, 225, 58, 78, "Germany");
	CAR("i5_m60", "BMW", "i5 M60", 2024, EVehicleClass::Sports, 85000, 230, 58, 72, "Germany");
	CAR("i7_m70", "BMW", "i7 M70", 2024, EVehicleClass::Sedan, 170000, 250, 52, 62, "Germany");
	CAR("ix_m60", "BMW", "iX M60", 2023, EVehicleClass::SUV, 110000, 250, 55, 62, "Germany");
	CAR("xm", "BMW", "XM", 2023, EVehicleClass::SUV, 160000, 270, 58, 58, "Germany");
	// X Series
	CAR("x1", "BMW", "X1 xDrive28i", 2023, EVehicleClass::SUV, 40000, 220, 42, 62, "Germany");
	CAR("x3_m40i", "BMW", "X3 M40i", 2023, EVehicleClass::SUV, 62000, 250, 52, 65, "Germany");
	CAR("x4_m40i", "BMW", "X4 M40i", 2023, EVehicleClass::SUV, 65000, 250, 52, 64, "Germany");
	CAR("x7_m60i", "BMW", "X7 M60i", 2023, EVehicleClass::SUV, 105000, 250, 55, 58, "Germany");
	// M4 convertible
	CAR("m4_convertible", "BMW", "M4 Competition Convertible", 2023, EVehicleClass::Sports, 86000, 280, 58, 80, "Germany");
	// Additional specials
	CAR("m5_touring_g99", "BMW", "M5 Touring", 2025, EVehicleClass::Sports, 120000, 305, 62, 72, "Germany");
	CAR("m2_g87_comp", "BMW", "M2 Competition", 2023, EVehicleClass::Sports, 65000, 290, 62, 84, "Germany");
}

void AddFerrariComplete(TArray<FVehicleDef>& Catalog)
{
	FVehicleDef V;
	CAR("250_gto", "Ferrari", "250 GTO", 1962, EVehicleClass::Classic, 48000000, 280, 48, 68, "Italy");
	CAR("250_lm", "Ferrari", "250 LM", 1964, EVehicleClass::Classic, 15000000, 290, 50, 65, "Italy");
	CAR("275_gtb4", "Ferrari", "275 GTB/4", 1967, EVehicleClass::Classic, 3000000, 270, 48, 65, "Italy");
	CAR("330_p4", "Ferrari", "330 P4", 1967, EVehicleClass::Classic, 12000000, 320, 55, 68, "Italy");
	CAR("365_gtb4", "Ferrari", "365 GTB/4 Daytona", 1971, EVehicleClass::Classic, 800000, 280, 50, 62, "Italy");
	CAR("dino_246_gt", "Ferrari", "Dino 246 GT", 1972, EVehicleClass::Classic, 300000, 230, 45, 78, "Italy");
	CAR("308_gts", "Ferrari", "308 GTS", 1982, EVehicleClass::Classic, 60000, 255, 42, 72, "Italy");
	CAR("288_gto", "Ferrari", "288 GTO", 1985, EVehicleClass::Super, 3000000, 304, 58, 68, "Italy");
	CAR("testarossa", "Ferrari", "Testarossa", 1986, EVehicleClass::Classic, 120000, 290, 55, 62, "Italy");
	CAR("f40", "Ferrari", "F40", 1988, EVehicleClass::Super, 1800000, 324, 78, 75, "Italy");
	CAR("f355", "Ferrari", "F355 Berlinetta", 1997, EVehicleClass::Sports, 60000, 295, 54, 75, "Italy");
	CAR("550_maranello", "Ferrari", "550 Maranello", 2000, EVehicleClass::Sports, 90000, 320, 55, 70, "Italy");
	CAR("360_modena", "Ferrari", "360 Modena", 2003, EVehicleClass::Sports, 55000, 295, 54, 76, "Italy");
	CAR("360_cs", "Ferrari", "360 Challenge Stradale", 2005, EVehicleClass::Super, 150000, 300, 58, 82, "Italy");
	CAR("f430", "Ferrari", "F430", 2007, EVehicleClass::Super, 80000, 315, 62, 78, "Italy");
	CAR("430_scuderia", "Ferrari", "430 Scuderia", 2008, EVehicleClass::Super, 180000, 320, 68, 84, "Italy");
	CAR("f50", "Ferrari", "F50", 1996, EVehicleClass::Super, 2200000, 325, 76, 78, "Italy");
	CAR("enzo", "Ferrari", "Enzo", 2003, EVehicleClass::Super, 2800000, 350, 82, 80, "Italy");
	CAR("599_gto", "Ferrari", "599 GTO", 2011, EVehicleClass::Super, 350000, 335, 72, 74, "Italy");
	CAR("458_speciale", "Ferrari", "458 Speciale", 2015, EVehicleClass::Super, 280000, 325, 74, 86, "Italy");
	CAR("f12_tdf", "Ferrari", "F12 tdf", 2017, EVehicleClass::Super, 700000, 340, 76, 76, "Italy");
	CAR("laferrari", "Ferrari", "LaFerrari", 2015, EVehicleClass::Hypercar, 3500000, 352, 88, 84, "Italy");
	CAR("laferrari_aperta", "Ferrari", "LaFerrari Aperta", 2017, EVehicleClass::Hypercar, 5000000, 352, 88, 82, "Italy");
	CAR("812_superfast", "Ferrari", "812 Superfast", 2020, EVehicleClass::Super, 350000, 340, 72, 78, "Italy");
	CAR("812_comp", "Ferrari", "812 Competizione", 2022, EVehicleClass::Super, 650000, 340, 78, 80, "Italy");
	CAR("sf90_stradale", "Ferrari", "SF90 Stradale", 2022, EVehicleClass::Hypercar, 500000, 340, 90, 86, "Italy");
	CAR("296_gtb", "Ferrari", "296 GTB", 2023, EVehicleClass::Super, 320000, 330, 74, 84, "Italy");
	CAR("purosangue", "Ferrari", "Purosangue", 2023, EVehicleClass::SUV, 400000, 310, 68, 68, "Italy");
	CAR("roma", "Ferrari", "Roma", 2022, EVehicleClass::Sports, 230000, 320, 68, 80, "Italy");
	CAR("sp3_daytona", "Ferrari", "SP3 Daytona", 2023, EVehicleClass::Super, 2200000, 340, 78, 80, "Italy");
	CAR("12cilindri", "Ferrari", "12Cilindri", 2024, EVehicleClass::Super, 400000, 340, 72, 76, "Italy");
	CAR("f80", "Ferrari", "F80", 2025, EVehicleClass::Hypercar, 3200000, 350, 92, 86, "Italy");
	CAR("f8_tributo", "Ferrari", "F8 Tributo", 2020, EVehicleClass::Super, 280000, 340, 74, 84, "Italy");
	CAR("488_pista", "Ferrari", "488 Pista", 2019, EVehicleClass::Super, 350000, 340, 76, 86, "Italy");
	CAR("375_mm", "Ferrari", "375 MM", 1954, EVehicleClass::Classic, 5000000, 250, 30, 55, "Italy");
	CAR("410_superamerica", "Ferrari", "410 Superamerica", 1958, EVehicleClass::Classic, 3500000, 260, 32, 52, "Italy");
	CAR("250_gt_swb", "Ferrari", "250 GT SWB", 1960, EVehicleClass::Classic, 8000000, 260, 45, 62, "Italy");
	CAR("monza_sp2", "Ferrari", "Monza SP2", 2020, EVehicleClass::Super, 1800000, 300, 60, 76, "Italy");
	CAR("roma_spider", "Ferrari", "Roma Spider", 2023, EVehicleClass::Sports, 260000, 320, 68, 78, "Italy");
	CAR("250_california", "Ferrari", "250 GT California Spyder", 1961, EVehicleClass::Classic, 12000000, 250, 42, 60, "Italy");
	CAR("amalfi", "Ferrari", "Amalfi", 2025, EVehicleClass::Sports, 280000, 330, 68, 80, "Italy");
	CAR("849_testarossa", "Ferrari", "849 Testarossa", 2025, EVehicleClass::Super, 500000, 340, 78, 84, "Italy");
	CAR("luce", "Ferrari", "Luce", 2026, EVehicleClass::Sedan, 300000, 250, 62, 72, "Italy");
}

void AddLamborghiniComplete(TArray<FVehicleDef>& Catalog)
{
	FVehicleDef V;
	CAR("350gt", "Lamborghini", "350 GT", 1965, EVehicleClass::Classic, 600000, 250, 30, 55, "Italy");
	CAR("miura_sv", "Lamborghini", "Miura SV", 1971, EVehicleClass::Classic, 2000000, 290, 52, 60, "Italy");
	CAR("espada", "Lamborghini", "Espada Series III", 1974, EVehicleClass::Classic, 120000, 250, 32, 52, "Italy");
	CAR("countach_lp400", "Lamborghini", "Countach LP400", 1976, EVehicleClass::Classic, 600000, 288, 45, 52, "Italy");
	CAR("countach_5000qv", "Lamborghini", "Countach 5000 QV", 1986, EVehicleClass::Classic, 250000, 300, 48, 54, "Italy");
	CAR("countach_25th", "Lamborghini", "Countach 25th Anniversary", 1989, EVehicleClass::Classic, 250000, 295, 48, 54, "Italy");
	CAR("lm002", "Lamborghini", "LM002", 1988, EVehicleClass::SUV, 80000, 210, 32, 40, "Italy");
	CAR("diablo_sv", "Lamborghini", "Diablo SV", 1997, EVehicleClass::Super, 180000, 328, 62, 64, "Italy");
	CAR("diablo_gt", "Lamborghini", "Diablo GT", 2000, EVehicleClass::Super, 500000, 338, 65, 66, "Italy");
	CAR("murcielago_lp640", "Lamborghini", "Murciélago LP640", 2007, EVehicleClass::Super, 150000, 340, 65, 68, "Italy");
	CAR("murcielago_sv", "Lamborghini", "Murciélago SV", 2010, EVehicleClass::Super, 280000, 342, 72, 70, "Italy");
	CAR("reventon", "Lamborghini", "Reventón", 2008, EVehicleClass::Hypercar, 1500000, 340, 68, 72, "Italy");
	CAR("gallardo_lp560", "Lamborghini", "Gallardo LP560-4", 2009, EVehicleClass::Super, 100000, 325, 68, 78, "Italy");
	CAR("gallardo_superleggera", "Lamborghini", "Gallardo Superleggera", 2008, EVehicleClass::Super, 120000, 315, 64, 82, "Italy");
	CAR("gallardo_squadra", "Lamborghini", "Gallardo Squadra Corse", 2014, EVehicleClass::Super, 200000, 320, 66, 84, "Italy");
	CAR("aventador_svj", "Lamborghini", "Aventador SVJ", 2020, EVehicleClass::Super, 500000, 350, 84, 76, "Italy");
	CAR("huracan_performante", "Lamborghini", "Huracán Performante", 2018, EVehicleClass::Super, 250000, 325, 76, 86, "Italy");
	CAR("huracan_sto", "Lamborghini", "Huracán STO", 2022, EVehicleClass::Super, 330000, 325, 82, 84, "Italy");
	CAR("huracan_sterrato", "Lamborghini", "Huracán Sterrato", 2023, EVehicleClass::Rally, 280000, 260, 62, 76, "Italy");
	CAR("veneno", "Lamborghini", "Veneno", 2014, EVehicleClass::Hypercar, 4500000, 355, 78, 76, "Italy");
	CAR("centenario", "Lamborghini", "Centenario", 2017, EVehicleClass::Hypercar, 2000000, 350, 76, 78, "Italy");
	CAR("sian", "Lamborghini", "Sián FKP 37", 2020, EVehicleClass::Hypercar, 3600000, 350, 76, 78, "Italy");
	CAR("countach_lpi", "Lamborghini", "Countach LPI 800-4", 2022, EVehicleClass::Hypercar, 2640000, 355, 76, 76, "Italy");
	CAR("revuelto", "Lamborghini", "Revuelto", 2024, EVehicleClass::Hypercar, 600000, 350, 78, 80, "Italy");
	CAR("temerario", "Lamborghini", "Temerario", 2025, EVehicleClass::Hypercar, 380000, 343, 75, 82, "Italy");
	CAR("urus_performante", "Lamborghini", "Urus Performante", 2023, EVehicleClass::SUV, 260000, 306, 68, 68, "Italy");
	CAR("urus_se", "Lamborghini", "Urus SE", 2025, EVehicleClass::SUV, 280000, 305, 62, 68, "Italy");
	// One-offs & limited
	CAR("sesto_elemento", "Lamborghini", "Sesto Elemento", 2012, EVehicleClass::Hypercar, 2500000, 355, 88, 84, "Italy");
	CAR("sc18_alston", "Lamborghini", "SC18 Alston", 2018, EVehicleClass::Hypercar, 5000000, 350, 80, 78, "Italy");
	CAR("sc20", "Lamborghini", "SC20", 2021, EVehicleClass::Hypercar, 6000000, 350, 78, 76, "Italy");
	CAR("essenza_scv12", "Lamborghini", "Essenza SCV12", 2020, EVehicleClass::Hypercar, 2800000, 340, 84, 80, "Italy");
	CAR("invencible", "Lamborghini", "Invencible", 2023, EVehicleClass::Hypercar, 5000000, 355, 80, 78, "Italy");
	CAR("autentica", "Lamborghini", "Autentica", 2023, EVehicleClass::Hypercar, 5000000, 355, 78, 78, "Italy");
	CAR("fenomeno", "Lamborghini", "Fenomeno", 2025, EVehicleClass::Hypercar, 380000, 350, 80, 78, "Italy");
	// Additional variants
	CAR("aventador_sv", "Lamborghini", "Aventador SV LP750-4", 2016, EVehicleClass::Super, 400000, 350, 78, 76, "Italy");
	CAR("aventador_s", "Lamborghini", "Aventador S LP740-4", 2018, EVehicleClass::Super, 350000, 350, 78, 76, "Italy");
	CAR("aventador_ultimae", "Lamborghini", "Aventador LP780-4 Ultimae", 2022, EVehicleClass::Super, 550000, 355, 80, 76, "Italy");
	CAR("huracan_evo_rwd", "Lamborghini", "Huracan EVO RWD", 2020, EVehicleClass::Super, 210000, 325, 68, 84, "Italy");
	CAR("gallardo_lp550_valentino", "Lamborghini", "Gallardo LP550-2 Balboni", 2010, EVehicleClass::Super, 130000, 320, 62, 82, "Italy");
	CAR("diablo_se30_jota", "Lamborghini", "Diablo SE30 Jota", 1995, EVehicleClass::Super, 300000, 340, 65, 66, "Italy");
	CAR("diablo_vt_60", "Lamborghini", "Diablo VT 6.0", 2000, EVehicleClass::Super, 200000, 335, 62, 64, "Italy");
	CAR("murcielago_40th", "Lamborghini", "Murcielago 40th Anniversary", 2004, EVehicleClass::Super, 180000, 330, 62, 66, "Italy");
	CAR("countach_evoluzione", "Lamborghini", "Countach Evoluzione", 1987, EVehicleClass::Classic, 800000, 330, 55, 52, "Italy");
	CAR("miura_jota", "Lamborghini", "Miura Jota", 1970, EVehicleClass::Classic, 3500000, 300, 55, 60, "Italy");
}

void AddAstonMartinComplete(TArray<FVehicleDef>& Catalog)
{
	FVehicleDef V;
	CAR("db5", "Aston Martin", "DB5", 1964, EVehicleClass::Classic, 650000, 230, 40, 62, "UK");
	CAR("v8_vantage_77", "Aston Martin", "V8 Vantage", 1977, EVehicleClass::Classic, 80000, 265, 42, 55, "UK");
	CAR("db7_vantage", "Aston Martin", "DB7 Vantage", 2000, EVehicleClass::Sports, 25000, 296, 52, 68, "UK");
	CAR("v12_vanquish_s", "Aston Martin", "V12 Vanquish S", 2005, EVehicleClass::Sports, 55000, 320, 58, 68, "UK");
	CAR("db9", "Aston Martin", "DB9", 2008, EVehicleClass::Sports, 35000, 306, 55, 72, "UK");
	CAR("dbs_08", "Aston Martin", "DBS", 2008, EVehicleClass::Super, 60000, 307, 60, 72, "UK");
	CAR("one_77", "Aston Martin", "One-77", 2011, EVehicleClass::Hypercar, 1500000, 354, 72, 74, "UK");
	CAR("v12_vantage", "Aston Martin", "V12 Vantage", 2013, EVehicleClass::Super, 90000, 305, 62, 72, "UK");
	CAR("vanquish_2014", "Aston Martin", "Vanquish", 2014, EVehicleClass::Super, 150000, 324, 68, 74, "UK");
	CAR("db11", "Aston Martin", "DB11", 2018, EVehicleClass::Sports, 180000, 322, 68, 78, "UK");
	CAR("dbs_superleggera", "Aston Martin", "DBS Superleggera", 2020, EVehicleClass::Super, 300000, 340, 72, 76, "UK");
	CAR("vantage_f1", "Aston Martin", "Vantage F1 Edition", 2023, EVehicleClass::Sports, 160000, 314, 64, 82, "UK");
	CAR("dbx707", "Aston Martin", "DBX707", 2023, EVehicleClass::SUV, 240000, 310, 62, 62, "UK");
	CAR("db12", "Aston Martin", "DB12", 2024, EVehicleClass::Sports, 250000, 325, 68, 78, "UK");
	CAR("valkyrie", "Aston Martin", "Valkyrie", 2023, EVehicleClass::Hypercar, 3200000, 362, 94, 88, "UK");
	CAR("vulcan", "Aston Martin", "Vulcan", 2016, EVehicleClass::Hypercar, 2300000, 330, 80, 82, "UK");
	CAR("victor", "Aston Martin", "Victor", 2021, EVehicleClass::Super, 3000000, 320, 68, 72, "UK");
	// Pre-war
	CAR("standard_sports", "Aston Martin", "Standard Sports", 1923, EVehicleClass::Classic, 400000, 120, 12, 40, "UK");
	CAR("ulster", "Aston Martin", "Ulster", 1935, EVehicleClass::Classic, 800000, 165, 22, 48, "UK");
	CAR("international", "Aston Martin", "International", 1930, EVehicleClass::Classic, 350000, 140, 18, 45, "UK");
	// Post-war
	CAR("db1", "Aston Martin", "DB1 2-Litre Sports", 1949, EVehicleClass::Classic, 200000, 150, 20, 50, "UK");
	CAR("db2_vantage", "Aston Martin", "DB2 Vantage", 1952, EVehicleClass::Classic, 300000, 185, 28, 52, "UK");
	CAR("db4_gt", "Aston Martin", "DB4 GT", 1960, EVehicleClass::Classic, 800000, 246, 42, 58, "UK");
	CAR("db6_mk2", "Aston Martin", "DB6 Mk2", 1969, EVehicleClass::Classic, 200000, 230, 38, 55, "UK");
	CAR("dbs_1968", "Aston Martin", "DBS", 1969, EVehicleClass::Classic, 180000, 240, 38, 55, "UK");
	CAR("v8_vantage_93", "Aston Martin", "V8 Vantage", 1993, EVehicleClass::Classic, 60000, 275, 42, 58, "UK");
	CAR("v8_vantage_zagato", "Aston Martin", "V8 Zagato", 1988, EVehicleClass::Classic, 200000, 300, 48, 55, "UK");
	// Special/Limited
	CAR("lagonda", "Aston Martin", "Lagonda", 1978, EVehicleClass::Classic, 80000, 230, 35, 50, "UK");
	CAR("cygnet", "Aston Martin", "Cygnet", 2012, EVehicleClass::Compact, 35000, 170, 25, 60, "UK");
	CAR("rapide_s", "Aston Martin", "Rapide S", 2015, EVehicleClass::Sports, 100000, 327, 62, 68, "UK");
	CAR("v12_zagato", "Aston Martin", "V12 Zagato", 2013, EVehicleClass::Super, 400000, 310, 58, 68, "UK");
	CAR("db10", "Aston Martin", "DB10", 2015, EVehicleClass::Sports, 3000000, 310, 62, 78, "UK");
	CAR("cc100", "Aston Martin", "CC100 Speedster", 2013, EVehicleClass::Sports, 2000000, 290, 55, 72, "UK");
	CAR("v12_speedster", "Aston Martin", "V12 Speedster", 2021, EVehicleClass::Sports, 950000, 300, 58, 72, "UK");
	CAR("dbs_770", "Aston Martin", "DBS 770 Ultimate", 2023, EVehicleClass::Super, 350000, 340, 72, 74, "UK");
	CAR("valour", "Aston Martin", "Valour", 2024, EVehicleClass::Super, 1500000, 320, 62, 72, "UK");
	CAR("valiant", "Aston Martin", "Valiant", 2025, EVehicleClass::Super, 2000000, 330, 65, 72, "UK");
	CAR("valkyrie_amr_pro", "Aston Martin", "Valkyrie AMR Pro", 2023, EVehicleClass::Hypercar, 4000000, 360, 94, 86, "UK");
	CAR("vanquish_2024", "Aston Martin", "Vanquish", 2024, EVehicleClass::Super, 350000, 345, 72, 76, "UK");
}

void AddAudiComplete(TArray<FVehicleDef>& Catalog)
{
	FVehicleDef V;
	// Current
	CAR("a3_sedan", "Audi", "A3 Sedan", 2024, EVehicleClass::Sedan, 36000, 240, 45, 72, "Germany");
	CAR("a4_45", "Audi", "A4 45 TFSI", 2024, EVehicleClass::Sedan, 45000, 250, 48, 70, "Germany");
	CAR("a5_sportback", "Audi", "A5 Sportback B10", 2025, EVehicleClass::Sports, 52000, 250, 52, 74, "Germany");
	CAR("a6_c9", "Audi", "A6 C9", 2025, EVehicleClass::Sedan, 62000, 250, 55, 68, "Germany");
	CAR("a7", "Audi", "A7 Sportback", 2023, EVehicleClass::Sports, 72000, 250, 55, 72, "Germany");
	CAR("a8_d5", "Audi", "A8 L", 2023, EVehicleClass::Sedan, 88000, 250, 50, 65, "Germany");
	// Q Series
	CAR("q3_35", "Audi", "Q3 35 TFSI", 2025, EVehicleClass::SUV, 38000, 220, 42, 62, "Germany");
	CAR("q5_45", "Audi", "Q5 45 TFSI", 2024, EVehicleClass::SUV, 48000, 240, 48, 65, "Germany");
	CAR("q7_55", "Audi", "Q7 55 TFSI", 2023, EVehicleClass::SUV, 65000, 250, 52, 58, "Germany");
	CAR("q8_55", "Audi", "Q8 55 TFSI", 2023, EVehicleClass::SUV, 72000, 250, 55, 60, "Germany");
	// Electric
	CAR("etron_gt", "Audi", "e-tron GT RS", 2024, EVehicleClass::Sports, 145000, 250, 72, 78, "Germany");
	CAR("q4_etron", "Audi", "Q4 e-tron 50", 2023, EVehicleClass::SUV, 55000, 180, 50, 62, "Germany");
	CAR("q6_etron", "Audi", "Q6 e-tron", 2025, EVehicleClass::SUV, 75000, 230, 52, 62, "Germany");
	// S/RS
	CAR("rs3_8y", "Audi", "RS3 Sportback", 2023, EVehicleClass::Sports, 65000, 280, 62, 80, "Germany");
	CAR("rs4_avant", "Audi", "RS4 Avant", 2023, EVehicleClass::Sports, 82000, 280, 58, 74, "Germany");
	CAR("rs5_sportback", "Audi", "RS5 Sportback", 2023, EVehicleClass::Sports, 80000, 280, 58, 72, "Germany");
	CAR("rs6_avant", "Audi", "RS6 Avant", 2023, EVehicleClass::Sports, 115000, 305, 62, 70, "Germany");
	CAR("rs7", "Audi", "RS7 Sportback", 2023, EVehicleClass::Sports, 125000, 305, 62, 68, "Germany");
	CAR("s8", "Audi", "S8", 2023, EVehicleClass::Sports, 125000, 305, 58, 62, "Germany");
	// R8
	CAR("r8_v10", "Audi", "R8 V10 Performance", 2023, EVehicleClass::Super, 195000, 330, 82, 84, "Germany");
	CAR("r8_gt_rwd", "Audi", "R8 GT RWD", 2024, EVehicleClass::Super, 250000, 320, 78, 86, "Germany");
	// TT
	CAR("tts_mk3", "Audi", "TTS", 2022, EVehicleClass::Sports, 55000, 280, 52, 82, "Germany");
	CAR("tt_rs", "Audi", "TT RS", 2023, EVehicleClass::Sports, 72000, 280, 58, 80, "Germany");
	// Classics
	CAR("quattro_20v", "Audi", "Quattro 20V", 1990, EVehicleClass::Classic, 80000, 240, 42, 72, "Germany");
	CAR("sport_quattro_s1", "Audi", "Sport Quattro S1", 1985, EVehicleClass::Rally, 400000, 250, 55, 75, "Germany");
	CAR("v8_quattro", "Audi", "V8 Quattro", 1991, EVehicleClass::Classic, 15000, 250, 40, 62, "Germany");
	CAR("s2_coupe", "Audi", "S2 Coupé", 1993, EVehicleClass::Sports, 20000, 250, 42, 68, "Germany");
	CAR("rs2_avant", "Audi", "RS2 Avant", 1995, EVehicleClass::Sports, 50000, 262, 48, 68, "Germany");
	CAR("a2", "Audi", "A2", 2003, EVehicleClass::Compact, 5000, 188, 32, 65, "Germany");
	// Race
	CAR("r10_tdi", "Audi", "R10 TDI Le Mans", 2006, EVehicleClass::Hypercar, 2000000, 340, 78, 80, "Germany");
	CAR("r18_etron", "Audi", "R18 e-tron quattro", 2015, EVehicleClass::Hypercar, 2500000, 350, 84, 84, "Germany");
}
