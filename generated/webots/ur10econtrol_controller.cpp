// Auto-generated Webots controller for: UR10eControl

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <iostream>
#include "ur10econtrol_client.hpp"

using namespace webots;
using namespace std;

#define TIME_STEP 64

int main(int argc, char **argv) {
    Robot *robot = new Robot();
    UR10eControlClient client;

    cout << "=== UR10eControl Webots Controller ===" << endl;

    // Get motors/sensors
    Motor *shoulder_pan_joint_motor = robot->getMotor("shoulder_pan_joint");
    Motor *shoulder_lift_joint_motor = robot->getMotor("shoulder_lift_joint");
    Motor *elbow_joint_motor = robot->getMotor("elbow_joint");
    Motor *wrist_1_joint_motor = robot->getMotor("wrist_1_joint");
    Motor *wrist_2_joint_motor = robot->getMotor("wrist_2_joint");
    Motor *wrist_3_joint_motor = robot->getMotor("wrist_3_joint");

    // Connect to CODESYS
    while (!client.Connect(L"GlobalLibMem")) {
        cout << "Waiting for CODESYS..." << endl;
        robot->step(TIME_STEP);
    }
    cout << ">> Connected!" << endl;

    // Main loop
    while (robot->step(TIME_STEP) != -1) {
        if (shoulder_pan_joint_motor) shoulder_pan_joint_motor->setPosition(client.GetShoulder_pan_joint());
        if (shoulder_lift_joint_motor) shoulder_lift_joint_motor->setPosition(client.GetShoulder_lift_joint());
        if (elbow_joint_motor) elbow_joint_motor->setPosition(client.GetElbow_joint());
        if (wrist_1_joint_motor) wrist_1_joint_motor->setPosition(client.GetWrist_1_joint());
        if (wrist_2_joint_motor) wrist_2_joint_motor->setPosition(client.GetWrist_2_joint());
        if (wrist_3_joint_motor) wrist_3_joint_motor->setPosition(client.GetWrist_3_joint());
        client.Sync();
    }

    delete robot;
    return 0;
}
