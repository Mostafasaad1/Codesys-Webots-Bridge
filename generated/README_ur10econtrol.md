# UR10eControl - Generated Integration

Auto-generated from JSON schema by Program12 Unified Generator

## Generated Files

- **CODESYS**: `codesys/ur10econtrol_Types.xml`
- **C++ Types**: `cpp/ur10econtrol_types.hpp`
- **C++ Client**: `cpp/ur10econtrol_client.hpp`
- **Webots Controller**: `webots/ur10econtrol_controller.cpp`

## CODESYS Setup

1. **Import PLCopenXML:**
   - Right-click project tree → 'Import PLCopenXML...'
   - Select `codesys/ur10econtrol_Types.xml`
   - Structs appear in DUT (Data Unit Types)

2. **Use in Program:**
   ```structured-text
   VAR
       data : ST_UR10eControl;
       fbShm : FB_GenericSharedMem;
   END_VAR
   
   fbShm(
       pData := ADR(data),
       DataSize := SIZEOF(ST_UR10eControl),
       sMemoryName := 'GlobalLibMem',
       Enable := TRUE
   );
   ```

## C++ Client Setup

1. **Include files:**
   ```cpp
   #include "ur10econtrol_client.hpp"
   ```

2. **Use client:**
   ```cpp
   UR10eControlClient client;
   client.Connect(L"GlobalLibMem");
   
   auto* data = client.GetData();
   client.Sync();
   ```

## Webots Setup

1. **Create controller directory:**
   `<webots_project>/controllers/ur10econtrol/`

2. **Copy files:**
   - `webots/ur10econtrol_controller.cpp`
   - `cpp/ur10econtrol_types.hpp`
   - `cpp/ur10econtrol_client.hpp`
   - `GenericSharedMemory.hpp`
   - `CodesysTypes.hpp`

3. **Compile** and set as robot controller

## Schema Variables

| Name | Type | Direction | Purpose |
|------|------|-----------|----------|
| shoulder_pan_joint | LREAL | toCPP | PLC → C++ |
| shoulder_lift_joint | LREAL | toCPP | PLC → C++ |
| elbow_joint | LREAL | toCPP | PLC → C++ |
| wrist_1_joint | LREAL | toCPP | PLC → C++ |
| wrist_2_joint | LREAL | toCPP | PLC → C++ |
| wrist_3_joint | LREAL | toCPP | PLC → C++ |
| shoulder_pan_joint_sensor | LREAL | fromCPP | C++ → PLC |
| shoulder_lift_joint_sensor | LREAL | fromCPP | C++ → PLC |
| elbow_joint_sensor | LREAL | fromCPP | C++ → PLC |
| wrist_1_joint_sensor | LREAL | fromCPP | C++ → PLC |
| wrist_2_joint_sensor | LREAL | fromCPP | C++ → PLC |
| wrist_3_joint_sensor | LREAL | fromCPP | C++ → PLC |

## UUIDs (for reference)

- ST_UR10eControl: `faad68ca-f395-403a-9b97-c279fd2be4e1`
- ST_ToCPP: `d2ba4069-ddf7-4280-a7ed-643772f102ba`
- ST_FromCPP: `570bbf3b-0acd-4310-a3f8-e96c4370ae62`
