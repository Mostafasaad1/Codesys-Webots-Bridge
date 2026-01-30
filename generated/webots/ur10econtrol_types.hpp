#pragma once
#include "CodesysTypes.hpp"
#include <cstring>

// Auto-generated from schema: UR10eControl

#pragma pack(push, 1)

struct ST_ToCPP {
    LREAL shoulder_pan_joint;
    LREAL shoulder_lift_joint;
    LREAL elbow_joint;
    LREAL wrist_1_joint;
    LREAL wrist_2_joint;
    LREAL wrist_3_joint;
};

struct ST_FromCPP {
    LREAL shoulder_pan_joint_sensor;
    LREAL shoulder_lift_joint_sensor;
    LREAL elbow_joint_sensor;
    LREAL wrist_1_joint_sensor;
    LREAL wrist_2_joint_sensor;
    LREAL wrist_3_joint_sensor;
};

struct ST_UR10eControl {
    ST_ToCPP ToCPP;
    ST_FromCPP FromCPP;
    DINT LifePLC;
    DINT LifeCPP;
};

#pragma pack(pop)
