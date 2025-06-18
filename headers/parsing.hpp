#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "tasks.hpp"

// Should maybe add execDate
inline constexpr int MAX_ARGS = 5;
inline std::regex time12h_regex(R"(^([1-9]|1[0-2]):[0-5][0-9](AM|PM)$)");

// need to split this in the producer process and throw
// std::runtime_error(string literal)
inline Task parse_command_line_arguments(int argc, char** argv) {
  const std::vector<std::string> arguments(argv, argv + argc);
  const std::vector<std::string> keywords{"job", "exec", "repeat", "exclude"};
  // std::cout << "BEGIN: Parsing command line arguments" << std::endl;

  Task prepped_task;
  TimeOrder time_order;

  // std::cout << "Argument number passed: " << argc << " " << "Max arguments: "
  // << MAX_ARGS << std::endl;
  if (argc > MAX_ARGS) {
    throw std::runtime_error(
        "EXCEPTION: Too many arguments passed to repool. "
        "Likely fix the uneeded spaces in the arguments.");
  }
  if (argc == 1) {  // Might replace later
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
                << std::endl;  // This is the prefix to the keyword argument
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

  // Maybe you cannot pass exact dates with a repeat mode

  constexpr char left_square_bracket = '[';
  constexpr char right_square_bracket = ']';

  // Exec time blocks
  // 1st block
  // Need to make checks to see if they aren't empty
  auto l1 =
      std::find(exec_string.begin(), exec_string.end(), left_square_bracket);
  auto r1 = std::find(l1, exec_string.end(), right_square_bracket);
  std::string mode_state_string = exec_string.substr(
      std::distance(exec_string.begin(), l1) + 1, std::distance(l1, r1) - 1);

  // Ending bracket should be skipped to when searching, this code has been
  // generated. 2nd block
  auto l2 = std::find(r1, exec_string.end(), left_square_bracket);
  auto r2 = std::find(l2, exec_string.end(), right_square_bracket);
  std::string time_string = exec_string.substr(
      std::distance(exec_string.begin(), l2) + 1, std::distance(l2, r2) - 1);

  // 3rd block
  auto l3 = std::find(r2, exec_string.end(), left_square_bracket);
  auto r3 = std::find(l3, exec_string.end(), right_square_bracket);
  std::string date_string = exec_string.substr(
      std::distance(exec_string.begin(), l3) + 1, std::distance(l3, r3) - 1);

  Modes mode_state;

  if (mode_state_string == "NoRepeat12") {
    mode_state = Modes::NoRepeat12;
    if (!repeat_string.empty()) {
      throw std::runtime_error(
          "NoRepeat12 mode enabled but repeat argument passed anyway. Do not "
          "set a repeat schedule if you don't intend to repeat");
    }
  } else if (mode_state_string == "NoRepeat24") {
    mode_state = Modes::NoRepeat24;
    if (!repeat_string.empty()) {
      throw std::runtime_error(
          "NoRepeat24 mode enabled but repeat argument passed anyway. Do not "
          "set a repeat schedule if you don't intend to repeat");
    }
  } else if (mode_state_string == "Repeat12") {
    mode_state = Modes::Repeat12;
  } else if (mode_state_string == "Repeat24") {
    mode_state = Modes::Repeat24;
  } else {
    throw std::runtime_error(
        "EXCEPTION: Repeat mode option is ill-formed: NoRepeat12, NoRepeat24, "
        "Repeat12, Repeat24 are possible options.");
  }

  time_order.repeat_mode = mode_state;

  if (mode_state == Modes::NoRepeat12) {
    // Parse AM PM
    std::string temp_time_container;
    std::string::iterator it = time_string.begin();

    // parsing time list but while(true) is bad practice because of bounds
    // TODO: make a general split function to clean up code
    while (it != time_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
        time_order.times.push_back(temp_time_container);
        temp_time_container.clear();
      } else {
        // std::cout << "Reading character: '" << *it << "'\n";
        temp_time_container += *it;
      }
      ++it;
    }

    // After loop: push remaining part if any
    if (!temp_time_container.empty()) {
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.times.push_back(temp_time_container);
    }

    it = date_string.begin();
    std::string single_date_container;

    while (it != date_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
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
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.dates.push_back(single_date_container);
    }

    // 12h parsing AI generated
    for (const std::string& time : time_order.times) {
      if (!std::regex_match(time, time12h_regex)) {
        throw std::runtime_error("Invalid 12h time format: " + time);
      }
    }
  } else if (mode_state == Modes::Repeat12) {
    std::string temp_time_container;
    std::string::iterator it = time_string.begin();

    // parsing time list but while(true) is bad practice because of bounds
    // TODO: make a general split function to clean up code
    while (it != time_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
        time_order.times.push_back(temp_time_container);
        temp_time_container.clear();
      } else {
        // std::cout << "Reading character: '" << *it << "'\n";
        temp_time_container += *it;
      }
      ++it;
    }

    // After loop: push remaining part if any
    if (!temp_time_container.empty()) {
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.times.push_back(temp_time_container);
    }

    it = date_string.begin();
    std::string single_date_container;

    while (it != date_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
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
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.dates.push_back(single_date_container);
    }
    time_order.repeat_schedule = repeat_string;

    std::string single_time_container;

    if (!exclude_string.empty()) {
      auto l0 = std::find(exclude_string.begin(), exclude_string.end(),
                          left_square_bracket);
      auto r0 = std::find(l0, exclude_string.end(), right_square_bracket);
      exclude_string =
          exclude_string.substr(std::distance(exclude_string.begin(), l0) + 1,
                                std::distance(l0, r0) - 1);
    }
    std::string::iterator eit = exclude_string.begin();

    // Re create the excluded dates in a vector
    while (eit != exclude_string.end()) {
      if (*eit == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
        time_order.excluded_dates.push_back(single_time_container);
        single_time_container.clear();
      } else {
        // std::cout << "Reading character: '" << *it << "'\n";
        single_time_container += *eit;
      }
      ++eit;
    }

    // After loop: push remaining part if any
    if (!single_time_container.empty()) {
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.excluded_dates.push_back(single_time_container);
    }
    for (const std::string& time : time_order.times) {
      if (!std::regex_match(time, time12h_regex)) {
        throw std::runtime_error("Invalid 12h time format: " + time);
      }
    }
  } else if (mode_state == Modes::NoRepeat24) {
    std::string temp_time_container;
    std::string::iterator it = time_string.begin();

    // parsing time list but while(true) is bad practice because of bounds
    // TODO: make a general split function to clean up code
    while (it != time_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
        time_order.times.push_back(temp_time_container);
        temp_time_container.clear();
      } else {
        // std::cout << "Reading character: '" << *it << "'\n";
        temp_time_container += *it;
      }
      ++it;
    }

    // After loop: push remaining part if any
    if (!temp_time_container.empty()) {
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.times.push_back(temp_time_container);
    }

    it = date_string.begin();
    std::string single_date_container;

    while (it != date_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
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
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.dates.push_back(single_date_container);
    }
  }
  // If the mode is Repeat12 or NoRepeat12 we have to check if every time has an
  // AM or PM suffix We also have to add date validation
  else if (mode_state == Modes::Repeat24) {
    std::string temp_time_container;
    std::string::iterator it = time_string.begin();

    // parsing time list but while(true) is bad practice because of bounds
    // TODO: make a general split function to clean up code
    while (it != time_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
        time_order.times.push_back(temp_time_container);
        temp_time_container.clear();
      } else {
        // std::cout << "Reading character: '" << *it << "'\n";
        temp_time_container += *it;
      }
      if (mode_state == Modes::NoRepeat12) {
        // Parse AM PM
        std::string temp_time_container;
        std::string::iterator it = time_string.begin();

        // parsing time list but while(true) is bad practice because of bounds
        // TODO: make a general split function to clean up code
        while (it != time_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
            time_order.times.push_back(temp_time_container);
            temp_time_container.clear();
          } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            temp_time_container += *it;
          }
          ++it;
        }

        // After loop: push remaining part if any
        if (!temp_time_container.empty()) {
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.times.push_back(temp_time_container);
        }

        it = date_string.begin();
        std::string single_date_container;

        while (it != date_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
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
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.dates.push_back(single_date_container);
        }
      } else if (mode_state == Modes::Repeat12) {
        std::string temp_time_container;
        std::string::iterator it = time_string.begin();

        // parsing time list but while(true) is bad practice because of bounds
        // TODO: make a general split function to clean up code
        while (it != time_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
            time_order.times.push_back(temp_time_container);
            temp_time_container.clear();
          } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            temp_time_container += *it;
          }
          ++it;
        }

        // After loop: push remaining part if any
        if (!temp_time_container.empty()) {
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.times.push_back(temp_time_container);
        }

        it = date_string.begin();
        std::string single_date_container;

        while (it != date_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
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
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.dates.push_back(single_date_container);
        }
        time_order.repeat_schedule = repeat_string;

        std::string single_time_container;

        if (!exclude_string.empty()) {
          auto l0 = std::find(exclude_string.begin(), exclude_string.end(),
                              left_square_bracket);
          auto r0 = std::find(l0, exclude_string.end(), right_square_bracket);
          exclude_string = exclude_string.substr(
              std::distance(exclude_string.begin(), l0) + 1,
              std::distance(l0, r0) - 1);
        }
        std::string::iterator eit = exclude_string.begin();

        // Re create the excluded dates in a vector
        while (eit != exclude_string.end()) {
          if (*eit == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
            time_order.excluded_dates.push_back(single_time_container);
            single_time_container.clear();
          } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            single_time_container += *eit;
          }
          ++eit;
        }

        // After loop: push remaining part if any
        if (!single_time_container.empty()) {
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.excluded_dates.push_back(single_time_container);
        }

      } else if (mode_state == Modes::NoRepeat24) {
        std::string temp_time_container;
        std::string::iterator it = time_string.begin();

        // parsing time list but while(true) is bad practice because of bounds
        // TODO: make a general split function to clean up code
        while (it != time_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
            time_order.times.push_back(temp_time_container);
            temp_time_container.clear();
          } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            temp_time_container += *it;
          }
          ++it;
        }

        // After loop: push remaining part if any
        if (!temp_time_container.empty()) {
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.times.push_back(temp_time_container);
        }

        it = date_string.begin();
        std::string single_date_container;

        while (it != date_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
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
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.dates.push_back(single_date_container);
        }
      }
      // If the mode is Repeat12 or NoRepeat12 we have to check if every time
      // has an AM or PM suffix We also have to add date validation
      else if (mode_state == Modes::Repeat24) {
        std::string temp_time_container;
        std::string::iterator it = time_string.begin();

        // parsing time list but while(true) is bad practice because of bounds
        // TODO: make a general split function to clean up code
        while (it != time_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
            time_order.times.push_back(temp_time_container);
            temp_time_container.clear();
          } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            temp_time_container += *it;
          }
          ++it;
        }

        // After loop: push remaining part if any
        if (!temp_time_container.empty()) {
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.times.push_back(temp_time_container);
        }

        it = date_string.begin();
        std::string single_date_container;

        while (it != date_string.end()) {
          if (*it == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
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
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.dates.push_back(single_date_container);
        }

        time_order.repeat_schedule = repeat_string;

        std::string single_time_container;

        if (!exclude_string.empty()) {
          auto l0 = std::find(exclude_string.begin(), exclude_string.end(),
                              left_square_bracket);
          auto r0 = std::find(l0, exclude_string.end(), right_square_bracket);
          exclude_string = exclude_string.substr(
              std::distance(exclude_string.begin(), l0) + 1,
              std::distance(l0, r0) - 1);
        }
        std::string::iterator eit = exclude_string.begin();

        // Re create the excluded dates in a vector
        while (eit != exclude_string.end()) {
          if (*eit == ',') {
            // std::cout << "Split occurred at comma. Pushing: \"" <<
            // single_time_container << "\"\n";
            time_order.excluded_dates.push_back(single_time_container);
            single_time_container.clear();
          } else {
            // std::cout << "Reading character: '" << *it << "'\n";
            single_time_container += *eit;
          }
          ++eit;
        }

        // After loop: push remaining part if any
        if (!single_time_container.empty()) {
          // std::cout << "End of string reached. Final push: \"" <<
          // single_time_container << "\"\n";
          time_order.excluded_dates.push_back(single_time_container);
        }
      }

      ++it;
    }

    // After loop: push remaining part if any
    if (!temp_time_container.empty()) {
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.times.push_back(temp_time_container);
    }

    it = date_string.begin();
    std::string single_date_container;

    while (it != date_string.end()) {
      if (*it == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
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
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.dates.push_back(single_date_container);
    }

    time_order.repeat_schedule = repeat_string;

    std::string single_time_container;

    if (!exclude_string.empty()) {
      auto l0 = std::find(exclude_string.begin(), exclude_string.end(),
                          left_square_bracket);
      auto r0 = std::find(l0, exclude_string.end(), right_square_bracket);
      exclude_string =
          exclude_string.substr(std::distance(exclude_string.begin(), l0) + 1,
                                std::distance(l0, r0) - 1);
    }
    std::string::iterator eit = exclude_string.begin();

    // Re create the excluded dates in a vector
    while (eit != exclude_string.end()) {
      if (*eit == ',') {
        // std::cout << "Split occurred at comma. Pushing: \"" <<
        // single_time_container << "\"\n";
        time_order.excluded_dates.push_back(single_time_container);
        single_time_container.clear();
      } else {
        // std::cout << "Reading character: '" << *it << "'\n";
        single_time_container += *eit;
      }
      ++eit;
    }

    // After loop: push remaining part if any
    if (!single_time_container.empty()) {
      // std::cout << "End of string reached. Final push: \"" <<
      // single_time_container << "\"\n";
      time_order.excluded_dates.push_back(single_time_container);
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