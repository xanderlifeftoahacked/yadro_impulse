#pragma once
#include "event.hpp"
#include "utils/utils.hpp"
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace core;
namespace io {

class InputParser {
  template <typename T, typename Parser, typename Validator>
  static auto parse_line(const std::string &line, size_t line_num,
                         Parser &&parser, Validator &&validator,
                         const std::string &error_msg) {
    try {
      T value = parser(line);
      if (!validator(value)) {
        throw std::invalid_argument(error_msg);
      }
      return value;
    } catch (...) {
      throw std::invalid_argument("Line " + std::to_string(line_num) + ": " +
                                  error_msg);
    }
  }

  static utils::ClubConfig read_config(std::ifstream &input_file,
                                       size_t &line_num) {
    utils::ClubConfig config;
    std::string line;

    if (!std::getline(input_file, line))
      throw std::invalid_argument("Missing table count");

    config.num_tables_ = parse_line<int64_t>(
        line, line_num++,
        [](auto &s) {
          return std::all_of(s.begin(), s.end(), ::isdigit) ? std::stoll(s)
                                                            : -1;
        },
        [](auto n) { return n <= UINT16_MAX && n > 0; }, "Invalid table count");

    if (!std::getline(input_file, line))
      throw std::invalid_argument("Missing working hours");
    std::istringstream time_ss(line);
    std::string open_str, close_str;
    if (!(time_ss >> open_str >> close_str))
      throw std::invalid_argument("Invalid time format");

    config.opening_time_ = parse_line<Time>(
        open_str, line_num, [](auto &s) { return Time::from_string(s); },
        [](auto &&) { return true; }, "Invalid opening time");

    config.closing_time_ = parse_line<Time>(
        close_str, line_num, [](auto &s) { return Time::from_string(s); },
        [&](auto &t) { return t > config.opening_time_; },
        "Invalid closing time");
    line_num++;

    if (!std::getline(input_file, line))
      throw std::invalid_argument("Missing hour price");
    config.hour_cost_ = parse_line<int64_t>(
        line, line_num++,
        [](auto &s) {
          return std::all_of(s.begin(), s.end(), ::isdigit) ? std::stoll(s)
                                                            : -1;
        },
        [](auto h) { return h <= UINT32_MAX && h > 0; }, "Invalid hour price");

    return config;
  }

public:
  static std::pair<utils::ClubConfig, std::vector<Event>>
  parse_input_file(const std::string &path) {
    if (!std::filesystem::exists(path))
      throw std::invalid_argument("File " + path + " doesn't exist");

    std::ifstream input_file(path);
    if (!input_file.good())
      throw std::invalid_argument("Can't open file: " + path);

    utils::ClubConfig config;
    std::vector<Event> events;
    size_t line_num = 1;

    try {
      config = read_config(input_file, line_num);
      Time prev_time;

      for (std::string line; std::getline(input_file, line); line_num++) {
        if (line.empty())
          continue;

        Event event = parse_line<Event>(
            line, line_num, [](auto &s) { return Event::from_string(s); },
            [&](auto &e) {
              return events.empty() || e.get_time() >= prev_time;
            },
            "Invalid event");

        prev_time = event.get_time();
        events.push_back(event);
      }
    } catch (const std::exception &e) {
      throw std::runtime_error("Parsing failed: " + std::string(e.what()));
    }

    return {config, events};
  }
};

} // namespace io
