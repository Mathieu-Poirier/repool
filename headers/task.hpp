#pragma once
#include "../headers/time_order.hpp"

class Task {
public:
    static inline int next_id = 0; // static member to track next ID
    int id;                        // instance ID
    std::string path;
    TimeOrder schedule_ordered;

    Task() : id(next_id++), path(""), schedule_ordered() {}

    Task(const std::string &path, const TimeOrder &schedule_ordered)
        : id(next_id++), path(path), schedule_ordered(schedule_ordered) {}
};


