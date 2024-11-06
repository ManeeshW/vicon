#include <pthread.h>
#include <iostream>
#include <vector> 
#include <time.h>

#include "fdcl/common_types.hpp"
#include "fdcl/matrix_utils.hpp"
#include "fdcl/vicon.hpp"

#define NUM_THREADS 4
pthread_mutex_t obs_mutex;

fdcl::vicon VICON;

bool SYS_ON = true;


int main(int argc, char **argv)
{
    std::cout << "Starting program .." << std::endl;

    VICON.open();
    while (SYS_ON)
    {
        VICON.loop();
    }
    VICON.close();

    std::cout << "Program closed!" << std::endl;

    return 0;
}
