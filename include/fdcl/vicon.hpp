#ifndef FDCL_VICON_H
#define FDCL_VICON_H

#include <stdio.h>
#include <iostream>
#include <vrpn_Tracker.h>
#include <Eigen/Dense>
#include "fdcl/matrix_utils.hpp"

namespace fdcl
{
    
class vicon
{
public:
    vicon();
    ~vicon();

    std::string object = "racing@192.168.10.1";
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

}  // end of namespace fdcl

#endif
