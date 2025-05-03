#include "core/club_state.hpp"
#include "core/handler_factory.hpp"
#include <algorithm>

using namespace core;

ClubState::ClubState(uint16_t num_tables, Time opening, Time closing,
                     uint32_t hour_cost)
    : num_tables_(num_tables), opening_time_(opening), closing_time_(closing),
      hour_cost_(hour_cost) {
  tables_.resize(num_tables_ + 1);
  for (uint16_t i = 1; i <= num_tables_; ++i) {
    tables_[i].number = i;
  }
}

void ClubState::process_event(const Event &event, std::vector<Event> &output) {
  static HandlerFactory factory;

  if (auto handler = factory.get_handler(event.get_type())) {
    handler->handle(event, *this, output);
  } else {
    output.emplace_back(Event(event.get_time(), EventType::ERROR_OCCURRED,
                              {"UnknownEventType"}));
  }
}

void ClubState::close(std::vector<Event> &output) {
  std::vector<std::string> remaining_clients;
  for (const auto &[client, _] : clients_) {
    remaining_clients.push_back(client);
  }
  std::sort(remaining_clients.begin(), remaining_clients.end());

  for (const auto &client : remaining_clients) {
    output.emplace_back(
        Event{closing_time_, EventType::CLIENT_FORCE_LEFT, {client}});

    if (clients_[client].table_id != 0) {
      free_table(clients_[client].table_id, closing_time_, output);
    }
  }
  clients_.clear();
}

void ClubState::free_table(uint16_t table_id, Time end_time,
                           std::vector<Event> &output) {
  Table &table = tables_[table_id];
  calculate_revenue(table, end_time);
  table.is_occupied = false;

  if (!waiting_queue_.empty()) {
    const std::string next_client = waiting_queue_.front();
    waiting_queue_.pop_front();
    seat_client(next_client, table_id, end_time);
    output.emplace_back(Event{end_time,
                              EventType::CLIENT_SEATED_FROM_QUEUE,
                              {next_client, std::to_string(table_id)}});
  }
}

void ClubState::seat_client(const std::string &client, uint16_t table_id,
                            Time time) {
  Table &table = tables_[table_id];
  table.is_occupied = true;
  table.start_time = time;

  ClientInfo &info = clients_[client];
  info.table_id = table_id;
  info.is_waiting = false;
}

void ClubState::calculate_revenue(Table &table, Time end_time) {
  uint32_t minutes = end_time - table.start_time;
  table.revenue += ((minutes + 59) / 60) * hour_cost_;
  table.total_minutes += minutes;
}

bool ClubState::has_free_tables() const {
  for (uint16_t i = 1; i <= num_tables_; ++i) {
    if (!tables_[i].is_occupied)
      return true;
  }
  return false;
}
