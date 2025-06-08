#pragma once
#include "tasks.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

// Should maybe add execDate
inline constexpr int MAX_ARGS = 5;

Task parse_command_line_arguments(int argc, char** argv){
    std::vector<std::string> arguments(argv, argv + argc);
    std::vector<std::string> keywords{"job", "execTime", "mode", "excludedDates"};
    std::cout << "BEGIN: Parsing command line arguments" << std::endl;
    
    Task prepped_task;
    std::string too_many_args_exception = "EXCEPTION: Too many arguments passed to repool. Likely fix the uneeded spaces in the arguments.";
    std::string zero_args_exception = "EXCEPTION: Zero arguments passed therefore input is ill-formed.";
    
    std::cout << "Argument number passed: " << argc << " " << "Max arguments: " << MAX_ARGS << std::endl;
    if(argc > MAX_ARGS){
        throw too_many_args_exception;
    }
    if(argc == 1){ // Might replace later
        throw zero_args_exception;
    }
    // How do we handle backslash
    for (int i = 1; i < argc and argc <= MAX_ARGS; i++){
        char equality_character = '=';
        // std::cout << i << std::endl;
        auto equality_token = std::find(arguments.at(i).begin(), arguments.at(i).end(), equality_character);
        if(equality_token != arguments.at(i).end()){
            int equality_position = std::distance(arguments.at(i).begin(), equality_token);
            std::string kwarg = arguments.at(i).substr(0, equality_position);
            // Test if this works
            std::cout << "Keyword argument found: " << kwarg << std::endl; // This is the prefix to the keyword argument
            if(kwarg == keywords[0]){
                std::cout << "Job selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
            }
            if(kwarg == keywords[1]){
                std::cout << "execTime selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
            }
            if(kwarg == keywords[2]){
                std::cout << "mode selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
            }
            if(kwarg == keywords[3]){
                std::cout << "excludedDates selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
            }
        }
    }
    return prepped_task;
}