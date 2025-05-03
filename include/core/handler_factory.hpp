#pragma once
#include "event.hpp"
#include "event_handler.hpp"
#include <memory>
#include <unordered_map>

namespace core {

class HandlerFactory {
  std::unordered_map<EventType, std::unique_ptr<EventHandler>> handlers_;

public:
  HandlerFactory();
  EventHandler *get_handler(EventType type) const;
};

} // namespace core
