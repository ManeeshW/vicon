#include "vicon.hpp"
#include <fstream>
#include <sstream>
#include <string>

vicon::vicon()
{
    on = true;
    // Load configuration from config.cfg
    if (!loadConfig("../config.cfg")) {
        std::cerr << "VICON: Failed to load config file, using default object" << std::endl;
        object = "Origins@192.168.10.1"; // Fallback default
    }
}

vicon::~vicon()
{
    if (on)
    {
        vicon::close();
    }
}

void vicon::close()
{
    delete tracker;
    on = false;

    std::cout << "VICON: object closed" << std::endl;
}

void vicon::open(std::string object)
{
    tracker = new vrpn_Tracker_Remote(object.c_str());
    tracker->register_change_handler(this, vicon::callback);

    std::cout << "VICON: object opened" << std::endl;
}

void vicon::open()
{
    open(object);
}

std::pair<Eigen::Vector3d, Eigen::Matrix3d> vicon::loop() {
    tracker->mainloop();
    return {x_v, R_vm};  // Return the position and rotation matrix
}

bool vicon::loadConfig(const std::string& configFile)
{
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "VICON: Could not open config file: " << configFile << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Remove leading/trailing whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Skip empty lines or lines that don't contain object definition
        if (line.empty() || line.find("object:") == std::string::npos) {
            continue;
        }

        // Parse the object line
        size_t pos = line.find("object:");
        if (pos != std::string::npos) {
            object = line.substr(pos + 7); // Extract after "object:"
            object.erase(0, object.find_first_not_of(" \t\"")); // Remove leading spaces/quotes
            object.erase(object.find_last_not_of(" \t\"") + 1); // Remove trailing spaces/quotes
            break;
        }
    }

    file.close();
    if (object.empty()) {
        std::cerr << "VICON: No valid object found in config file" << std::endl;
        return false;
    }

    std::cout << "VICON: Loaded object: " << object << std::endl;
    return true;
}

void vicon::callback(void* userdata, const vrpn_TRACKERCB tdata)
{
    vicon* self = static_cast<vicon*>(userdata);

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