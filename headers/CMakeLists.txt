cmake_minimum_required(VERSION 3.10)
add_library(Headers time.hpp tasks.hpp parsing.hpp)
set_target_properties(Headers PROPERTIES LINKER_LANGUAGE CXX)
