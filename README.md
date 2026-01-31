# Codesys-Webots Bridge

![Logo](Media/Logo.png)

**One Command → Everything!**

## Quick Start

**First time setup** (run once):
```powershell
.\setup.ps1
```

**Generate files:**
```powershell
.\CodesysWebotsBridge.exe schemas\puma560.json
```

## Demos

### 1. Puma560 Control from CODESYS
Direct control of Puma560 joints from CODESYS.
<video src="Media/preview.mp4" controls width="100%"></video>

### 2. Puma Jogging with Robotics Library
Using CODESYS SoftMotion/Robotics library for jogging & HMI ( forward kinematics [x,y,z, roll, pitch, yaw]).
<video src="Media/preview2.mp4" controls width="100%"></video>

### 3. Dual UR10e Control
Controlling two UR10e robots simultaneously from a single CODESYS session.
<video src="Media/preview3.mp4" controls width="100%"></video>

## Output Structure

```
generated/          ← Output folder for generated code
schemas/            ← JSON schemas for robots
src/                ← Source code (.cpp)
include/            ← Header files (.hpp)
build/              ← Compilation artifacts
Makefile            ← Build script
CodesysWebotsBridge.exe ← Main executable
```

## Building

```powershell
# Option 1: Using Make (if installed)
make

# Option 2: Using build script (Windows Bundle)
.\build.bat

# Option 3: Manual Command
windres src/resource.rc -O coff -o build/resource.o
g++ -std=c++17 -Iinclude -o CodesysWebotsBridge.exe src/main.cpp build/resource.o
```
```

## Quick Start


## Complete Workflow

1. **Setup** (once): `.\setup.ps1`
2. **Write Your Robot JSON schema**: `schemas\YourRobot.json`
3. **Generate**: `.\CodesysWebotsBridge.exe schemas\YourRobot.json`
4. **CODESYS**: Import `generated/codesys/*.xml` & add ToWebots Library to your project 
5. **Webots**: Copy `generated/webots/*` to controller directory
6. **Docs**: Follow Auto Generated Documentation in `generated/` for your robot

## Files

- `src/` - Source code (`main.cpp`)
- `include/` - Header files (`UnifiedCodeGenerator.hpp`, etc.)
- `CodesysWebotsBridge.exe` - Main executable  
- `setup.ps1` - Create output directories
- `schemas/` - Example JSON schemas

## Benefits

✅ All files from one command  
✅ Organized output structure  
✅ C++ and Webots files together  
✅ Auto-generated documentation
✅ You can add your custom robot to Webots and drive it from codesys as well
---

**Troubleshooting:**
- If no files appear → Run `.\setup.ps1` first
- Need custom schema → See `schemas/puma560.json` for format
