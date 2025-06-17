#include "version.hpp"
#include "tasks.hpp"
#include "time.hpp"
#include "parsing.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

// input and daemon different like the UI just adds to a file which could be locked if open
// might have to fork the daemon process that just reads and writes to file but the CLI takes in 
// Might need some interprocess communicate (pipes)
int main(int argc, char** argv){
    std::cout << "Version number: " << Repool_VERSION_MAJOR << std::endl;
    std::string current_time = get_current_time();
    std::cout << current_time << std::endl;
    try{
        parse_command_line_arguments(argc, argv); 
    }
    catch(std::runtime_error error){
        std::cout << error.what() << std::endl;
        return 1;
    }
    return 0;
}