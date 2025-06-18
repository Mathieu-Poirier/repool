#pragma once
#include <ctime>
#include <string>

inline std::string get_current_time() {
  time_t timestamp;
  time(&timestamp);
  const char *time_as_string = ctime(&timestamp);
  return time_as_string;
}