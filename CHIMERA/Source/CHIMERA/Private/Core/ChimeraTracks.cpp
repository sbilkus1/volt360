#include "Core/ChimeraTracks.h"

static TArray<FChimeraTrack> BuildRealTracks()
{
	TArray<FChimeraTrack> T;
	// === 2025 F1 calendar — all 24 real circuits ===
	FChimeraTrack t; t.bReal = true;
	t.Name = TEXT("Albert Park");           t.Location = TEXT("Melbourne");       t.Country = TEXT("Australia");    t.LapKm = 5.278f; t.Corners = 14; t.LapCount = 3; t.Difficulty = 0.55f; T.Add(t);
	t.Name = TEXT("Shanghai International"); t.Location = TEXT("Shanghai");        t.Country = TEXT("China");        t.LapKm = 5.451f; t.Corners = 16; t.LapCount = 3; t.Difficulty = 0.50f; T.Add(t);
	t.Name = TEXT("Suzuka");                t.Location = TEXT("Suzuka");          t.Country = TEXT("Japan");        t.LapKm = 5.807f; t.Corners = 18; t.LapCount = 3; t.Difficulty = 0.75f; T.Add(t);
	t.Name = TEXT("Bahrain International");  t.Location = TEXT("Sakhir");          t.Country = TEXT("Bahrain");      t.LapKm = 5.412f; t.Corners = 15; t.LapCount = 3; t.Difficulty = 0.45f; T.Add(t);
	t.Name = TEXT("Jeddah Corniche");        t.Location = TEXT("Jeddah");          t.Country = TEXT("Saudi Arabia"); t.LapKm = 6.174f; t.Corners = 27; t.LapCount = 2; t.Difficulty = 0.85f; T.Add(t);
	t.Name = TEXT("Miami International");    t.Location = TEXT("Miami");           t.Country = TEXT("USA");          t.LapKm = 5.412f; t.Corners = 19; t.LapCount = 3; t.Difficulty = 0.55f; T.Add(t);
	t.Name = TEXT("Imola");                 t.Location = TEXT("Imola");           t.Country = TEXT("Italy");        t.LapKm = 4.909f; t.Corners = 19; t.LapCount = 3; t.Difficulty = 0.65f; T.Add(t);
	t.Name = TEXT("Circuit de Monaco");      t.Location = TEXT("Monte Carlo");      t.Country = TEXT("Monaco");       t.LapKm = 3.337f; t.Corners = 19; t.LapCount = 4; t.Difficulty = 0.95f; T.Add(t);
	t.Name = TEXT("Circuit Gilles Villeneuve"); t.Location = TEXT("Montreal");     t.Country = TEXT("Canada");       t.LapKm = 4.361f; t.Corners = 14; t.LapCount = 3; t.Difficulty = 0.60f; T.Add(t);
	t.Name = TEXT("Barcelona-Catalunya");    t.Location = TEXT("Barcelona");       t.Country = TEXT("Spain");        t.LapKm = 4.675f; t.Corners = 16; t.LapCount = 3; t.Difficulty = 0.55f; T.Add(t);
	t.Name = TEXT("Red Bull Ring");         t.Location = TEXT("Spielberg");       t.Country = TEXT("Austria");      t.LapKm = 4.318f; t.Corners = 10; t.LapCount = 3; t.Difficulty = 0.40f; T.Add(t);
	t.Name = TEXT("Silverstone");           t.Location = TEXT("Silverstone");     t.Country = TEXT("UK");           t.LapKm = 5.891f; t.Corners = 18; t.LapCount = 2; t.Difficulty = 0.70f; T.Add(t);
	t.Name = TEXT("Hungaroring");           t.Location = TEXT("Budapest");        t.Country = TEXT("Hungary");      t.LapKm = 4.381f; t.Corners = 14; t.LapCount = 3; t.Difficulty = 0.60f; T.Add(t);
	t.Name = TEXT("Spa-Francorchamps");      t.Location = TEXT("Spa");             t.Country = TEXT("Belgium");      t.LapKm = 7.004f; t.Corners = 19; t.LapCount = 2; t.Difficulty = 0.85f; T.Add(t);
	t.Name = TEXT("Zandvoort");             t.Location = TEXT("Zandvoort");       t.Country = TEXT("Netherlands");  t.LapKm = 4.259f; t.Corners = 14; t.LapCount = 3; t.Difficulty = 0.65f; T.Add(t);
	t.Name = TEXT("Monza");                 t.Location = TEXT("Monza");           t.Country = TEXT("Italy");        t.LapKm = 5.793f; t.Corners = 11; t.LapCount = 3; t.Difficulty = 0.50f; T.Add(t);
	t.Name = TEXT("Baku City");             t.Location = TEXT("Baku");            t.Country = TEXT("Azerbaijan");   t.LapKm = 6.003f; t.Corners = 20; t.LapCount = 2; t.Difficulty = 0.75f; T.Add(t);
	t.Name = TEXT("Marina Bay");            t.Location = TEXT("Singapore");       t.Country = TEXT("Singapore");    t.LapKm = 4.940f; t.Corners = 19; t.LapCount = 3; t.Difficulty = 0.80f; T.Add(t);
	t.Name = TEXT("Circuit of the Americas"); t.Location = TEXT("Austin");        t.Country = TEXT("USA");          t.LapKm = 5.513f; t.Corners = 20; t.LapCount = 3; t.Difficulty = 0.65f; T.Add(t);
	t.Name = TEXT("Autódromo Hermanos Rodríguez"); t.Location = TEXT("Mexico City"); t.Country = TEXT("Mexico");    t.LapKm = 4.304f; t.Corners = 17; t.LapCount = 3; t.Difficulty = 0.50f; T.Add(t);
	t.Name = TEXT("Interlagos");            t.Location = TEXT("São Paulo");       t.Country = TEXT("Brazil");       t.LapKm = 4.309f; t.Corners = 15; t.LapCount = 3; t.Difficulty = 0.65f; T.Add(t);
	t.Name = TEXT("Las Vegas Strip");       t.Location = TEXT("Las Vegas");       t.Country = TEXT("USA");          t.LapKm = 6.201f; t.Corners = 17; t.LapCount = 2; t.Difficulty = 0.70f; T.Add(t);
	t.Name = TEXT("Lusail International");  t.Location = TEXT("Lusail");          t.Country = TEXT("Qatar");        t.LapKm = 5.419f; t.Corners = 16; t.LapCount = 3; t.Difficulty = 0.55f; T.Add(t);
	t.Name = TEXT("Yas Marina");            t.Location = TEXT("Abu Dhabi");       t.Country = TEXT("UAE");          t.LapKm = 5.281f; t.Corners = 16; t.LapCount = 3; t.Difficulty = 0.50f; T.Add(t);
	return T;
}

