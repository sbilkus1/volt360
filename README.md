# Volt360

A C11 desktop app for CAD/EDA co-design. Import datasheets, schematics, PCBs,
and mechanical CAD files into one unified project, then analyze fit and view the
result in 2D and 3D.

## Features

- **Importers** (extension dispatch, all mutate a single `Project`):
  - KiCad: `.kicad_sch`, `.kicad_pcb`, `.kicad_sym`, `.kicad_mod`
  - Eagle: `.sch`, `.brd`
  - LibrePCB: `.lpp` project and `.lp_sym` / `.lp_fp` / `.lp_cmp` / `.lp_dev` / `.lp_pkg` library files
  - Netlists: KiCad `.net`, SPICE `.cir` / `.asc` (as schematics)
  - Meshes: `.stl`, `.obj`, `.step` / `.stp`
  - Datasheets: `.md`, `.csv`, `.docx` (via miniz-internal ZIP + XML)
  - Archives: `.zip` (extracted to a temp dir, every supported file inside imported)
  - Directory trees imported recursively
- **Ollama integration** (optional): parse datasheet text into `Component`
  records (name, package, dimensions, specs, pins) and chat. Requires a local
  Ollama server on `http://127.0.0.1:11434`; the app tolerates its absence.
- **Fit analysis**: assembly box-vs-box collisions + part heights, PCB footprint
  placement vs board outline and footprint overlap.
- **Design workspace** (Fusion-style, with an AI slant):
  - Parametric feature timeline: box / cylinder / sphere primitives with live
    rebuild into CAD models.
  - Materials library (10 built-ins: Aluminum, Steel, Titanium, Copper, ABS,
    PLA, Nylon, Acrylic, FR4, Alumina) with live mass properties (volume, mass,
    centroid) per feature.
  - Generative design: SIMP topology optimization (grid, volume fraction, load
    position) solved in-app (sparse FEA + CG + optimality criteria) and meshed
    to voxel parts you can view in 3D and export.
  - STL (binary) and BOM export.
  - Assembly joints + motion: revolute/prismatic joints on placed parts with an
    animated 3D demo (`R` in 3D view).
- **AI assistant** (key `6`): natural-language to parametric part creation
  (offline keyword parser: "add a box 40x30x10", "cylinder radius 5 height 20",
  "sphere r 12") with an Ollama chat fallback when the server is present.
- **Co-design** (key `7`): generate a watertight 3D-printable enclosure for
  any loaded PCB with adjustable wall thickness, height, clearance, and standoffs.
  The enclosure is stored as a CAD model and linked to the PCB via an assembly.
- **Slicer** (key `7`): full 3D slicing engine (OrcaSlicer-inspired) with
  adjustable layer height, line width, perimeters, infill density/angle,
  temperatures, speeds, retraction, scarf joint, fuzzy skin, sandwich mode,
  and Z-adaptive infill. Emits Marlin/Klipper-compatible G-code with AMS/CFS
  multi-color tool changes and per-feature temperature/speed overrides.
- **Print farm** (backend, key `7`): multi-printer dashboard (64 printers),
  job queue (256 jobs) with priority-based smart scheduling, orders (16 orders),
  filament tracking (64 spools), AMS auto-mapping, anomaly detection heuristics,
  release-temp auto-eject, and JSON persistence.
- **Calibration suite** (key `7`): OrcaSlicer-style calibration test generators
  — temperature tower, flow rate calibration, pressure advance (linear advance)
  pattern, max volumetric flow test, and VFA resonance tower. Each generates
  a standalone ready-to-print G-code file.
- **Network printing** (key `7`): Klipper/Moonraker HTTP client for remote
  printer control — upload G-code, start/pause/resume/cancel prints, query status.
- **CAM** (key `7`): 2.5D CNC milling toolpath generator — facing, profiling,
  pocketing, drilling. PCB isolation routing and outline cutting with tabs
  from imported KiCad board data. Outputs standard RS-274 NGC G-code.
- **SPICE simulation** (key `1`): DC operating point solver using modified nodal
  analysis (MNA) with Gaussian elimination — solves resistor networks and
  voltage sources in imported schematics. Displays node voltages and branch currents.
- **3D tools**: click-to-select models (ray-triangle intersection), wireframe
  toggle (W), focus-on-selection (F), distance measurement (M), axis crosshair.
- **PCB editing**: drag-to-move footprints, rotate (R), delete (X) with visual
  highlighting.
- **UI** (raylib): sidebar project tree, schematic/PCB 2D canvases (pan/zoom),
  orbit 3D view of imported meshes, fit report panel, parametric design workspace,
  AI assistant, print center with enclosure generator + slicer controls,
  drag-and-drop folder import.
- **Persistence**: JSON project save/load with a small dependency-free parser/writer.

## Layout

```
src/
  core/      util (strings, buffers, arrays, file IO, vectors), json, xml
  model/     project.h/.c — unified data model + JSON serialization
  import/    importer dispatch, kicad, eagle, librepcb, netlist, mesh (stl/obj/step), zip, datasheet, sexpr
  analysis/  fit.h/.c — fit analysis, spice.h/.c — DC circuit solver
  design/    design (materials/primitives/mass props/STL/BOM), co_design (enclosure), gen_design (SIMP topopt), joints (assembly motion), assistant (offline NLP)
  slicer/    slicer (3D slicing engine), gcode (Marlin/Klipper emitter), farm (print farm), calibration (temp/flow/PA/VFA tests), moonraker (Klipper network), cam (2.5D CNC milling + PCB isolation)
  llm/       ollama.h/.c — WinHTTP client
  ui/        ui.h/.c — raylib application shell
  main.c
third_party/ raylib 5.5 (static), miniz (split build: miniz.c + tdef/tinfl/zip)
tests/       test_import.c — end-to-end harness
examples/    sample KiCad/Eagle/STL/OBJ/CSV/MD/netlist files
```

## Build (Windows, MSVC)

Requires Visual Studio Build Tools (x64) — `build.bat` locates vcvarsall at
`C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools`.

```bat
build.bat          rem app -> build\app.exe
build.bat test     rem tests -> build\test_import.exe
```

The build compiles every `src/**/*.c` except `main.c` and the test harness into
a core object set, adds the four miniz translation units, and links raylib plus
`winhttp.lib` (Ollama client).

## Run

```bat
build\app.exe [folder]   rem optional folder to import at startup
```

Controls: middle-drag to pan (2D) / orbit (3D), wheel to zoom. Keys:
`1` schematic, `2` PCB, `3` 3D, `4` fit report, `5` design workspace,
`6` AI assistant, `7` print center (enclosure + slicer + farm + CAM + calibration + network),
`F5` parse datasheets via Ollama, `R` (in 3D) build the joint
animation demo. Drag and drop a folder onto the window to import it.

`SMOKE=1` runs a headless mode that imports the given folder and steps through
every view for a few frames (used by CI).

## Test

`build.bat test` runs `tests/test_import.c`: imports `examples/`, checks every
format, round-trips a ZIP and a DOCX in memory, exercises the design module
(materials, primitives, mass properties, generative topopt, assembly joints) and
STL export, and saves/reloads the project JSON. Expect
`RESULT: PASS (failures=0)`.
