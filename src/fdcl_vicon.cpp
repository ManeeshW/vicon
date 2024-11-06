#include "fdcl/vicon.hpp"

fdcl::vicon::vicon()
{
    on = true;
}


fdcl::vicon::~vicon()
{
    if (on)
    {
        fdcl::vicon::close();
    }
}


void fdcl::vicon::close()
{
    delete tracker;
    on = false;

    std::cout << "VICON: object closed" << std::endl;
}


void fdcl::vicon::open(std::string object)
{
    tracker = new vrpn_Tracker_Remote(object.c_str());
    tracker->register_change_handler(this, fdcl::vicon::callback);

    std::cout << "VICON: object opened" << std::endl;
}


void fdcl::vicon::open()
{
    open(object);
}


std::pair<Eigen::Vector3d, Eigen::Matrix3d> fdcl::vicon::loop() {
    tracker->mainloop();
    return {x_v, R_vm};  // Return the position and rotation matrix
}

void fdcl::vicon::callback(void* userdata, const vrpn_TRACKERCB tdata)
{
    fdcl::vicon* self = static_cast<fdcl::vicon*>(userdata);

    // Position
    self->x_v(0) = tdata.pos[0];
    self->x_v(1) = tdata.pos[1];
    self->x_v(2) = tdata.pos[2];

    // Rotation matrix calculation
    self->R_vm(0, 0) = 1 - 2 * (tdata.quat[1] * tdata.quat[1]) - 2 * (tdata.quat[2] * tdata.quat[2]);
    self->R_vm(0, 1) = 2 * tdata.quat[0] * tdata.quat[1] - 2 * tdata.quat[3] * tdata.quat[2];
    self->R_vm(0, 2) = 2 * tdata.quat[0] * tdata.quat[2] + 2 * tdata.quat[3] * tdata.quat[1];
    self->R_vm(1, 0) = 2 * tdata.quat[0] * tdata.quat[1] + 2 * tdata.quat[3] * tdata.quat[2];
    self->R_vm(1, 1) = 1 - 2 * (tdata.quat[0] * tdata.quat[0]) - 2 * (tdata.quat[2] * tdata.quat[2]);
    self->R_vm(1, 2) = 2 * tdata.quat[1] * tdata.quat[2] - 2 * tdata.quat[3] * tdata.quat[0];
    self->R_vm(2, 0) = 2 * tdata.quat[0] * tdata.quat[2] - 2 * tdata.quat[3] * tdata.quat[1];
    self->R_vm(2, 1) = 2 * tdata.quat[0] * tdata.quat[3] + 2 * tdata.quat[2] * tdata.quat[1];
    self->R_vm(2, 2) = 1 - 2 * (tdata.quat[0] * tdata.quat[0]) - 2 * (tdata.quat[1] * tdata.quat[1]);

    std::cout << "\nx_v: " << self->x_v.transpose() << std::endl;
    std::cout << "R_vm: " << self->R_vm << std::endl;
}
