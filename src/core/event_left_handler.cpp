#include "core/event_left_handler.hpp"

void core::ClientLeftHandler::handle(const Event &event, ClubState &club,
                                     std::vector<Event> &output) {
  const auto &client = event.get_body()[0];
  const auto &time = event.get_time();

  if (!club.clients_.count(client)) {
    output.emplace_back(
        Event(time, EventType::ERROR_OCCURRED, {"ClientUnknown"}));
    return;
  }

  auto &info = club.clients_[client];

  if (info.table_id != 0) {
    club.free_table(info.table_id, time, output);
  }

  if (info.is_waiting) {
    club.waiting_queue_.erase(info.queue_pos);
  }

  club.clients_.erase(client);
}
