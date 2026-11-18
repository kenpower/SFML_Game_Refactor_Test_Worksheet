# SFML Game Refactor & Test Worksheet

This is your starter code for the three-week project. It has five small classic games written in C++17 with [SFML 3](https://www.sfml-dev.org/): **Tetris, Minesweeper, Tron, Netwalk and Xonix**. You start them all from one text menu.

The games work, but most of them do everything in one long loop: they read the keyboard, apply the game's rules, use the clock and random numbers, and draw, all mixed together. Over the next three weeks you will pick **one** game and:

1. **read** it until you understand how it works,
2. **refactor** it in small, safe steps,
3. **split** its rules away from the graphics, so they can be tested,
4. **test** those rules with unit tests,
5. **add a new feature**, test first.

## Start here

1. Read the project brief, **[assignment.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/assignment.md)**, then **[week 1](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/assignment_week1.md)**.
2. Build the project and run the tests (see [Build and test](#build-and-test) below).
3. Fill in your notes as you go, in [NOTES.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/NOTES.md).

| Document | What it is |
|----------|------------|
| [assignment.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/assignment.md) | **The project brief**: what you'll learn, marking, rules, and the tags you push |
| [assignment_week1.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/assignment_week1.md) | Week 1: understand your game and refactor it towards a seam |
| [assignment_week2.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/assignment_week2.md) | Week 2 *(draft)*: split the game at the seam and test it |
| [assignment_week3.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/assignment_week3.md) | Week 3 *(draft)*: add a feature, test first |
| [NOTES.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/NOTES.md) | Your notes for the project. Every section you need is already there, with a prompt. |
| [GIT_CHEATSHEET.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/GIT_CHEATSHEET.md) | How to commit, push, tag and undo, in Visual Studio and on the command line |
| [BUILDING.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/BUILDING.md) | Full setup, build, run and test instructions, if the quick version below isn't enough |

## Lecture notes and the worked example

The lecture notes and a worked example are in a separate repository, **[split-at-the-seam](https://github.com/kenpower/split-at-the-seam)**:

- **[seams_lecture_notes.md](https://github.com/kenpower/split-at-the-seam/blob/main/seams_lecture_notes.md):** what a "seam" is, how to find one, and why `rand()`, the clock and the keyboard stay outside the code you test.
- **[howto_split.md](https://github.com/kenpower/split-at-the-seam/blob/main/howto_split.md):** a step-by-step split of **Catcher**, a small game (move a basket, catch falling fruit) made to show the method. Catcher has one complete program for each step, so you can compare what changed. Follow the same steps on your own game.

## Build and test

SFML 3 and GoogleTest are downloaded the first time you configure, if they aren't already installed. If anything here doesn't work, see [BUILDING.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/BUILDING.md), or ask in the lab.

### Visual Studio (Windows)

Use the IDE, not the command line. You need Visual Studio 2022 with the **Desktop development with C++** workload, which includes CMake and the GoogleTest test adapter.

1. **Open:** *File → Open → Folder…* and pick your repository's folder. Visual Studio reads `CMakePresets.json` and configures the project. Wait for "CMake generation finished" in the Output window.
2. **Choose a configuration:** pick **Debug** in the configuration drop-down on the toolbar.
3. **Build:** *Build → Build All*.
4. **Run or debug a game:** choose **sfml_refactor_lab.exe** in the *Select Startup Item* drop-down next to the green ▶ button, then press **F5**. If the game's images don't load, the debugger is running it from the wrong folder (see [BUILDING.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/BUILDING.md#visual-studio)).
5. **Test:** *Test → Test Explorer*, then **Run All Tests** (▶▶). Every test appears in a tree. Double-click a failing test to go to it. To debug a test, right-click it and choose **Debug**.

### Command line (Linux, macOS, or Windows without Visual Studio)

You need CMake 3.25+, Ninja, a C++17 compiler and Git (without Ninja, see [BUILDING.md](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet/blob/main/BUILDING.md#without-presets)).

```sh
cmake --workflow --preset debug      # configure, build and run the tests (Debug, in build-debug/)
cmake --workflow --preset release    # the same for Release, in build/
ctest --preset debug                 # run the tests again
```

In VS Code, use the CMake Tools extension: pick a preset, then use its Build, Debug and Test buttons in the status bar.

### Playing the games

**Run the launcher from its own folder** (`build/bin`, or `build-debug/bin` for Debug), because the games load their images with relative paths. Type the capital letter in a game's name (e.g. `T` for Tetris) to start it, and close its window to return to the menu.

## Where things are

| Path | What's there |
|------|--------------|
| `Games/` | One `.cpp` file per game, and the launcher (`main.cpp`) |
| `Games/TetrisBoard.h`, `MineField.h`, `TronArena.h`, `PipeGrid.h` | An empty class for each game. In week 2, your game's rules move into its class. |
| `Games/Geometry.h` | `geom::Point`, `geom::Vec2f`, `geom::Rect` and `geom::overlaps()`. Use these in your class instead of SFML's types. |
| `Games/Grid.h`, `Player.h` | Xonix's rules, already split from SFML. This is an example of what you're aiming for. |
| `Games/images/` | The games' images, copied next to the program when you build |
| `Tests/` | Unit tests. There is one file for each game you can choose (e.g. `tetris_test.cpp`): your tests go in that one. `test.cpp` tests Xonix's classes and `geometry_test.cpp` tests `Geometry.h`, so you can look at them for examples. |

### Reading the game code

The game files have comments to help you find your way around:

- `// ===== GUIDE` at the top: where to start reading, what state survives between frames, and the refactorings to look for
- `// ---- SETUP / INPUT / UPDATE / DRAW ----`: the big jobs in each game loop
- `// HINT:` things worth looking at closely, often with the debugger

### The ✔ or ✘ on GitHub

Every time you push, GitHub builds your code and runs your tests, using two checks: the unit tests on Linux (without SFML), and a full build of the games on Windows. You can see the result as a ✔ or ✘ next to each commit. A ✘ on the unit tests usually means your class has picked up SFML or doesn't compile.
