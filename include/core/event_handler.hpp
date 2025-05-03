#pragma once
#include "club_state.hpp"

namespace core {

class EventHandler {
public:
  virtual ~EventHandler() = default;
  virtual void handle(const Event &event, ClubState &state,
                      std::vector<Event> &output) = 0;
};

} // namespace core
