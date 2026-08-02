// GDD 8 / 1 — Epic Opening Narrative. The Convergence — how Nexopolis was born.
// Plays during character creation as a cinematic text crawl + voiceover cues.
// Every player sees this once when they create their first character.

#pragma once
#include "CoreMinimal.h"

namespace ChimeraStory
{
	// ======================================================================
	// THE CONVERGENCE — the epic origin
	// ======================================================================
	inline TArray<FString> GetOpeningNarrative()
	{
		return {
			TEXT(""),
			TEXT("THE CONVERGENCE"),
			TEXT(""),
			TEXT("In the year 2187, the Multiverse was dying."),
			TEXT(""),
			TEXT("Eleven realities — eleven entire universes — had been drifting toward one another"),
			TEXT("for millennia. Faster than light. Faster than thought. Faster than anyone"),
			TEXT("in any reality could measure. The great minds of every world sensed it."),
			TEXT(""),
			TEXT("The Keepers — an ancient order that existed between realities — tried to stop it."),
			TEXT("They built the Nexus Tower, a structure that existed simultaneously in all eleven"),
			TEXT("worlds, tethered by Reality Anchors forged from the matter of dying stars."),
			TEXT(""),
			TEXT("But the multiverse was too powerful. Too inevitable."),
			TEXT(""),
			TEXT("On the last day of the old worlds — a day that would later be called the Crash —"),
			TEXT("the eleven realities collapsed into one another. Mountains fell into oceans"),
			TEXT("that had never existed. Skyscrapers rose from medieval fields. Dragons"),
			TEXT("circled the spires of futuristic cities. And in the chaos, a new world was born."),
			TEXT(""),
			TEXT("Nexopolis."),
			TEXT(""),
			TEXT("A supercontinent spanning 2,400 square kilometres. A city-planet where"),
			TEXT("superheroes, surgeons, dragon riders, police officers, wizards, tributes,"),
			TEXT("racing champions, netrunners, and kings all woke up in the same reality."),
			TEXT(""),
			TEXT("No one knew why they were chosen. No one knew how they survived."),
			TEXT(""),
			TEXT("But one thing was clear: the Crash wasn't an ending."),
			TEXT(""),
			TEXT("It was a beginning."),
			TEXT(""),
			TEXT("And you are at the centre of it."),
			TEXT(""),
		};
	}

	// ======================================================================
	// Character Origin — why you're here
	// ======================================================================
	inline TArray<FString> GetPlayerOrigin(const FString& LifePath)
	{
		TArray<FString> Origin;

		if (LifePath == TEXT("Cop"))
		{
			Origin = {
				TEXT("You were on patrol in Mid-Wilshire when the sky split open."),
				TEXT("One moment, you were responding to a domestic disturbance call."),
				TEXT("The next, the world dissolved into light and sound."),
				TEXT("When you came to, you were kneeling on marble in the Nexus."),
				TEXT("Your badge was still on your chest. Your service weapon was still loaded."),
				TEXT("And the Keepers told you that your training in law and order"),
				TEXT("was exactly what this fractured new world needed."),
			};
		}
		else if (LifePath == TEXT("Doctor") || LifePath == TEXT("Surgeon"))
		{
			Origin = {
				TEXT("You were mid-surgery when the Crash hit."),
				TEXT("The patient on the table dissolved into quantum foam. The ceiling"),
				TEXT("of Grey-Sloan Memorial folded inward and became the roof of the Nexus."),
				TEXT("You were still holding the scalpel when the Keepers found you."),
				TEXT("They said your hands — steady in the face of the impossible —"),
				TEXT("were the hands that would save thousands in the days to come."),
				TEXT("You never found out what happened to the patient."),
			};
		}
		else if (LifePath == TEXT("Dragon Rider"))
		{
			Origin = {
				TEXT("You were flying patrol over the Beor Mountains when the sky shattered."),
				TEXT("Your dragon — screaming — folded its wings around you as the Crash"),
				TEXT("compressed eleven realities into one. You felt every death. Every birth."),
				TEXT("Every love story and every war. The Ancient Language screamed in your mind."),
				TEXT("When the light faded, Farthen Dûr stood beside a cyberpunk skyscraper."),
				TEXT("And the Keepers told you that the Riders were needed again —"),
				TEXT("not to protect one land, but to unite eleven."),
			};
		}
		else if (LifePath == TEXT("Netrunner"))
		{
			Origin = {
				TEXT("You were deep in the Net when the Blackwall didn't just crack — it vaporised."),
				TEXT("Every digital consciousness across 11 realities flooded through your neural link."),
				TEXT("For one eternal second, you saw everything. Every code. Every soul."),
				TEXT("When the ice cleared, you were inside the Nexus's quantum servers."),
				TEXT("The Keepers found you with data streaming from your fingertips."),
				TEXT("You were the only person who understood what had happened."),
				TEXT("You became their translator between worlds."),
			};
		}
		else if (LifePath == TEXT("Dauntless Initiate") || LifePath == TEXT("Tribute"))
		{
			Origin = {
				TEXT("You were fighting for your life when reality broke."),
				TEXT("The arena dissolved around you. The Capitol skyline folded into"),
				TEXT("the Dauntless compound. The choosing ceremony merged with the reaping."),
				TEXT("Tris and Katniss stood side by side, weapons raised at the same enemy."),
				TEXT("The Keepers told you that courage — the kind you showed in those"),
				TEXT("final moments of the old worlds — was the rarest resource of all."),
				TEXT("They asked you to fight again. Not for glory. For survival."),
			};
		}
		else
		{
			// Generic origin for other life paths
			Origin = {
				TEXT("You don't remember the Crash clearly. No one does."),
				TEXT("What you remember is the light. The sound of eleven realities"),
				TEXT("collapsing into a single point. And then the silence."),
				TEXT("You woke up in the Nexus, surrounded by strangers from worlds"),
				TEXT("you'd never imagined. A dragon rider offered you water."),
				TEXT("A surgeon checked your pulse. A police officer guarded the door."),
				TEXT("The Keepers said one word: 'Welcome.'"),
				TEXT("And your story in Nexopolis began."),
			};
		}
		return Origin;
	}
}

