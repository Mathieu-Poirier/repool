#include "time.hpp"
#include "tasks.hpp"

#include <vector>
#include <assert.h>

int main(){
    std::vector<std::string> dates = {"2025-06-01", "2025-06-02"};
    std::vector<std::string> times = {"08:00", "14:30"};

    TimeOrder mySchedule(
        dates,
        times,
        true,
        "weekly",
        "2025-06-01 08:00",
        "2025-06-10"
    );

    Task task1("backup.sh", 512, mySchedule);
    assert(task1.memory_size == 512 && "CONSTRUCTOR ERROR: memory_size initialization fault");
    assert(task1.schedule_ordered.dates.at(1) == "2025-06-02" && "CONSTRUCTOR ERROR: schedule_ordered.dates initialization fault");
    // Failing test
    // assert(task1.schedule_ordered.dates.at(0) == "2025-06-02" && "CONSTRUCTOR ERROR: schedule_ordered.dates initialization fault");

    std::string current_time = get_current_time();
    std::cout << current_time << std::endl;
    return 0;
}