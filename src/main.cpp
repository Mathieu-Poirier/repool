#include "../headers/parsing.hpp"
#include "../headers/time.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

// input and daemon different like the UI just adds to a file which could be
// locked if open might have to fork the daemon process that just reads and
// writes to file but the CLI takes in Might need some interprocess communicate
// (pipes) for one process being the producer and one being the consumer (the
// daemon)

// Need to be able to signal to the consumer to delete jobs by id either by file
// or pipes Use flock for file locking Create new job function Need to be able
// to singal to consumer to show current jobs -> id: path

int main(int argc, char **argv) {
  std::string current_time = get_current_time();
  std::cout << current_time << std::endl;
  try {
    parse_command_line_arguments(argc, argv);
  } catch (std::runtime_error error) {
    std::cout << error.what() << std::endl;
    return 1;
  }
  return 0;
}