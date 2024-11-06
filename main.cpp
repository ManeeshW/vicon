//#include <pthread.h>
#include <iostream>
#include <vector> 
#include <time.h>

#include "include/fdcl/common_types.hpp"
#include "include/fdcl/matrix_utils.hpp"
#include "include/fdcl/vicon.hpp"

// #define NUM_THREADS 4
// pthread_mutex_t obs_mutex;

fdcl::vicon VICON;

bool SYS_ON = true;


int main(int argc, char **argv)
{
    std::cout << "Starting program .." << std::endl;

    VICON.open();
    while (SYS_ON)
        {
            // Fetch the position and rotation data from the VICON system
            auto [x_v, R_vm] = VICON.loop();  // Using structured bindings to unpack the result
            
            // Print the results
            std::cout << "x_v: " << x_v.transpose() << std::endl;  // Print position vector
            std::cout << "R_vm:\n" << R_vm << std::endl;  // Print rotation matrix
        }
    VICON.close();

    std::cout << "Program closed!" << std::endl;

    return 0;
}
