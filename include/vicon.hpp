#ifndef VICON_H
#define VICON_H

#include <stdio.h>
#include <iostream>
#include <vrpn_Tracker.h>
#include <Eigen/Dense>


class vicon
{
public:
    vicon();
    ~vicon();

    std::string object = "OriginsX@192.168.10.1";
    static void callback(void* userdata, const vrpn_TRACKERCB tdata);

    bool on = false;

    void open();
    void open(std::string object);
    void close();
    std::pair<Eigen::Vector3d, Eigen::Matrix3d> loop();

private:
    vrpn_Tracker_Remote *tracker;

    // Internal variables to store position and rotation
    Eigen::Vector3d x_v;
    Eigen::Matrix3d R_vm;
};


#endif