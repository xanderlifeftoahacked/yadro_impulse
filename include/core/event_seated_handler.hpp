#pragma once
#include "event_handler.hpp"

namespace core {

class ClientSeatedHandler : public EventHandler {
public:
  void handle(const Event &event, ClubState &state,
              std::vector<Event> &output) override;
};

} // namespace core
