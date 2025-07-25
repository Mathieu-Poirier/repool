#pragma once

#include <string>
#include <vector>
#include "../headers/task.hpp"
#include "../headers/week_days.hpp"

bool validate_string_date(std::string date);
std::vector<std::string> extract_bracket_blocks(const std::string &input);
std::vector<std::string> split_string(const std::string &input, char delimiter);
std::vector<std::string> parse_dates(const std::string &date_str, bool validate = false);
Task parse_command_line_arguments(int argc, char **argv);
Day_Options parse_repeat_string(const std::string &repeat_string);
// Declarations for parsing.cpp

