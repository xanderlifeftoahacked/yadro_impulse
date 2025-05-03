#include "core/event_waiting_handler.hpp"

void core::ClientWaitingHandler::handle(const Event &event, ClubState &club,
                                        std::vector<Event> &output) {
  const auto &client = event.get_body()[0];
  const auto &time = event.get_time();

  if (!club.clients_.count(client)) {
    output.emplace_back(
        Event(time, EventType::ERROR_OCCURRED, {"ClientUnknown"}));
    return;
  }

  if (club.has_free_tables()) {
    output.emplace_back(
        Event(time, EventType::ERROR_OCCURRED, {"ICanWaitNoLonger!"}));
    return;
  }

  if (club.waiting_queue_.size() >= club.num_tables_) {
    output.emplace_back(Event(time, EventType::CLIENT_FORCE_LEFT, {client}));
    club.clients_.erase(client);
    return;
  }

  club.waiting_queue_.push_back(client);
  auto &info = club.clients_[client];
  info.is_waiting = true;
  info.queue_pos = --club.waiting_queue_.end();
}
