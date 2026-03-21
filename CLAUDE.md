# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

**Game (Visual Studio / MSBuild):**
```
"C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe" GameSolution.sln /p:Configuration=Debug /p:Platform=x64 /m
```

**Tests (CMake + GTest):**
```
cd GameTests
cmake -B build
cmake --build build
cd build && ctest --output-on-failure
```

Single test: run the `GameTests` executable with `--gtest_filter=TestSuiteName.TestName`

## Architecture

The game uses a **state machine** with a stack managed by the `GameManager` singleton ([GameProject/GameManager.h](GameProject/GameManager.h)).

**Core flow:**
- `main.cpp` → `Application` (game loop, ~60 FPS) → `GameManager` (state stack) → active `GameState`
- Empty state stack = application exit

**GameState interface** ([GameProject/GameState.h](GameProject/GameState.h)): `OnEnter`, `OnExit`, `OnResume`, `Update(deltaTime)`, `Render`

**States:**
- `MainMenuState` — console-based menu using `ConsoleUtils` (Windows console I/O)
- `PlayingState` — forest level with player movement, camera, grass tiles, animated trees

**Rendering:** Abstract `Renderer` interface implemented by `RaylibRenderer` (Raylib 5.0). Sprites are frame-based from sprite sheets. Raylib key codes: Escape=256, Up=265, Down=264, Left=263, Right=262.

**Key constants (PlayingState):**
- Window: 800×600, Map: 1600×1200
- Player: 64×64 frames, 6 frames/direction, speed 200 px/s
- Pine trees: 128×128 frames, 8 frames, 5 color variants, 30 instances per level
- Tile: 16×16 source → 32×32 rendered

**Singletons:** `GameManager`, `Logger` (file-based logging)

**Stubs:** `AudioManager` and `NetworkManager` exist but have minimal implementation.

**Dependencies:** Raylib managed via vcpkg (`vcpkg.json`); packages cached in `packages/`.

## Planned Game Design

Core loop (from README): Enter forest → Cut trees → Transport logs to sled → Day ends → Sleep → Repeat. Planned systems: stamina, inventory, equipment progression.
