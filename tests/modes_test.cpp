// Auto-generated test for modes
// need to refactor for iostream probably
#include <iostream>
#include <cassert>
#include "../headers/modes.hpp"  // Adjust if needed

int main() {
    std::cout << "Running tests for modes..." << std::endl;

    constexpr Modes mode_NoRepeat12 = Modes::NoRepeat12;
    constexpr Modes mode_Repeat12 = Modes::Repeat12;
    constexpr Modes mode_NoRepeat24 = Modes::NoRepeat24;
    constexpr Modes mode_Repeat24= Modes::Repeat24;
    constexpr Modes mode_enum_size = Modes::Size;
    constexpr int target_enum_size = 4;

    assert(static_cast<int>(Modes::Size) == target_enum_size && "Enum size does not match 4. Has a state been added?");
    std::cout << "OK: Completed enum size check" << std::endl;
    
    return 0;
}
