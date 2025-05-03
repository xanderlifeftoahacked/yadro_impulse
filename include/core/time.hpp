#pragma once
#include <cstdint>
#include <string>

namespace core {

class Time {
  uint16_t hours;
  uint16_t minutes;

public:
  Time();
  Time(uint16_t hours, uint16_t minutes);
  static Time from_string(const std::string &s);
  std::string to_string() const;
  void validate() const;

  Time add_minutes(int16_t minutes) const;
  int16_t operator-(const Time &other) const;

  uint16_t get_hours() const { return hours; }
  uint16_t get_minutes() const { return minutes; }

  bool operator<(const Time &other) const;
  bool operator<=(const Time &other) const;
  bool operator>(const Time &other) const;
  bool operator>=(const Time &other) const;
  bool operator==(const Time &other) const;
  bool operator!=(const Time &other) const;
};

} // namespace core
