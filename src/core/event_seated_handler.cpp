#include "core/event_seated_handler.hpp"
#include "core/club_state.hpp"

void core::ClientSeatedHandler::handle(const Event &event, ClubState &club,
                                       std::vector<Event> &output) {
  const auto &client = event.get_body()[0];
  uint16_t table_id = static_cast<uint16_t>(std::stoi(event.get_body()[1]));
  const auto &time = event.get_time();

  if (!club.clients_.count(client)) {
    output.emplace_back(
        Event(time, EventType::ERROR_OCCURRED, {"ClientUnknown"}));
    return;
  }

  if (table_id < 1 || table_id > club.num_tables_ ||
      club.tables_[table_id].is_occupied) {
    output.emplace_back(
        Event(time, EventType::ERROR_OCCURRED, {"PlaceIsBusy"}));
    return;
  }

  auto &info = club.clients_[client];
  if (info.table_id != 0) {
    club.free_table(info.table_id, time, output);
  }

  club.seat_client(client, table_id, time);
}