static TArray<FChimeraTrack> BuildCustomTracks()
{
	TArray<FChimeraTrack> T;
	FChimeraTrack t; t.bReal = false;
	// One custom circuit per CHIMERA ring + bonus tracks.
	t.Name = TEXT("Nexus Ring Circuit");     t.Location = TEXT("The Nexus");       t.Country = TEXT("CHIMERA"); t.LapKm = 4.0f;  t.Corners = 12; t.LapCount = 3; t.Difficulty = 0.40f; T.Add(t);
	t.Name = TEXT("Aether City Skyway");      t.Location = TEXT("Aether City");     t.Country = TEXT("CHIMERA"); t.LapKm = 8.0f;  t.Corners = 22; t.LapCount = 2; t.Difficulty = 0.80f; T.Add(t);
	t.Name = TEXT("Neo-Kingdom Royal GP");    t.Location = TEXT("Neo-Kingdom");     t.Country = TEXT("CHIMERA"); t.LapKm = 3.5f;  t.Corners = 14; t.LapCount = 3; t.Difficulty = 0.50f; T.Add(t);
	t.Name = TEXT("Mercy Heights Hospital Loop"); t.Location = TEXT("Mercy Heights"); t.Country = TEXT("CHIMERA"); t.LapKm = 2.8f; t.Corners = 10; t.LapCount = 4; t.Difficulty = 0.35f; T.Add(t);
	t.Name = TEXT("Mid-Wilshire Street GP");   t.Location = TEXT("Mid-Wilshire");   t.Country = TEXT("CHIMERA"); t.LapKm = 6.0f;  t.Corners = 20; t.LapCount = 2; t.Difficulty = 0.65f; T.Add(t);
	t.Name = TEXT("Alagaësia Dragon Circuit"); t.Location = TEXT("Alagaesia");      t.Country = TEXT("CHIMERA"); t.LapKm = 9.0f;  t.Corners = 25; t.LapCount = 2; t.Difficulty = 0.90f; T.Add(t);
	t.Name = TEXT("Frontier Voxel GP");       t.Location = TEXT("Minecraft Frontier"); t.Country = TEXT("CHIMERA"); t.LapKm = 5.0f; t.Corners = 16; t.LapCount = 3; t.Difficulty = 0.55f; T.Add(t);
	t.Name = TEXT("Grand Coliseum Speedway");  t.Location = TEXT("Grand Coliseum"); t.Country = TEXT("CHIMERA"); t.LapKm = 4.5f;  t.Corners = 8;  t.LapCount = 4; t.Difficulty = 0.45f; T.Add(t);
	t.Name = TEXT("Night City Expanse GP");    t.Location = TEXT("Night City");     t.Country = TEXT("CHIMERA"); t.LapKm = 7.0f;  t.Corners = 24; t.LapCount = 2; t.Difficulty = 0.85f; T.Add(t);
	t.Name = TEXT("Seattle Grey-Sloan GP");    t.Location = TEXT("Seattle");        t.Country = TEXT("CHIMERA"); t.LapKm = 4.2f;  t.Corners = 15; t.LapCount = 3; t.Difficulty = 0.50f; T.Add(t);
	t.Name = TEXT("Chicago Windy City GP");    t.Location = TEXT("Chicago");        t.Country = TEXT("CHIMERA"); t.LapKm = 5.5f;  t.Corners = 18; t.LapCount = 3; t.Difficulty = 0.60f; T.Add(t);
	t.Name = TEXT("Chastain Park Memorial GP"); t.Location = TEXT("Chastain Park"); t.Country = TEXT("CHIMERA"); t.LapKm = 3.8f;  t.Corners = 14; t.LapCount = 3; t.Difficulty = 0.45f; T.Add(t);
	t.Name = TEXT("Cleveleys Seafront GP");    t.Location = TEXT("Cleveleys");      t.Country = TEXT("CHIMERA"); t.LapKm = 3.2f;  t.Corners = 12; t.LapCount = 3; t.Difficulty = 0.40f; T.Add(t);
	t.Name = TEXT("Vernet-les-Bains Mountain GP"); t.Location = TEXT("Vernet-les-Bains"); t.Country = TEXT("CHIMERA"); t.LapKm = 6.5f; t.Corners = 30; t.LapCount = 2; t.Difficulty = 0.88f; T.Add(t);
	t.Name = TEXT("Panem Capitol GP");         t.Location = TEXT("Panem");          t.Country = TEXT("CHIMERA"); t.LapKm = 7.5f;  t.Corners = 26; t.LapCount = 2; t.Difficulty = 0.82f; T.Add(t);
	return T;
}

static const TArray<FChimeraTrack>& Real() { static const TArray<FChimeraTrack> R = BuildRealTracks(); return R; }
static const TArray<FChimeraTrack>& Custom() { static const TArray<FChimeraTrack> C = BuildCustomTracks(); return C; }

const TArray<FChimeraTrack>& FChimeraTrackData::RealTracks() { return Real(); }
const TArray<FChimeraTrack>& FChimeraTrackData::CustomTracks() { return Custom(); }

const FChimeraTrack* FChimeraTrackData::FindTrack(const FString& Name)
{
	for (const FChimeraTrack& T : Real()) if (T.Name == Name) return &T;
	for (const FChimeraTrack& T : Custom()) if (T.Name == Name) return &T;
	return nullptr;
}
