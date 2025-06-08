#include "version.hpp"
#include "tasks.hpp"
#include "time.hpp"

#include <iostream>

int main(){
    std::cout << "Version number: " << Repool_VERSION_MAJOR << std::endl;
    std::string current_time = get_current_time();
    std::cout << current_time << std::endl;
    return 0;
}