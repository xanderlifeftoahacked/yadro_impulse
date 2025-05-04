#pragma once
#include "club_state.hpp"
#include <ostream>

using namespace core;
namespace io {

class OutputWriter {
public:
  static void print_output(std::ostream &out, const std::vector<Event> &events,
                           ClubState &club) {
    std::vector<core::Event> output;

    output.emplace_back(Event(club.opening_time_, EventType::CLUB_OPEN,
                              std::vector<std::string>{}));

    for (const auto &event : events) {
      if (event.get_time() < club.closing_time_) {
        output.push_back(event);
        club.process_event(event, output);
      } else if (!club.closed) {
        club.close(output);
        output.push_back(event);
        club.process_event(event, output);
      } else {
        club.process_event(event, output);
        output.push_back(event);
      }
    }

    club.close(output);
    output.emplace_back(Event(club.closing_time_, EventType::CLUB_CLOSE,
                              std::vector<std::string>{}));

    std::vector<std::string> result;
    for (const auto &e : output) {
      result.push_back(e.to_string());
    }

    for (uint16_t i = 1; i <= club.num_tables_; ++i) {
      const auto &t = club.table(i);
      result.push_back(
          std::to_string(i) + " " + std::to_string(t.revenue) + " " +
          Time(t.total_minutes / 60, t.total_minutes % 60).to_string());
    }

    for (const std::string &str : result) {
      out << str << std::endl;
    }
  }
};

} // namespace io
