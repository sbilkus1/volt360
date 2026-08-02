# CHIMERA (UE5 prototype)

"Every game you've ever wanted to play. In one game. At the same time."

A from-scratch Unreal Engine 5 C++ prototype that implements the full CHIMERA
GDD (`../game.md`) as a *playable vertical slice* plus the complete system
architecture, organized section-by-section against the design doc.

## What's playable now

- **Character creation** (GDD 5.1): pick a Life Path (Cop, Doctor, Dragon Rider,
  Superhero, Racer, Builder, Netrunner...), distribute 7 attributes, get your
  start-of-game perk.
- **The Nexus hub + all 9 rings** (GDD 1, 4): procedurally built world. Walk
  through ring gates to teleport between Aether City, Neo-Kingdom, Mercy Heights,
  Mid-Wilshire, Alagaesia, Minecraft Frontier, Grand Coliseum, Night City, and
  back to the Nexus. Dragon-flight fast travel is unlocked via the egg.
- **Skills** (GDD 5.2): XP gain as you do things, per-skill levels, perk points,
  Legendary reset.
- **Currencies + BAWSAQ** (GDD 9): Credits/Emeralds/Crowns/Eddies, and a live
  stock market that reacts to your actions (crime waves, dragon attacks, race
  results).
- **Quest system** (GDD 12): a scripted tutorial arc + procedural radiant quests,
  mission ratings, cross-ring objectives.
- **Minigames** (GDD 13): Breach Protocol (hacking grid), Surgery (precision
  QTE), Lockpicking (timing bar), Interrogation (evidence-drop dialogue), and an
  F1 hot-lap time trial.
- **NPC brain** (GDD 14.19): needs, personality (7 axes), memory, schedules,
  and a Nemesis system (GDD 14.1) that evolves NPCs you wronged.
- **World simulation** (GDD 14.4/14.5/14.7): faction territory drift, disasters,
  elections, management sims (hospital / F1 / police), all ticking in the
  background.

## Controls

- Mouse: look. `WASD`: move. `Space`: jump. `Shift`: sprint. `C`: crouch (stealth).
- `E`: interact (nearest interactable). `F`: melee attack. `R`: ranged attack.
- `M`: map / ring list. `TAB`: BAWSAQ ticker. `P`: pause menu.
- Minigames: follow on-screen prompts (`WASD/arrows`, `Space`, `E/Enter`, `Esc` to cancel).

## Requirements

- Unreal Engine **5.3, 5.4 or 5.5** (free from the Epic Games Launcher). Visual
  Studio 2022 with "Game development with C++" workload.

## Build (first time)

1. Install UE 5.3+ and note its install path (e.g. `C:\Program Files\Epic Games\UE_5.4`).
2. Right-click `CHIMERA.uproject` -> **Generate Visual Studio project files**.
   (If the launcher asks for an engine version match, either set
   `EngineAssociation` in the `.uproject` to your installed version, or install
   the version named there.)
3. Open the generated `CHIMERA.sln` in VS, select **Development Editor / Win64**,
   and build. First build compiles the engine modules, so give it time.
4. Hit **F5 / Play**. The game starts on the Character Creation screen, then
   drops you into the Nexus.

Headless sanity check (no editor needed once built):
`CHIMERA.exe -game` launches straight into gameplay.

## The gameplay loop (GDD 3)

- **Seconds** - move, interact, attack, minigames.
- **Minutes** - a radiant callout / surgery / lap / heist step.
- **Hours** - quest chains and career milestones (skills + rep).
- **Sessions** - BAWSAQ, faction borders, elections, nemesis evolutions persist.
- **Lifetime** - Legacy System: retire your character, a heir inherits 20% of
  skills + world state (see `UChimeraSessionSubsystem::RetireCharacter`).

## Architecture (GDD section -> code)

| GDD section | Code |
|---|---|
| 1 World & rings | `World/ChimeraWorldBuilders.h` - `AChimeraWorld`, `FRingDef`, `ARingTeleporter` |
| 2 Raytracing | `Config/DefaultEngine.ini` `[SystemSettings]` |
| 3 Loops | `Core/ChimeraSessionSubsystem` (hours/sessions/lifetime) |
| 4 Districts | `Core/ChimeraTypes.h` ring defs; ring zones in world builder |
| 5 Progression | `Character/ChimeraCharacter` (attributes/skills/rep) + creation flow |
| 6 Mechanics | combat/stealth/medical/police/crafting/dragon/building hooks in `ChimeraCharacter` |
| 7 Multiplayer | co-op hooks in session subsystem (prototype = solo) |
| 8 Narrative | `Core/ChimeraQuests.h` Act-1 quest definitions |
| 9 Economy | currencies + `UBAWSAQSubsystem` + insurance/real-estate stubs |
| 10 Tech | `CHIMERA.Build.cs` module set |
| 11 Coherence | cross-ring event hooks (BAWSAQ, disasters, wanted) |
| 12 Missions | `UQuestSystem` + `GenerateRadiantQuest` + mission rating |
| 13 Minigames | `Minigames/ChimeraMinigames.h` |
| 14 Extra | nemesis, factions, disasters, politics, NPC brain, base building |

This is the scaffolding for the real thing: every system exists as a compiling,
tickable, data-backed C++ class that can be grown toward the full GDD. It is not
the $2.2B version. Yet.
