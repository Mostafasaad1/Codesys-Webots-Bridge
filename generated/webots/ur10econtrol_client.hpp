#pragma once
#include "GenericSharedMemory.hpp"
#include "ur10econtrol_types.hpp"

// Auto-generated typed client for: UR10eControl

class UR10eControlClient : public TypedSharedMemory<ST_UR10eControl> {
public:
    UR10eControlClient() : TypedSharedMemory<ST_UR10eControl>() {}

    LREAL GetShoulder_pan_joint() const {
        return IsConnected() ? GetData()->ToCPP.shoulder_pan_joint : 0;
    }

    LREAL GetShoulder_lift_joint() const {
        return IsConnected() ? GetData()->ToCPP.shoulder_lift_joint : 0;
    }

    LREAL GetElbow_joint() const {
        return IsConnected() ? GetData()->ToCPP.elbow_joint : 0;
    }

    LREAL GetWrist_1_joint() const {
        return IsConnected() ? GetData()->ToCPP.wrist_1_joint : 0;
    }

    LREAL GetWrist_2_joint() const {
        return IsConnected() ? GetData()->ToCPP.wrist_2_joint : 0;
    }

    LREAL GetWrist_3_joint() const {
        return IsConnected() ? GetData()->ToCPP.wrist_3_joint : 0;
    }

    void SetShoulder_pan_joint_sensor(LREAL value) {
        if (IsConnected()) GetData()->FromCPP.shoulder_pan_joint_sensor = value;
    }

    void SetShoulder_lift_joint_sensor(LREAL value) {
        if (IsConnected()) GetData()->FromCPP.shoulder_lift_joint_sensor = value;
    }

    void SetElbow_joint_sensor(LREAL value) {
        if (IsConnected()) GetData()->FromCPP.elbow_joint_sensor = value;
    }

    void SetWrist_1_joint_sensor(LREAL value) {
        if (IsConnected()) GetData()->FromCPP.wrist_1_joint_sensor = value;
    }

    void SetWrist_2_joint_sensor(LREAL value) {
        if (IsConnected()) GetData()->FromCPP.wrist_2_joint_sensor = value;
    }

    void SetWrist_3_joint_sensor(LREAL value) {
        if (IsConnected()) GetData()->FromCPP.wrist_3_joint_sensor = value;
    }

};
