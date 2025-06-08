#pragma once
#include <iostream>
#include <ctime>
#include <string>

std::string get_current_time(){
    time_t timestamp;
    time(&timestamp);
    const char* time_as_string = ctime(&timestamp);
    return time_as_string;
}