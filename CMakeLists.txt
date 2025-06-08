cmake_minimum_required(VERSION 3.10)
project(Repool VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED 26)
add_subdirectory(headers)
add_subdirectory(tests)

add_executable(repool src/repool.cpp)

target_link_libraries(repool PUBLIC Headers)

configure_file(headers/version.hpp.in headers/version.hpp)

target_include_directories(repool PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_BINARY_DIR}/src"
                          "${PROJECT_SOURCE_DIR}/headers"
                          "${PROJECT_SOURCE_DIR}/tests"
                          )

enable_testing()
add_test(NAME TestSuite COMMAND repool_tests)