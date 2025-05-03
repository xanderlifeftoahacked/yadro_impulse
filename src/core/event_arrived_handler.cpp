#include "core/event_arrived_handler.hpp"

void core::ClientArrivedHandler::handle(const Event &event, ClubState &club,
                                        std::vector<Event> &output) {
  const auto &client = event.get_body()[0];
  const auto &time = event.get_time();

  if (time < club.opening_time_ || time >= club.closing_time_) {
    output.emplace_back(Event(time, EventType::ERROR_OCCURRED, {"NotOpenYet"}));
    return;
  }

  if (club.clients_.count(client)) {
    output.emplace_back(
        Event(time, EventType::ERROR_OCCURRED, {"YouShallNotPass"}));
    return;
  }

  club.clients_.emplace(client, ClientInfo{});
}
