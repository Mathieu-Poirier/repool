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
add_test(NAME NoArgs COMMAND repool_tests)
add_test(NAME ProperArgs COMMAND repool_tests job=man hello=5)
add_test(NAME MaxArgs COMMAND repool_tests job=man hello=5 hello=5 hello=5)
add_test(NAME TooManyArgs COMMAND repool_tests job=man hello=5 hello=5 hello=5 hello=5 hello=5)
add_test(NAME FiveArgs COMMAND repool_tests job=man hello=5 hello=5 hello=5 hello=5)
add_test(NAME AllFieldsAndOne COMMAND repool_tests job=home/src execTime=5 mode=5 excludedDates=5 hello=5)
add_test(NAME AllFields COMMAND repool_tests job=home/src execTime=5 mode=5 excludedDates=5)
add_test(NAME AllFieldsPermutation1 COMMAND repool_tests execTime=5 job=home/src mode=5 excludedDates=5)
add_test(NAME AllFieldsPermutation2 COMMAND repool_tests job=home/src mode=5 excludedDates=5 execTime=5)
add_test(NAME RunsWith<>Token COMMAND repool_tests job=home/<src> mode=5 excludedDates=5 execTime=<5.0>)
add_test(NAME NewCorrectForm COMMAND repool_tests job=home/static exec=[NoRepeat12][12:00AM][Jun-15-2026] repeat=weekly)
add_test(NAME Invalid12h COMMAND repool_tests job=home/static exec=[NoRepeat12][12:00][Jun-15-2026] repeat=weekly)
add_test(NAME Invalid12h_2 COMMAND repool_tests job=home/static exec=[NoRepeat12][13:00AM][Jun-15-2026] repeat=weekly)