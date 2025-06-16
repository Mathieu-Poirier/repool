#pragma once
#include <string>
#include <vector>

enum class Modes {
    NoRepeat12,
    Repeat12,
    NoRepeat24,
    Repeat24,
    Size
};
    
// Could update to use own allocator like allocate chunks of heap memory
// Refactor to just a single class probably
class TimeOrder {
public:
    std::vector<std::string> dates;
    std::vector<std::string> times;
    Modes repeat_mode;
    std::string repeat_schedule;
    std::string next_run; // 365 state options
    std::vector<std::string>  excluded_dates; // 365 state options

    TimeOrder() {
        dates = {};
        times = {};
        excluded_dates = {};
        repeat_mode = Modes::NoRepeat12;
        repeat_schedule = "";
        next_run = "";
    }

    TimeOrder(const std::vector<std::string>& dates,
              const std::vector<std::string>& times,
              Modes repeat_mode,
              const std::string& repeat_schedule,
              const std::string& next_run,
              const std::vector<std::string>& excluded_dates) {
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
    TimeOrder schedule_ordered;

    Task(){
        this->path = "";
        this->schedule_ordered = TimeOrder();
    }

    Task(const std::string& path, const TimeOrder& schedule_ordered) {
        this->path = path;
        this->schedule_ordered = schedule_ordered;
    }
};
