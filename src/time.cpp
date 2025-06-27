#include "../headers/time.hpp"

std::string get_current_time() {
  time_t timestamp;
  time(&timestamp);
  const char *time_as_string = ctime(&timestamp);
  return time_as_string;
}

bool is_valid_date(int y, unsigned m, unsigned d) {
  using namespace std::chrono;
  std::chrono::year_month_day ymd{year{y}, month{m}, day{d}};
  return ymd.ok();
}
