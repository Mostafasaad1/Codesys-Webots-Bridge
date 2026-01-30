# Unified Robot Code Generator

**One Command → Everything!**

## Quick Start

**First time setup** (run once):
```powershell
.\setup.ps1
```

**Generate files:**
```powershell
.\unified_codegen.exe schemas\puma560.json
```

## Output Structure

```
generated/
├── codesys/
│   └── puma560control_Types.xml    ← Import to CODESYS
├── webots/
│   ├── puma560control_types.hpp     ← C++ types
│   ├── puma560control_client.hpp    ← C++ client
│   └── puma560control_controller.cpp ← Webots controller
└── README_puma560control.md         ← Detailed usage guide
```

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
