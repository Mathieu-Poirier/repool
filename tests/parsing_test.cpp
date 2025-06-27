// Auto-generated test for parsing
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../headers/parsing.hpp"

int main() {
    std::cout << "Running tests for parsing..." << std::endl;

    // Test: well-formed input
    {
        std::string input = "Mar-21-2026";
        bool result = validate_string_date(input);
        assert(result && "Failed: expected well-formed date to pass");
        std::cout << "OK: Well-formed input passed" << std::endl;
    }

    // Test: ill-formed input (missing day)
    {
        std::string input = "Mar--21-2026";
        bool exception_thrown = false;
        try {
            validate_string_date(input);
        } catch (const std::runtime_error& e) {
            std::cout << "Caught expected exception: " << e.what() << std::endl;
            exception_thrown = true;
        }
        assert(exception_thrown && "Expected exception not thrown for ill-formed input");
        std::cout << "OK: Ill-formed input correctly detected" << std::endl;
    }

    // Test: extract_bracket_blocks well-formed with nested brackets
    bool exception_thrown = false;
    try
    {
   
        std::string input = "[NoRepeat12][[]]";
        std::vector<std::string> blocks = extract_bracket_blocks(input);
        assert(blocks.size() == 2);
        assert(blocks[0] == "NoRepeat12");
        assert(blocks[1] == "[");
    }
    catch(const std::runtime_error& e){
        std::cout << "Caught expected exception: " << e.what() << std::endl;
        exception_thrown = true;
    }
    assert(exception_thrown && "Expected exception not thrown for ill-formed input: [NoRepeat12][[]]");
    std::cout << "OK: Parsed [NoRepeat12][[]] throws for ill-formed input" << std::endl;

    exception_thrown = false;

    // Test for small block size

    try
    {
        std::string input = "[3x]";
        std::vector<std::string> blocks = extract_bracket_blocks(input);
        assert(blocks.size() == 1);
        assert(blocks[0] == "3x");
    }
    catch(const std::runtime_error& e){
        std::cout << "Caught expected exception: " << e.what() << std::endl;
        exception_thrown = true;
    }
    assert(exception_thrown && "Expected exception not thrown for ill-formed input: [3x]");
    std::cout << "OK: Parsed [3x] throws for too little blocks" << std::endl;


    // Placeholder for future tests
    std::cout << "All parsing tests passed" << std::endl;
    return 0;
}
