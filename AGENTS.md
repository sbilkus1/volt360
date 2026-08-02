# AGENTS.md — Volt360

## Build
- `cmd /c build.bat` → `build\app.exe` (raylib GUI)
- `cmd /c build.bat test` → `build\test_import.exe` (test suite, PASS/failures=0)
- Toolchain: MSVC (VS18 Build Tools); `vcvarsall.bat x64` must run first
- COBJECTs exclude raylib internals and test mains when linking manually

## Important Conventions
- `M_PI` must be `#define`d before `<math.h>` (project uses C, not C++)      — see `slicer.c`, `gcode.c`, `co_design.c`
- `BUF_CSTR` strings must be `str_dup`'d before `buf_free` — the buffer owns the chars
- `project_find_cad` / `project_find_pcb` return from `p->cad_models` / `p->pcbs` dynamic arrays

## Known Quirks
- `mesh_mass_props` returns **NaN volume** on multi-body meshes (separate closed solids like enclosure shell + standoffs + lid). Use **bbox dimension check** instead of volume assertions for these shapes.
- `infill_segments` in `src\slicer\slicer.c` has three defensive bounds guards (volatile `y_iter`, `nx_guard`, `cap_n`). Do NOT remove or simplify them — they protect against an MSVC `/O2` optimizer sensitivity where auto-initialization inside a loop body may be hoisted/elided, causing unbounded inner loops. The guards make the loops **always bounded** regardless of optimizer decisions.
- Any code change (even a harmless store) inside `slice_mesh` or its immediate caller chain **changes the optimizer's register/stack layout** and can mask the hang (false negative). To confirm hang fixes reliably, use a separate **debugger-attacher process** (launch target with `DEBUG_PROCESS|DEBUG_ONLY_THIS_PROCESS`, pump WaitForDebugEvent, check exit after timeout). The debug heap makes the hang 100% deterministic.

## Testing Quirks
- Full test suite hangs (used to) non-deterministically at ~45% rate during enclosure slicing (`co_design_print_cad` → `infill_segments`). Fixed 2026-08-02.
- PowerShell pattern for hang detection in loops:
  ```powershell
  $p = Start-Process -FilePath ... -PassThru -RedirectStandardOutput ...
  if ($p.WaitForExit(15000)) { <check ExitCode> } else { $p.Kill() }
  ```
- `$p.ExitCode` from `Start-Process -PassThru` is unreliable with `-RedirectStandardOutput` — prefer checking stdout content.

## Architecture Notes
- Slicer: `slice_mesh` → `join_loops` → `poly_offset_impl` → `infill_segments` → `gcode_emit`
- Co-design: `co_design_print_cad` → `gcode_slice_and_emit` → (`slice_mesh` then `gcode_emit`)
- PCB enclosure (`pcb_enclosure_mesh`): single closed pocket shell (no overlapping volumes), built with `m_quad`/`m_box`/`m_cyl` helpers in `co_design.c`
- UI modes: SCH(1), PCB(2), 3D(3), FIT(4), DESIGN(5), ASSIST(6), PRINT(7) — mode enum in `ui.h:8`
- New modules: `calibration.c` (OrcaSlicer test generators), `moonraker.c` (Klipper HTTP), `cam.c` (2.5D CNC + PCB isolation), `spice.c` (DC solver)

## Pending Warnings
- `ollama.c:37,41` (unused params)
- `ui.c:59,309` (unused locals)
- `test_co.c:21,25` — removed harness; warnings from ad-hoc test binaries

## Pending Warnings
- `ollama.c:37,41` (unused params)
- `ui.c:59,309` (unused locals)
