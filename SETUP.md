# Setup

This course targets **macOS, Linux, and Windows**. You need three things:

1. A **C++ compiler** that supports C++17 (or newer).
2. **CMake** ≥ 3.16 (used from stage 05 onward).
3. `clang-format` and `clang-tidy` (introduced in stage 05; optional before then).

Everything else — **SFML**, **Catch2**, **Dear ImGui** — is downloaded and built automatically
by CMake (`FetchContent`) the first time you build a stage that needs it. You do **not** install
those with your system package manager. (The first such build is slower because the library
compiles from source; it's cached in the build directory afterward.)

> **Note on this repo's origin:** it was scaffolded and verified on macOS. The Linux and Windows
> instructions below are written from each toolchain's documented requirements but were **not
> executed on those platforms here** — if you hit a snag on Linux/Windows, it's most likely a
> missing system package listed below.

---

## macOS

```bash
# 1. Compiler: Apple Clang, via the Xcode Command Line Tools (includes lldb)
xcode-select --install        # skip if `clang++ --version` already works

# 2. CMake (and the style tools)
brew install cmake
brew install llvm             # provides clang-format & clang-tidy (or: brew install clang-format)
```

- Debugger: `lldb` ships with the Command Line Tools.
- SFML needs no extra system packages on macOS.

---

## Linux

Package names below are for Debian/Ubuntu (`apt`). For Fedora use `dnf` and the `-devel`
equivalents; for Arch use `pacman`.

```bash
# 1. Compiler + build tools + CMake
sudo apt update
sudo apt install build-essential cmake gdb

# 2. Style tools
sudo apt install clang-format clang-tidy

# 3. SFML's native build dependencies (needed even though SFML is built from source,
#    because these are OS system libraries CMake cannot fetch)
sudo apt install \
  libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
  libudev-dev libgl1-mesa-dev libfreetype-dev \
  libflac-dev libogg-dev libvorbis-dev libopenal-dev
```

- Debugger: `gdb` (installed above). `lldb` also works if you prefer (`sudo apt install lldb`).
- The X11/OpenGL/FreeType/audio packages are only strictly required once you reach the SFML
  stages (14+), but installing them now avoids a confusing build failure later.

---

## Windows

Two supported options — pick one.

### Option A: Visual Studio 2022 (recommended)

1. Install **Visual Studio 2022** (Community edition is free).
2. In the installer, select the **"Desktop development with C++"** workload. This includes the
   MSVC compiler, a bundled **CMake**, `clang-format`, and the Visual Studio debugger.
3. Open the repo folder directly in Visual Studio (it understands CMake projects), **or** use the
   "Developer PowerShell for VS 2022" so `cmake` and `cl` are on your `PATH`.

### Option B: MSYS2 / MinGW-w64

```bash
# In an MSYS2 UCRT64 shell:
pacman -S --needed \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-clang-tools-extra \
  mingw-w64-ucrt-x86_64-gdb
```

- Debugger: the Visual Studio debugger (Option A) or `gdb` (Option B).
- SFML needs no extra system packages on Windows (it's built from source via CMake, and Windows
  provides the needed graphics/audio libraries through the SDK).

---

## VS Code extensions (optional but recommended)

If you use Visual Studio Code, install these from the Extensions view (`Cmd/Ctrl+Shift+X`) or with
`code --install-extension <id>`:

| Extension | ID | Why |
|-----------|----|-----|
| **C/C++** | `ms-vscode.cpptools` | IntelliSense, code navigation, and debugging for C++. |
| **CMake Tools** | `ms-vscode.cmake-tools` | Configure / build / test / debug the CMake stages (05+) from the editor — pick a build type, hit build, launch the debugger. |
| **CodeLLDB** | `vadimcn.vscode-lldb` | Native `lldb` debugging on macOS/Linux (the debugger you meet in stage 05); nicer than the default on macOS. |
| **clangd** *(optional)* | `llvm-vs-code-extensions.vscode-clangd` | More accurate IntelliSense + inline `clang-tidy` warnings, driven by the `compile_commands.json` the build exports. **If you enable it, disable the C/C++ extension's IntelliSense** (`"C_Cpp.intelliSenseEngine": "disabled"`) so the two don't fight. |

Shortcut: the **C/C++ Extension Pack** (`ms-vscode.cpptools-extension-pack`) bundles C/C++ + CMake
Tools in one install.

Notes:
- The root `CMakeLists.txt` sets `CMAKE_EXPORT_COMPILE_COMMANDS`, so a `compile_commands.json` is
  produced when you configure — this is what powers accurate IntelliSense (especially with clangd).
- Formatting/linting: the C/C++ extension formats with `clang-format` out of the box (it picks up
  the repo's `.clang-format`); clangd applies the repo's `.clang-tidy`. Enable
  `"editor.formatOnSave": true` to auto-format on save (the C++ `black`-on-save equivalent).
- On **Windows**, if you'd rather stay in full Visual Studio 2022, you don't need any of these —
  VS already provides IntelliSense, the CMake integration, and the debugger.

This repo ships a `.vscode/extensions.json` listing these as **workspace recommendations** — when
you open the folder, VS Code offers to install them in one click. Personal editor settings
(`.vscode/settings.json`, etc.) are intentionally left uncommitted.

---

## Building a stage

**Stages 00–04** are plain single-/multi-file programs you compile directly (each README shows
the exact `clang++`/`g++` command). From **stage 05 onward** you use CMake. The workflow is
identical on every OS:

```bash
# from a stage folder, e.g. stages/05-cmake-and-debugging-basics
cmake -S . -B build            # configure (first run may download dependencies)
cmake --build build            # compile
ctest --test-dir build         # run tests (stages that have them, 13+)
```

The compiled program lands under `build/`. Each stage README names the exact binary to run.
(Unit tests arrive at stage 13; `ctest` is meaningful from there on.)

### Debug vs Release

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug     # symbols for lldb/gdb, assertions on
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release   # optimized
```

(On Visual Studio's multi-config generator, pass `--config Debug` to `cmake --build` instead.)

If a build fails, re-read the relevant stage README and the "SFML native dependencies" list
above — on Linux that's the usual culprit.
