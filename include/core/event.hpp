#pragma once
#include "time.hpp"
#include <vector>

namespace core {

enum class EventType {
  CLIENT_ARRIVED = 1,
  CLIENT_SEATED = 2,
  CLIENT_WAITING = 3,
  CLIENT_LEFT = 4,
  CLIENT_FORCE_LEFT = 11,
  CLIENT_SEATED_FROM_QUEUE = 12,
  ERROR_OCCURRED = 13,
  CLUB_OPEN = -1,
  CLUB_CLOSE = -2
};

enum class ErrorType {
  NOT_OPEN_YET,
  YOU_SHALL_NOT_PASS,
  CLIENT_UNKNOWN,
  PLACE_IS_BUSY,
  I_CAN_WAIT_NO_LONGER
};

class Event {
  Time time_;
  EventType type_;
  std::vector<std::string> body_;

public:
  Event(Time time, EventType type, std::vector<std::string> body);

  static Event from_string(const std::string &input);
  static Event create_error(Time time, ErrorType error);

  std::string to_string() const;

  Time get_time() const { return time_; }
  EventType get_type() const { return type_; }
  const std::vector<std::string> &get_body() const { return body_; }

  bool is_valid() const;

  bool operator<(const Event &other) const;
};

} // namespace core
