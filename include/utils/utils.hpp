#pragma once
#include "time.hpp"
#include <cctype>
#include <cstdint>
#include <string>

using namespace core;
namespace utils {

struct ClubConfig {
  uint16_t num_tables_;
  Time opening_time_;
  Time closing_time_;
  uint32_t hour_cost_;
};

size_t count_digits(const std::string &s);

} // namespace utils
