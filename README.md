# Unified Robot Code Generator

**One Command → Everything!**

## Quick Start

**First time setup** (run once):
```powershell
.\setup.ps1
```

**Generate files:**
```powershell
.\RobotCodeGenerator.exe schemas\puma560.json
```

## Output Structure

```
generated/          ← Output folder for generated code
schemas/            ← JSON schemas for robots
src/                ← Source code (.cpp)
include/            ← Header files (.hpp)
build/              ← Compilation artifacts
Makefile            ← Build script
RobotCodeGenerator.exe ← Main executable
```

## Building

```powershell
make
# OR
g++ -std=c++17 -Iinclude -o RobotCodeGenerator.exe src/main.cpp
```

## Quick Start


## Complete Workflow

1. **Setup** (once): `.\setup.ps1`
2. **Generate**: `.\unified_codegen.exe schemas\puma560.json`
3. **CODESYS**: Import `generated/codesys/*.xml`
4. **Webots**: Copy `generated/webots/*` to controller directory

## Files

- `UnifiedCodeGenerator.hpp` - Generator implementation
- `unified_codegen.exe` - Main executable  
- `setup.ps1` - Create output directories
- `schemas/` - Example JSON schemas

## Benefits

✅ All files from one command  
✅ Organized output structure  
✅ C++ and Webots files together  
✅ Auto-generated documentation

---

**Troubleshooting:**
- If no files appear → Run `.\setup.ps1` first
- Need custom schema → See `schemas/puma560.json` for format
