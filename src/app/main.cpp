#include "club_state.hpp"
#include "io/input_parser.hpp"
#include "output_writer.hpp"
#include <exception>
#include <iostream>
int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cerr << "Correct usage: pcclub <filename>" << std::endl;
    return 1;
  }

  try {
    auto [config, events] = io::InputParser::parse_input_file(argv[1]);

    ClubState club(config.num_tables_, config.opening_time_,
                   config.closing_time_, config.hour_cost_);

    io::OutputWriter::print_output(std::cout, events, club);
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
