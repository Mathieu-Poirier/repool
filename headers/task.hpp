#pragma once
#include "../headers/time_order.hpp"

class Task {
public:
  std::string path;
  TimeOrder schedule_ordered;

  Task() {
    this->path = "";
    this->schedule_ordered = TimeOrder();
  }

  Task(const std::string &path, const TimeOrder &schedule_ordered) {
    this->path = path;
    this->schedule_ordered = schedule_ordered;
  }
};
