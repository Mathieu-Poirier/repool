#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "../headers/parsing.hpp"
#include "../headers/task.hpp"
#include "../headers/time.hpp"
#include "../headers/time_order.hpp"

enum Months {
  OFFSET,
  Jan,
  Feb,
  Mar,
  Apr,
  May,
  Jun,
  Jul,
  Aug,
  Sep,
  Oct,
  Nov,
  Dec,
};

bool validate_string_date(std::string date) {

  if (date.size() < 10 || date[3] != '-' || date[6] != '-') {
    throw std::runtime_error("Invalid date format: " + date +
                             ". Should be something like [Jun-12-2025]");
  }

  std::string month = date.substr(0, 3);
  std::string day = date.substr(4, 2);
  std::string year = date.substr(7, std::string::npos);

  unsigned int year_number = std::stoi(year);
  unsigned int day_number = std::stoi(day);

  unsigned int month_number = 0;

  if (month == "Jan") {
    month_number = Months::Jan;
  } else if (month == "Feb") {
    month_number = Months::Feb;
  } else if (month == "Mar") {
    month_number = Months::Mar;
  } else if (month == "Apr") {
    month_number = Months::Apr;
  } else if (month == "May") {
    month_number = Months::May;
  } else if (month == "Jun") {
    month_number = Months::Jun;
  } else if (month == "Jul") {
    month_number = Months::Jul;
  } else if (month == "Aug") {
    month_number = Months::Aug;
  } else if (month == "Sep") {
    month_number = Months::Sep;
  } else if (month == "Oct") {
    month_number = Months::Oct;
  } else if (month == "Nov") {
    month_number = Months::Nov;
  } else if (month == "Dec") {
    month_number = Months::Dec;
  } else {
    throw std::runtime_error("Month format ill-formed: " + month +
                             ". Options are: Jan, Feb, Mar, Apr, May, Jun, "
                             "Jul, Aug, Sep, Oct, Nov, Dec");
  }

  bool valid = is_valid_date(year_number, month_number, day_number);

  return valid;
}

std::vector<std::string> extract_bracket_blocks(const std::string &input) {
  std::vector<std::string> blocks;
  size_t pos = 0;

  while (pos < input.size()) {
    auto l = input.find('[', pos);
    if (l == std::string::npos)
      break;

    auto r = input.find(']', l);
    if (r == std::string::npos)
      throw std::runtime_error("Unmatched '[' at position " +
                               std::to_string(l));
    
    std::string block = input.substr(l + 1, r - l - 1);
        
    if (block.find('[') != std::string::npos || block.find(']') != std::string::npos)
      throw std::runtime_error("Nested brackets are not allowed in block: " + block);

    blocks.push_back(block);
    pos = r + 1;
  }

  if (blocks.size() < 2 || blocks.size() > 3)
    throw std::runtime_error("Expected [Mode][Times] or [Mode][Times][Dates]");

  return blocks;
}

std::vector<std::string> split_string(const std::string &input,
                                      char delimiter) {
  std::vector<std::string> tokens;
  std::string current;
  for (char c : input) {
    if (c == delimiter) {
      if (!current.empty())
        tokens.push_back(current);
      current.clear();
    } else {
      current += c;
    }
  }
  if (!current.empty())
    tokens.push_back(current);
  return tokens;
}

std::vector<std::string> parse_dates(const std::string &date_str,
                                     bool validate) {
  auto dates = split_string(date_str, ',');
  if (validate) {
    for (const std::string &d : dates) {
      if (!validate_string_date(d)) {
        throw std::runtime_error("Invalid date: " + d);
      }
    }
  }
  return dates;
}

// Should maybe add execDate
inline constexpr int MAX_ARGS = 5;
inline std::regex time12h_regex(R"(^([1-9]|1[0-2]):[0-5][0-9](AM|PM)$)");

