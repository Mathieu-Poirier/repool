#pragma once
#include <iostream>
#include <string>
#include <vector>

// Could update to use own allocator like allocate chunks of heap memory
class TimeOrder {
public:
    std::vector<std::string> dates;
    std::vector<std::string> times;
    bool repeat_mode;
    std::string repeat_schedule;
    std::string next_run; // 365 state options
    std::string excluded_dates; // 365 state options

    TimeOrder() {
        repeat_mode = false;
        repeat_schedule = "";
        next_run = "";
        excluded_dates = "";
    }

    TimeOrder(const std::vector<std::string>& dates,
              const std::vector<std::string>& times,
              bool repeat_mode,
              const std::string& repeat_schedule,
              const std::string& next_run,
              const std::string& excluded_dates) {
        this->dates = dates;
        this->times = times;
        this->repeat_mode = repeat_mode;
        this->repeat_schedule = repeat_schedule;
        this->next_run = next_run;
        this->excluded_dates = excluded_dates;
    }
};

class Task {
public:
    std::string path;
    int memory_size;
    TimeOrder schedule_ordered;

    Task(){
        this->path = "";
        this->memory_size = 0;
        this->schedule_ordered = TimeOrder();
    }

    Task(const std::string& path, int memory_size, const TimeOrder& schedule_ordered) {
        this->path = path;
        this->memory_size = memory_size;
        this->schedule_ordered = schedule_ordered;
    }
};