// ======================================================================
// Vehicle Camera Modes (separate from on-foot camera modes)
// ======================================================================
UENUM(BlueprintType)
enum class EVehicleCamera : uint8
{
	Cockpit,       // inside the vehicle — steering wheel, dash, mirrors
	Chase,         // behind the car, GTA-style (default)
	ChaseFar,      // pulled back chase cam
	Hood,          // bumper/hood view — low to the road
	Wheel,         // next to front wheel — drifting/action shots
	Cinematic,     // rotating orbit around vehicle
	CineAbove,     // helicopter-style top-down follow
	FreeLook,      // detached free camera — pan around the vehicle freely
	COUNT
};

namespace ChimeraVehicleCamera
{
	// Presets for the UE5 SpringArm/Camera on a vehicle
	struct FVehicleCamPreset
	{
		float SpringLength;
		FVector Offset;       // relative to vehicle
		float Pitch;          // degrees
		float FieldOfView;
		const TCHAR* Label;
	};

	inline FVehicleCamPreset GetPreset(EVehicleCamera Mode)
	{
		switch (Mode)
		{
		case EVehicleCamera::Cockpit:    return { 0.f,   FVector(0, -50, 40), 0.f,   90.f, TEXT("Cockpit — inside the car, full immersion") };
		case EVehicleCamera::Chase:      return { 600.f, FVector(0, 0, 120),  -8.f,  85.f, TEXT("Chase — standard behind-car view") };
		case EVehicleCamera::ChaseFar:   return { 1200.f, FVector(0, 0, 200), -5.f, 70.f, TEXT("Chase Far — wider behind-car") };
		case EVehicleCamera::Hood:        return { 0.f,   FVector(0, 200, 30), 5.f,  110.f, TEXT("Hood — low bumper view, sense of speed") };
		case EVehicleCamera::Wheel:       return { 0.f,   FVector(90, 40, 10), 0.f,  95.f, TEXT("Wheel — side view, drifting") };
		case EVehicleCamera::Cinematic:   return { 1500.f, FVector(0, 0, 300), -15.f, 60.f, TEXT("Cinematic — orbiting the vehicle") };
		case EVehicleCamera::CineAbove:   return { 0.f,   FVector(0, 0, 2000), -85.f, 70.f, TEXT("Helicopter — top-down chase") };
		case EVehicleCamera::FreeLook:    return { 2000.f, FVector(0, 0, 400), -20.f, 85.f, TEXT("Free Look — detached, orbit freely") };
		default: return { 600.f, FVector(0, 0, 120), -8.f, 85.f, TEXT("Default") };
		}
	}

	inline void ApplyToSpringArm(EVehicleCamera Mode, class USpringArmComponent* SpringArm, class UCameraComponent* Cam)
	{
		auto P = GetPreset(Mode);
		SpringArm->TargetArmLength = P.SpringLength;
		SpringArm->SetRelativeLocation(P.Offset);
		SpringArm->SetRelativeRotation(FRotator(P.Pitch, 0, 0));
		if (Cam) Cam->SetFieldOfView(P.FieldOfView);
	}
}
