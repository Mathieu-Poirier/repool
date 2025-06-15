#pragma once
#include "tasks.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

// Should maybe add execDate
inline constexpr int MAX_ARGS = 5;

inline Task parse_command_line_arguments(int argc, char** argv){
    std::vector<std::string> arguments(argv, argv + argc);
    std::vector<std::string> keywords{"job", "exec", "reapeat", "excluded"};
    std::cout << "BEGIN: Parsing command line arguments" << std::endl;
    std::string path;
    std::string exec_string;
    
    Task prepped_task;
    TimeOrder time_order;
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
                std::cout << "job selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
                path = arguments.at(i).substr(equality_position + 1, std::string::npos);
            }
            // Dates should be like exec=[NoRepeat24][12:30AM,13:45][Jun-15-2025,Jun-16-2025,@Today]
            // This keyword sets the mode
            if(kwarg == keywords[1]){
                std::cout << "exec schedule selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
                exec_string = arguments.at(i).substr(equality_position + 1, std::string::npos);
            }
            // Mode actually changes how things are parsed
            // Mode and time? -> this should maybe be changed to repeat schedule instead of mode because then it'spresence encodes the mode
            // Should only work if repeat state enabled 
            if(kwarg == keywords[2]){
                std::cout << "repeat schedule selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;

            }
            if(kwarg == keywords[3]){
                std::cout << "excluded dates selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
            }
        }
    }
    char left_square_bracket = '[';
    char right_square_bracket = ']';
    
// 1st block
auto l1 = std::find(exec_string.begin(), exec_string.end(), left_square_bracket);
auto r1 = std::find(l1, exec_string.end(), right_square_bracket);
std::cout << exec_string.substr(std::distance(exec_string.begin(), l1) + 1,
                                std::distance(l1, r1) - 1) << std::endl;

// Ending bracket should be skipped to when searching, this code has been generated.
// 2nd block
auto l2 = std::find(r1, exec_string.end(), left_square_bracket);
auto r2 = std::find(l2, exec_string.end(), right_square_bracket);
std::cout << exec_string.substr(std::distance(exec_string.begin(), l2) + 1,
                                std::distance(l2, r2) - 1) << std::endl;

// 3rd block
auto l3 = std::find(r2, exec_string.end(), left_square_bracket);
auto r3 = std::find(l3, exec_string.end(), right_square_bracket);
std::cout << exec_string.substr(std::distance(exec_string.begin(), l3) + 1,
                                std::distance(l3, r3) - 1) << std::endl;


    prepped_task.path = path;

    // Need to make a task object from the info?
    return prepped_task;
}