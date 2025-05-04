#include "core/event.hpp"
#include "core/time.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

using namespace core;

Event::Event(Time time, EventType type, std::vector<std::string> body)
    : time_(time), type_(type), body_(std::move(body)) {}

Event Event::from_string(const std::string &input) {
  std::istringstream iss(input);
  std::vector<std::string> parts;
  std::string part;

  while (iss >> part) {
    parts.push_back(part);
  }

  if (parts.size() < 3 || parts.size() > 4) {
    throw std::invalid_argument("Invalid event format");
  }

  Time time = Time::from_string(parts[0]);
  uint8_t id = std::stoi(parts[1]);
  EventType type = static_cast<EventType>(id);

  if (type < EventType::CLIENT_ARRIVED || type > EventType::ERROR_OCCURRED) {
    throw std::invalid_argument("Invalid event ID");
  }

  std::vector<std::string> body(parts.begin() + 2, parts.end());
  return Event(time, type, body);
}

Event Event::create_error(Time time, ErrorType error) {
  std::string error_msg;
  switch (error) {
  case ErrorType::NOT_OPEN_YET:
    error_msg = "NotOpenYet";
    break;
  case ErrorType::YOU_SHALL_NOT_PASS:
    error_msg = "YouShallNotPass";
    break;
  case ErrorType::CLIENT_UNKNOWN:
    error_msg = "ClientUnknown";
    break;
  case ErrorType::PLACE_IS_BUSY:
    error_msg = "PlaceIsBusy";
    break;
  case ErrorType::I_CAN_WAIT_NO_LONGER:
    error_msg = "ICanWaitNoLonger!";
    break;
  default:
    throw std::invalid_argument("Unknown error type");
  }
  return Event(time, EventType::ERROR_OCCURRED, {error_msg});
}

std::string Event::to_string() const {
  std::ostringstream oss;
  if (static_cast<int>(type_) > 0) {
    oss << time_.to_string() << ' ' << static_cast<int>(type_) << ' ';
  } else {
    oss << time_.to_string() << ' ';
  }

  for (const auto &param : body_) {
    oss << param << ' ';
  }

  std::string result = oss.str();
  if (!result.empty())
    result.pop_back();
  return result;
}

bool Event::is_valid() const {
  switch (type_) {
  case EventType::CLIENT_ARRIVED:
    return body_.size() == 1;

  case EventType::CLIENT_SEATED:
    return body_.size() == 2 &&
           std::all_of(body_[1].begin(), body_[1].end(), ::isdigit);

  case EventType::CLIENT_WAITING:
  case EventType::CLIENT_LEFT:
    return body_.size() == 1;

  case EventType::CLIENT_FORCE_LEFT:
  case EventType::CLIENT_SEATED_FROM_QUEUE:
    return body_.size() == 1;

  case EventType::ERROR_OCCURRED:
    return body_.size() == 1 &&
           (body_[0] == "NotOpenYet" || body_[0] == "YouShallNotPass" ||
            body_[0] == "ClientUnknown" || body_[0] == "PlaceIsBusy" ||
            body_[0] == "ICanWaitNoLonger!");

  default:
    return false;
  }
}

bool Event::operator<(const Event &other) const { return time_ < other.time_; }
