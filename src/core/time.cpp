#include "core/time.hpp"
#include "utils/utils.hpp"
#include <cctype>
#include <iomanip>
#include <sstream>

namespace core {

void Time::validate() const {
  if (hours >= 24 || minutes >= 60) {
    throw std::invalid_argument("Invalid time value");
  }
}

Time::Time() : hours(0), minutes(0) {}

Time::Time(uint16_t h, uint16_t m) : hours(h), minutes(m) { validate(); }

Time Time::from_string(const std::string &s) {
  if (s.length() != 5 || s[2] != ':' || utils::count_digits(s) != 4) {
    throw std::invalid_argument("Invalid time format");
  }

  uint16_t h = std::stoi(s.substr(0, 2));
  uint16_t m = std::stoi(s.substr(3, 2));

  return Time(h, m);
}

std::string Time::to_string() const {
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0')
      << std::setw(2) << minutes;
  return oss.str();
}

bool Time::operator<(const Time &other) const {
  return (hours * 60 + minutes) < (other.hours * 60 + other.minutes);
}

bool Time::operator<=(const Time &other) const {
  return (hours * 60 + minutes) <= (other.hours * 60 + other.minutes);
}

bool Time::operator>(const Time &other) const {
  return (hours * 60 + minutes) > (other.hours * 60 + other.minutes);
}

bool Time::operator>=(const Time &other) const {
  return (hours * 60 + minutes) >= (other.hours * 60 + other.minutes);
}

bool Time::operator==(const Time &other) const {
  return hours == other.hours && minutes == other.minutes;
}

bool Time::operator!=(const Time &other) const { return !(*this == other); }

Time Time::add_minutes(int16_t mins) const {
  int16_t total = hours * 60 + minutes + mins;
  total = (total % 1440 + 1440) % 1440;
  return Time(total / 60, total % 60);
}

int16_t Time::operator-(const Time &other) const {
  return (static_cast<int16_t>((hours * 60 + minutes)) -
          (other.hours * 60 + other.minutes) + 1440) %
         1440;
}

} // namespace core
