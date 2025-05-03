#pragma once
#include "core/event.hpp"
#include "core/time.hpp"
#include <cstdint>
#include <list>
#include <map>
#include <vector>

namespace core {

struct ClientInfo {
  uint16_t table_id = 0;
  bool is_waiting = false;
  std::list<std::string>::iterator queue_pos;
};

struct Table {
  uint16_t number;
  bool is_occupied = false;
  Time start_time;
  uint32_t revenue = 0;
  uint32_t total_minutes = 0;
};

class ClubState {
public:
  uint16_t num_tables_;
  Time opening_time_;
  Time closing_time_;
  uint32_t hour_cost_;

  std::map<std::string, ClientInfo> clients_;
  std::vector<Table> tables_;
  std::list<std::string> waiting_queue_;

  ClubState(uint16_t num_tables, Time opening, Time closing,
            uint32_t hour_cost);

  void process_event(const Event &event, std::vector<Event> &output);
  void close(std::vector<Event> &output);

  const std::map<std::string, ClientInfo> &clients() const { return clients_; }
  const Table &table(uint16_t id) const { return tables_[id]; }

  void free_table(uint16_t table_id, Time end_time, std::vector<Event> &output);
  void seat_client(const std::string &client, uint16_t table_id, Time time);
  void calculate_revenue(Table &table, Time end_time);
  bool has_free_tables() const;
};

} // namespace core
