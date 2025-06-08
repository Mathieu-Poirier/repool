#include "version.hpp"
#include "tasks.hpp"
#include "time.hpp"
#include "parsing.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv){
    std::cout << "Version number: " << Repool_VERSION_MAJOR << std::endl;
    std::string current_time = get_current_time();
    std::cout << current_time << std::endl;
    try{
        parse_command_line_arguments(argc, argv); 
    }
    catch(std::string error){
        std::cout << error << std::endl;
        return 1;
    }
    return 0;
}