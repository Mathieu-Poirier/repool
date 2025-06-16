#pragma once
#include "tasks.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

// Should maybe add execDate
inline constexpr int MAX_ARGS = 5;

// need to split this in the producer process and throw std::runtime_error(string literal)
inline Task parse_command_line_arguments(int argc, char** argv){
    const std::vector<std::string> arguments(argv, argv + argc);
    const std::vector<std::string> keywords{"job", "exec", "repeat", "exclude"};
    std::cout << "BEGIN: Parsing command line arguments" << std::endl;
    
    std::string path;
    std::string exec_string;
    std::string repeat_string;
    std::string exclude_string;
    
    Task prepped_task;
    TimeOrder time_order;
    const std::string too_many_args_exception = "EXCEPTION: Too many arguments passed to repool. Likely fix the uneeded spaces in the arguments.";
    const std::string zero_args_exception = "EXCEPTION: Zero arguments passed therefore input is ill-formed.";
    const std::string ill_formed_mode_exception = "EXCEPTION: Repeat mode option and 12h or 24h mode option is ill-formed.";
    
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
                repeat_string = arguments.at(i).substr(equality_position + 1, std::string::npos);
            }
            if(kwarg == keywords[3]){
                std::cout << "excluded dates selected: " << arguments.at(i).substr(equality_position + 1, std::string::npos) << std::endl;
                exclude_string = arguments.at(i).substr(equality_position + 1, std::string::npos);
            }
        }
    }
    // Maybe you cannot pass specific dates with a repeat mode

    char left_square_bracket = '[';
    char right_square_bracket = ']';

    // Exec time blocks
    // 1st block
    auto l1 = std::find(exec_string.begin(), exec_string.end(), left_square_bracket);
    auto r1 = std::find(l1, exec_string.end(), right_square_bracket);
    std::string mode_state_string = exec_string.substr(std::distance(exec_string.begin(), l1) + 1,
                                    std::distance(l1, r1) - 1);

    // Ending bracket should be skipped to when searching, this code has been generated.
    // 2nd block
    auto l2 = std::find(r1, exec_string.end(), left_square_bracket);
    auto r2 = std::find(l2, exec_string.end(), right_square_bracket);
    std::string time_string = exec_string.substr(std::distance(exec_string.begin(), l2) + 1,
                                    std::distance(l2, r2) - 1);

    // 3rd block
    auto l3 = std::find(r2, exec_string.end(), left_square_bracket);
    auto r3 = std::find(l3, exec_string.end(), right_square_bracket);
    std::string date_string = exec_string.substr(std::distance(exec_string.begin(), l3) + 1,
                                    std::distance(l3, r3) - 1);

    Modes mode_state;

    if(mode_state_string == "NoRepeat12"){
        mode_state = Modes::NoRepeat12;
    }
    else if(mode_state_string == "NoRepeat24") {
        mode_state = Modes::NoRepeat24;
    }
    else if(mode_state_string == "Repeat12") {
        mode_state = Modes::Repeat12;
    }
    else if(mode_state_string == "Repeat24") {
        mode_state = Modes::Repeat24;
    }
    else{
        throw ill_formed_mode_exception;
    }
    time_order.repeat_mode = mode_state;
    if(mode_state == Modes::Repeat12 or mode_state == Modes::Repeat24){
       time_order.repeat_schedule = repeat_string;
       // Re create the excluded dates in a vector
    }

    // Add date and times into vectors of the object
    // if it's in 24h mode it shoudln't accept AM and PM specifiers

    std::string single_time_container;
    std::string::iterator it = time_string.begin();

    // parsing time list but while(true) is bad practice because of bounds
    // TODO: make a general split function to clean up code
    while (it != time_string.end()) {
        if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" << single_time_container << "\"\n";
            time_order.times.push_back(single_time_container);
            single_time_container.clear();
        } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            single_time_container += *it;
        }
        ++it;
    }

    // After loop: push remaining part if any
    if (!single_time_container.empty()) {
        // std::cout << "End of string reached. Final push: \"" << single_time_container << "\"\n";
        time_order.times.push_back(single_time_container);
    }

    it = date_string.begin();
    std::string single_date_container;

    while (it != date_string.end()) {
        if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" << single_time_container << "\"\n";
            time_order.dates.push_back(single_date_container);
            single_date_container.clear();
        } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            single_date_container += *it;
        }
        ++it;
    }

    // After loop: push remaining part if any
    if (!single_date_container.empty()) {
        // std::cout << "End of string reached. Final push: \"" << single_time_container << "\"\n";
        time_order.dates.push_back(single_date_container);
    }

    // std::cout << time_order.times.size() << std::endl;
    for(std::string time : time_order.times){
        std::cout << "Time parsed: " << time << std::endl;
    }

    for(std::string date : time_order.dates){
        std::cout << "Date parsed: " << date << std::endl;
    }

    prepped_task.path = path;
    prepped_task.schedule_ordered = time_order;


    // Need to make a task object from the info?
    return prepped_task;
}