// need to split this in the producer process and throw
// std::runtime_error(string literal)
Task parse_command_line_arguments(int argc, char **argv) {
  const std::vector<std::string> arguments(argv, argv + argc);
  const std::vector<std::string> keywords{"job", "exec", "repeat", "exclude"};
  // std::cout << "BEGIN: Parsing command line arguments" << std::endl;

  Task prepped_task;
  TimeOrder time_order;

  // std::cout << "Argument number passed: " << argc << " " << "Max arguments: "
  // << MAX_ARGS << std::endl;
  if (argc > MAX_ARGS) {
    throw std::runtime_error("EXCEPTION: Too many arguments passed to repool. "
                             "Likely fix the uneeded spaces in the arguments.");
  }
  if (argc == 1) { // Might replace later
    throw std::runtime_error(
        "EXCEPTION: Zero arguments passed therefore input is ill-formed.");
  }
  // How do we handle backslash
  // Might need to move state in here
  constexpr char equality_character = '=';
  std::string path("");
  std::string exec_string("");
  std::string repeat_string("");
  std::string exclude_string("");

  for (int i = 1; i < argc and argc <= MAX_ARGS; i++) {
    // std::cout << i << std::endl;
    auto equality_token = std::find(arguments.at(i).begin(),
                                    arguments.at(i).end(), equality_character);
    if (equality_token != arguments.at(i).end()) {
      int equality_position =
          std::distance(arguments.at(i).begin(), equality_token);
      std::string kwarg = arguments.at(i).substr(0, equality_position);
      // Test if this works
      std::cout << "Keyword argument found: " << kwarg
                << std::endl; // This is the prefix to the keyword argument
      if (kwarg == keywords[0]) {
        std::cout << "job selected: "
                  << arguments.at(i).substr(equality_position + 1,
                                            std::string::npos)
                  << std::endl;
        path = arguments.at(i).substr(equality_position + 1, std::string::npos);
      }
      // Dates should be like
      // exec=[NoRepeat24][12:30AM,13:45][Jun-15-2025,Jun-16-2025,@Today] This
      // keyword sets the mode
      if (kwarg == keywords[1]) {
        std::cout << "exec schedule selected: "
                  << arguments.at(i).substr(equality_position + 1,
                                            std::string::npos)
                  << std::endl;
        exec_string =
            arguments.at(i).substr(equality_position + 1, std::string::npos);
      }
      // Mode actually changes how things are parsed
      // Mode and time? -> this should maybe be changed to repeat schedule
      // instead of mode because then it'spresence encodes the mode Should only
      // work if repeat state enabled
      if (kwarg == keywords[2]) {
        std::cout << "repeat schedule selected: "
                  << arguments.at(i).substr(equality_position + 1,
                                            std::string::npos)
                  << std::endl;
        repeat_string =
            arguments.at(i).substr(equality_position + 1, std::string::npos);
      }
      if (kwarg == keywords[3]) {
        std::cout << "excluded dates selected: "
                  << arguments.at(i).substr(equality_position + 1,
                                            std::string::npos)
                  << std::endl;
        exclude_string =
            arguments.at(i).substr(equality_position + 1, std::string::npos);
      }
    }
  }

  auto blocks = extract_bracket_blocks(exec_string);
  std::string mode_state_string = blocks[0];
  std::string time_string = blocks[1];
  std::string date_string = (blocks.size() == 3) ? blocks[2] : "";

  Modes mode_state;

  // TODO: Check for exclude string

  if (mode_state_string == "NoRepeat12") {
    mode_state = Modes::NoRepeat12;
    if (!repeat_string.empty()) {
      throw std::runtime_error("NoRepeat12 cannot use repeat= argument.");
    }
    if (date_string.empty()) {
      throw std::runtime_error("NoRepeat12 requires exact dates.");
    }
  } else if (mode_state_string == "NoRepeat24") {
    mode_state = Modes::NoRepeat24;
    if (!repeat_string.empty()) {
      throw std::runtime_error("NoRepeat24 cannot use repeat= argument.");
    }
    if (date_string.empty()) {
      throw std::runtime_error("NoRepeat24 requires exact dates.");
    }
  } else if (mode_state_string == "Repeat12") {
    mode_state = Modes::Repeat12;
    if (!date_string.empty()) {
      throw std::runtime_error("Repeat12 mode does not accept a date block.");
    }
  } else if (mode_state_string == "Repeat24") {
    mode_state = Modes::Repeat24;
    if (!date_string.empty()) {
      throw std::runtime_error("Repeat24 mode does not accept a date block.");
    }
  } else {
    throw std::runtime_error("Invalid repeat mode: " + mode_state_string);
  }

  time_order.repeat_mode = mode_state;

  if (mode_state == Modes::NoRepeat12 || mode_state == Modes::NoRepeat24) {
    time_order.times = split_string(time_string, ',');
    time_order.dates = parse_dates(date_string, true);

    if (mode_state == Modes::NoRepeat12) {
      for (const std::string &time : time_order.times) {
        if (!std::regex_match(time, time12h_regex)) {
          throw std::runtime_error("Invalid 12h time format: " + time);
        }
      }
    }
  } else if (mode_state == Modes::Repeat12 || mode_state == Modes::Repeat24) {
    time_order.times = split_string(time_string, ',');
    time_order.dates =
        parse_dates(date_string, false); // Should be empty or ignored
    time_order.repeat_schedule = repeat_string;

    if (mode_state == Modes::Repeat12) {
      for (const std::string &time : time_order.times) {
        if (!std::regex_match(time, time12h_regex)) {
          throw std::runtime_error("Invalid 12h time format: " + time);
        }
      }
    }

    if (!exclude_string.empty()) {
      auto l0 = std::find(exclude_string.begin(), exclude_string.end(), '[');
      auto r0 = std::find(l0, exclude_string.end(), ']');
      if (l0 != exclude_string.end() && r0 != exclude_string.end() && r0 > l0) {
        exclude_string =
            exclude_string.substr(std::distance(exclude_string.begin(), l0) + 1,
                                  std::distance(l0, r0) - 1);
      }
      time_order.excluded_dates = split_string(exclude_string, ',');
    }
  }

  // Add date and times into vectors of the object
  // if it's in 24h mode it shoudln't accept AM and PM specifiers

  // std::cout << time_order.times.size() << std::endl;
  for (std::string time : time_order.times) {
    std::cout << "Time parsed: " << time << std::endl;
  }

  for (std::string date : time_order.dates) {
    std::cout << "Date parsed: " << date << std::endl;
  }

  for (std::string edate : time_order.excluded_dates) {
    std::cout << "Excluded date parsed: " << edate << std::endl;
  }

  prepped_task.path = path;
  prepped_task.schedule_ordered = time_order;

  // Need to make a task object from the info?
  return prepped_task;
}