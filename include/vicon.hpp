#ifndef VICON_H
#define VICON_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vrpn_Tracker.h>
#include <Eigen/Dense>

class vicon
{
public:
    vicon();
    ~vicon();

    static void callback(void* userdata, const vrpn_TRACKERCB tdata);

    bool on = false;

    void open();
    void open(std::string object);
    void close();
    std::pair<Eigen::Vector3d, Eigen::Matrix3d> loop();
    bool loadConfig(const std::string& configFile);

private:
    vrpn_Tracker_Remote *tracker;
    std::string object; // Store the object name loaded from config

    // Internal variables to store position and rotation
    Eigen::Vector3d x_v;
    Eigen::Matrix3d R_vm;
};

#endif